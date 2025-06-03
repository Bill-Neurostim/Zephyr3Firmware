/**@brief Copyright 2019 NeurostimOAB, Inc
*/
#include "global.h"
#include <string.h>
#include "nrf_fstorage.h"
#include "nrf_fstorage_sd.h"
#include "nrf_log.h"

/**@brief Copyright 2019 NeurostimOAB, Inc
*/
#include "global.h"
#include <string.h>
#include "nrf_fstorage.h"
#include "nrf_fstorage_sd.h"
#include "nrf_log.h"

// WARNING: be careful placing the flash, since we are doing this outside the linker, make sure the
// space is available.  For example the linker puts the bootloader at 0x70000.
#define FLASH_MFGREC_START_ADDRESS  (0x00060000)
#define FLASH_MFGREC_SIZE           (0x200)
#define FLASH_LOG_START_ADDRESS     (FLASH_MFGREC_START_ADDRESS+FLASH_MFGREC_SIZE)
#define FLASH_LOG_END_ADDRESS       (FLASH_LOG_START_ADDRESS+RESULTS_SIZE-FLASH_MFGREC_SIZE)

static void fstorage_evt_handler(nrf_fstorage_evt_t * p_evt);

NRF_FSTORAGE_DEF(nrf_fstorage_t fstorage) =
{
    .evt_handler    = fstorage_evt_handler,
    .start_addr     = FLASH_MFGREC_START_ADDRESS,
    .end_addr       = FLASH_LOG_END_ADDRESS-1,
};

void wait_for_flash_ready(nrf_fstorage_t const * p_fstorage)
{
    /* While fstorage is busy, sleep and wait for an event. */
    while (nrf_fstorage_is_busy(p_fstorage))
    {
        (void) sd_app_evt_wait();
    }
}

static void fstorage_evt_handler(nrf_fstorage_evt_t * p_evt)
{
    if (p_evt->result != NRF_SUCCESS)
    {
        NRF_LOG_INFO("--> Event received: ERROR while executing an fstorage operation.");
        return;
    }

    switch (p_evt->id)
    {
        case NRF_FSTORAGE_EVT_WRITE_RESULT:
        {
            NRF_LOG_INFO("--> Event received: wrote %d bytes at address 0x%x.",
                         p_evt->len, p_evt->addr);
        } break;

        case NRF_FSTORAGE_EVT_ERASE_RESULT:
        {
            NRF_LOG_INFO("--> Event received: erased %d page from address 0x%x.",
                         p_evt->len, p_evt->addr);
        } break;

        default:
            break;
    }
}

/**@brief prepare flash area for use as a log
*/
void flash_log_init ( void )
{
    ret_code_t rc;

    nrf_fstorage_api_t  * p_fs_api;

    p_fs_api = &nrf_fstorage_sd;
    rc = nrf_fstorage_init(&fstorage, p_fs_api, NULL);
    APP_ERROR_CHECK(rc);

    // first entry in log should always be a GET_TIME message
    if ( GET_BYTE_3(flash_log_read ( 0 )) != GET_TIME )
        flash_log_erase ();   // if not, clear all pages of log flash

    // now scan the log for first empty entry (0xFFFFFFFF)
    for ( log_wrptr = 0; log_wrptr < RESULTS_SIZE; log_wrptr++ )
        if ( flash_log_read(log_wrptr) == 0xFFFFFFFF ) break;
}

/**@brief erase flash pages used for mfg record and log
*/
void flash_log_erase()
{
    static uint32_t page_address;
    static uint32_t num_pages;

    page_address = FLASH_MFGREC_START_ADDRESS/NRF_FICR->CODEPAGESIZE;
    num_pages    = (FLASH_LOG_END_ADDRESS-FLASH_MFGREC_START_ADDRESS)/NRF_FICR->CODEPAGESIZE;

    fstorage.p_api->erase(&fstorage, page_address, num_pages, NULL);
    wait_for_flash_ready(&fstorage);
}

/**@brief read data from the log based on passed index
*/
uint32_t flash_log_read ( uint32_t index )
{
    static uint32_t page_address;
    static uint32_t data;

    if ( index >= RESULTS_SIZE ) return ( 0 );

    page_address = FLASH_LOG_START_ADDRESS + ( index * 4 );
    fstorage.p_api->read(&fstorage, page_address, &data, NULL);

    return ( data );
}

/**@brief write passed data into the log based on log_wrptr
*/
void flash_log_write( uint32_t value )
{
    static uint32_t src;
    static uint32_t *p_src;
    static uint32_t dest;

    // make sure it is a valid address, if not quit
    if ( log_wrptr >= RESULTS_SIZE ) return;

    src = value;
    p_src = &src;
    dest = FLASH_LOG_START_ADDRESS+(log_wrptr*4);

    fstorage.p_api->write(&fstorage, dest, p_src, 1, NULL);

    wait_for_flash_ready(&fstorage);

    log_wrptr++;
}


/**@brief read data from the log based into passed buffer
*/
void flash_log_buffer_read ( uint32_t *buffer, unsigned int length )
{
    uint32_t *address;

    if ( length >= RESULTS_SIZE ) length = RESULTS_SIZE;

    address = (uint32_t *)FLASH_LOG_START_ADDRESS;
    memcpy ( buffer, address, length ) ;
};

void log_entry ()
{
    uint32_t  get_time;

    get_time = (GET_TIME << 24) | (seconds && 0x00FFFFFF);
    flash_log_write ( get_time );
    flash_log_write ( ble_request );
    flash_log_write ( ble_response );
}
/** @} */