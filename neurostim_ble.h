/**@brief Copyright 2019 NeurostimOAB, Inc
*/
#include <stdint.h>
#include "ble_srv_common.h"
#include <ble_advertising.h>

#define APP_BLE_OBSERVER_PRIO           3 

#define UUID_NEUROSTIM_SERVICE {0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}   // 128-bit base UUID 

//***the order of UUIDs is depended upon and must match the order of Char_Idx_t
#define UUID_NEUROSTIM_SERVICE_SHORT  0x1523
#define UUID_NEUROSTIM_REQUEST        0x1525
#define UUID_NEUROSTIM_RESPONSE       0x1526
#define UUID_NEUROSTIM_STREAM         0x1527

typedef enum {
    REQUEST_CHAR_IDX = 0,
    RESPONSE_CHAR_IDX,
    STREAM_CHAR_IDX,
    NUM_CHAR_IDX
} CharIdx_t;

typedef struct NEUROSTIM_s NEUROSTIM_t;

struct NEUROSTIM_s
{
    uint16_t                  service_handle;             /**< Handle of NEUROSTIM (as provided by the BLE stack). */
    ble_gatts_char_handles_t  NEUROSTIM_char_handles[NUM_CHAR_IDX];
    uint16_t                  conn_handle;                /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
    bool                      is_stream_notif_enabled;    /**< Variable to indicate if the peer has enabled notification of the characteristic.*/
};

typedef struct NEUROSTIM_init_s NEUROSTIM_init_t;

struct NEUROSTIM_init_s
{
    bool                   support_notification;           //< TRUE if notification of STATE measurement is supported.
};

typedef struct 
{
    const char* name;
//    uint32_t min_value;
//    uint32_t max_value;
//    CharIdx_t idx;
    uint16_t uuid;
//    uint8_t data_format;
    bool     indication;
} CharData_t;

// NOTE!!!! must be in SAME order of UUIDs :(
static const CharData_t g_char_data[] = {
    //{"Request",   0,      0xFFFFFFFF, REQUEST_CHAR_IDX,    UUID_NEUROSTIM_REQUEST,  BLE_GATT_CPF_FORMAT_UINT32, false},
    //{"Response" , 0,      0xFFFFFFFF, RESPONSE_CHAR_IDX,   UUID_NEUROSTIM_RESPONSE, BLE_GATT_CPF_FORMAT_UINT32, true},
    {"Request",   UUID_NEUROSTIM_REQUEST,  false},
    {"Response" , UUID_NEUROSTIM_RESPONSE, true},
    {"Stream" ,   UUID_NEUROSTIM_STREAM,   false},
};

NEUROSTIM_t m_NEUROSTIM;
NEUROSTIM_init_t m_NEUROSTIM_init;
NEUROSTIM_t *p_NEUROSTIM = &m_NEUROSTIM;
NEUROSTIM_init_t *p_NEUROSTIM_init = &m_NEUROSTIM_init;

uint8_t   m_NEUROSTIM_uuid_type = BLE_UUID_TYPE_VENDOR_BEGIN;

void NEUROSTIM_on_ble_evt(ble_evt_t *, void *);
uint32_t NEUROSTIM_char_add(NEUROSTIM_t *, uint8_t, uint8_t *, uint8_t, bool, bool );

NRF_SDH_BLE_OBSERVER(m_macp_obs, APP_BLE_OBSERVER_PRIO, (nrf_sdh_ble_evt_handler_t)NEUROSTIM_on_ble_evt, &m_NEUROSTIM);

/** @} */