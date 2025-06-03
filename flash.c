/**@brief Copyright 2020 Neurostim Technologies LLC
*/
#include "global.h"
#include <string.h>
#include "nrf_fstorage.h"
#include "nrf_fstorage_sd.h"
#include "nrf_log.h"

// WARNING: be careful placing the flash, since we are doing this outside the linker, make sure the
// space is available.  For example the linker puts the bootloader at 0x70000.
#define FLASH_MFGREC_SIZE           (0x200)
#define LOG_SIZE                    (0x2000)
#define FLASH_MFGREC_START_ADDRESS  (0x00060000)
#define FLASH_LOG_START_ADDRESS     (FLASH_MFGREC_START_ADDRESS+FLASH_MFGREC_SIZE)
#define FLASH_LOG_END_ADDRESS       (FLASH_MFGREC_START_ADDRESS+LOG_SIZE)

static void fstorage_evt_handler(nrf_fstorage_evt_t * p_evt);

NRF_FSTORAGE_DEF(nrf_fstorage_t fstorage) =
{
    .evt_handler    = fstorage_evt_handler,
    .start_addr     = FLASH_MFGREC_START_ADDRESS,
    .end_addr       = FLASH_LOG_END_ADDRESS-1,
};

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
    uint32_t err_code;
    nrf_fstorage_api_t * p_fs_api;

    /* Initialize an fstorage instance using the nrf_fstorage_sd backend.
     * nrf_fstorage_sd uses the SoftDevice to write to flash. This implementation can safely be
     * used whenever there is a SoftDevice, regardless of its status (enabled/disabled). */
    p_fs_api = &nrf_fstorage_sd;

    err_code = nrf_fstorage_init(&fstorage, p_fs_api, NULL);
    APP_ERROR_CHECK(err_code);
}

void flash_check( void )
{
    uint32_t   data;

    // first entry in log should always be a GET_TIME message
    data = flash_log_read ( 0 );
    if ( GET_BIT24_31_BYTE(data) != GET_TIME )
        flash_log_erase ();   // if not, clear all pages of log flash

    last_log_entry = 0;
    // now scan the log for first empty entry (0xFFFFFFFF)
    for ( log_wrptr = 0; log_wrptr < LOG_SIZE; log_wrptr++ )
    {
        // get data from log (uint32)
        data = flash_log_read(log_wrptr);
        // keep track of GET_TIME entries.  Towards the end of valid data in log
        // last_log_entry should contain index of last log entry.  A 'log entry' typically
        // is at least GET_TIME entry, BLE_REQUEST, BLE_RESPONSE
        if ( GET_BIT24_31_BYTE(data) == GET_TIME ) 
            last_log_entry = log_wrptr;
        if ( data == 0xFFFFFFFF ) break;
    }
}

/**@brief erase flash pages used for mfg record and log
*/
void flash_log_erase()
{
    uint32_t err_code;

    err_code = nrf_fstorage_erase(&fstorage, fstorage.start_addr, 2, NULL);
    APP_ERROR_CHECK(err_code);

    log_wrptr = 0;
}

/**@brief read data from the log based on passed index.  Index is uint32_t index into flash.
*/
uint32_t flash_log_read ( uint32_t index )
{
    static uint32_t data;

    if ( index >= LOG_SIZE ) return ( 0 );

    nrf_fstorage_read( &fstorage, FLASH_LOG_START_ADDRESS + (index * sizeof(uint32_t)), &data, sizeof(uint32_t) );

    return ( data );
}

/**@brief write passed data into the log based on log_wrptr
*/
void flash_log_write( uint32_t *buffer, uint32_t size )
{
    // make sure new message doesn't exceed available log
    if ( (log_wrptr + size) >= LOG_SIZE ) return;

    // make sure we have valid space
    nrf_fstorage_write( &fstorage, FLASH_LOG_START_ADDRESS + (log_wrptr * sizeof(uint32_t)), buffer, size * sizeof(uint32_t), NULL );
    log_wrptr += size; 
} 

/**@brief read data from the log based into passed buffer
*/
void flash_log_buffer_read ( uint32_t *dest_buffer, uint32_t index, uint32_t size )
{
    uint32_t data, loop;

    if ( index >= LOG_SIZE ) return;
    if ( index + size >= LOG_SIZE ) size = LOG_SIZE-index;

    for ( loop = 0; loop < size; loop++ )
    {
        data = flash_log_read ( index + loop );
        *dest_buffer = __REV(data);
        dest_buffer++;
        }
};


/**@brief read data from the mfg log based on passed index
*/
uint16_t flash_mfg_read ( uint16_t index )
{
    uint32_t flash_address;
    uint16_t retval;
    uint32_t data = 0;

    // get base address of flash uint32_t (index might point to high or low nibble)
    // even addresses are low 16 bits, odd addresses are high 16 bits
    flash_address = index / 2;
    flash_address *= 4;
    flash_address += FLASH_MFGREC_START_ADDRESS;

    nrf_fstorage_read( &fstorage, flash_address, &data, sizeof(uint32_t) );

    // if we are at an 'odd' address get high order bytes
    if ( index & 1 )
        retval = data >> 16;
    else
        retval = data & 0x0000FFFF;

    return ( retval );
}
/**@brief write passed data into the mfg log at half-word index
*/
void flash_mfg_write( uint8_t index, uint16_t value )
{
    uint32_t flash_address;
    static uint32_t old_data;
    static uint32_t new_data;

    flash_address = index / 2;
    flash_address *= 4;
    flash_address += FLASH_MFGREC_START_ADDRESS;

    nrf_fstorage_read( &fstorage, flash_address, &old_data, sizeof(uint32_t) );
    if ( index & 1 )
        new_data = (old_data & 0x0000FFFF) | (value << 16);
    else
        new_data = (old_data & 0xFFFF0000) | value;        

    nrf_fstorage_write( &fstorage, flash_address, &new_data, sizeof(uint32_t), NULL );
} 

void log_entry ( uint8_t size, uint32_t param4, uint32_t param5, uint32_t param6, uint32_t param7 )
{
    static uint32_t   temp_log[7];
    uint8_t           index;
    uint16_t          results_index;

    last_log_entry = log_wrptr;   // save this as the last_log_entry

    temp_log[0] = (GET_TIME << 24) | (seconds & 0x00FFFFFF);
    temp_log[1] = ble_request;
    temp_log[2] = ble_response;
    temp_log[3] = param4;
    temp_log[4] = param5;
    temp_log[5] = param6;
    temp_log[6] = param7;
    flash_log_write ( temp_log, size );

    results_index = RESULTS_SIZE-1;
    for ( index = 0; index < size; index++ )
    {
        results_vector[results_index--] = GET_BIT00_07_BYTE ( temp_log[index] );
        results_vector[results_index--] = GET_BIT08_15_BYTE ( temp_log[index] );
        results_vector[results_index--] = GET_BIT16_23_BYTE ( temp_log[index] );
        results_vector[results_index--] = GET_BIT24_31_BYTE ( temp_log[index] );
    }
}
/** @} */