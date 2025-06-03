/**@brief Copyright 2020 Neurostim Technologies LLC
*/
#include "global.h"

#include "nrf_sdh_ble.h"
#include "neurostim_ble.h"
#include "nrf_log.h"
#include "ble_gatts.h"

/**
 * @brief This code sends a indication to the client when characteristic ble_response changes
 */
uint32_t ble_resp_send( void )
    {
    // Send indication
    uint32_t                       err_code;
    static uint16_t                hvx_len;
    static ble_gatts_hvx_params_t  hvx_params;

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_len = sizeof ( ble_response );

    hvx_params.handle   = m_NEUROSTIM.NEUROSTIM_char_handles[RESPONSE_CHAR_IDX].value_handle;
    hvx_params.type     = BLE_GATT_HVX_INDICATION;
    hvx_params.offset   = 0;
    hvx_params.p_len    = &hvx_len;
    hvx_params.p_data   = NULL;

    err_code = sd_ble_gatts_hvx(m_NEUROSTIM.conn_handle, &hvx_params);

    return err_code;
    }

/**
 * @brief Set Response characteristic in local memory
 */
void set_ble_response ( uint8_t command, uint8_t param1, uint8_t param2, uint8_t param3 )
{
    ((uint8_t*)&ble_response)[0] = param3;
    ((uint8_t*)&ble_response)[1] = param2;
    ((uint8_t*)&ble_response)[2] = param1;
    ((uint8_t*)&ble_response)[3] = command;
}
/**
 * @brief Set Response characteristic in local memory and update BLE
 */
void set_Response_characteristic ()
{
    ble_gatts_value_t attr_val = {
        .len = sizeof(ble_response),
        .p_value = (uint8_t*)&ble_response
    };

    if ( SPARE1_on_response )
    {
        pulse ( SPARE1 );
    }

    sd_ble_gatts_value_set (  p_NEUROSTIM->conn_handle,
                              p_NEUROSTIM->NEUROSTIM_char_handles[RESPONSE_CHAR_IDX].value_handle, 
                              &attr_val );
    //ble_resp_send();
}

/**
 * @brief Intialize BLE for this product
 */
ret_code_t NEUROSTIM_ble_init() {
    ret_code_t err_code;

    ble_uuid_t      ble_uuid;
    ble_uuid128_t   base_uuid = UUID_NEUROSTIM_SERVICE;

    err_code = sd_ble_uuid_vs_add (&base_uuid, &m_NEUROSTIM_uuid_type);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    p_NEUROSTIM->conn_handle = BLE_CONN_HANDLE_INVALID;

    ble_uuid.uuid = UUID_NEUROSTIM_SERVICE_SHORT;
    ble_uuid.type = m_NEUROSTIM_uuid_type;
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &ble_uuid,
                                        &p_NEUROSTIM->service_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Sanity check
    APP_ERROR_CHECK_BOOL(NUM_CHAR_IDX == sizeof(g_char_data)/sizeof(g_char_data[0]));

    err_code = NEUROSTIM_char_add(p_NEUROSTIM, REQUEST_CHAR_IDX, (uint8_t *)&ble_request, sizeof(ble_request),  true, false );
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    err_code = NEUROSTIM_char_add(p_NEUROSTIM, RESPONSE_CHAR_IDX, (uint8_t *)&ble_response, sizeof(ble_response),  true, false );
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    err_code = NEUROSTIM_char_add(p_NEUROSTIM, STREAM_CHAR_IDX, ble_stream, sizeof(ble_stream),  false, true );
    return err_code;
}

/**
 * @brief Add a characteristic to BLE
 */
uint32_t NEUROSTIM_char_add(NEUROSTIM_t * p_NEUROSTIM, CharIdx_t index, uint8_t *data, uint8_t data_size, bool indication, bool notify )
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md = { {0, 0, 0, 0, 0, 0, 0}, {0, 0}, NULL, 0, 0, NULL, NULL, NULL, NULL};
    ble_gatts_attr_md_t cccd_md = { {0, 0}, {0, 0}, 0, 0, 0, 0 };
    ble_gatts_attr_t    attr_char_value = {0, 0, NULL, NULL, NULL, 0};
    ble_uuid_t          ble_uuid = { 0, 0 };
    ble_gatts_attr_md_t attr_md = { {0, 0}, {0, 0}, 0, 0, 0, 0 };

    // Add characteristic
    if (g_char_data[index].indication)
    {
        BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
        BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
        cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    }

    char_md.char_user_desc_max_size = strlen(g_char_data[index].name);
    char_md.char_user_desc_size     = strlen(g_char_data[index].name);
    char_md.p_char_user_desc        = g_char_data[index].name;
    char_md.char_props.write    = 1;
    char_md.char_props.write_wo_resp = 1;
    char_md.char_props.read     = 1;
    char_md.char_props.notify   = notify;
    char_md.char_props.indicate = indication;

    BLE_UUID_BLE_ASSIGN(ble_uuid, g_char_data[index].uuid);

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc       = BLE_GATTS_VLOC_STACK;

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = data_size;
    attr_char_value.max_len   = data_size;
    attr_char_value.p_value   = data;

    err_code = sd_ble_gatts_characteristic_add(p_NEUROSTIM->service_handle, &char_md,
                                               &attr_char_value,
                                               &p_NEUROSTIM->NEUROSTIM_char_handles[index]);
    return err_code;
}

/**@brief Function for handling the Connect event.
*
* @param[in]   p_NEUROSTIM NEUROSTIM structure.
* @param[in]   p_ble_evt   Event received from the BLE stack.
*/
static void on_connect(NEUROSTIM_t * p_NEUROSTIM, ble_evt_t * p_ble_evt)
{
    p_NEUROSTIM->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}

/**@brief Function for handling the Disconnect event.
*
* @param[in]   p_NEUROSTIM    NEUROSTIM structure.
* @param[in]   p_ble_evt      Event received from the BLE stack.
*/
static void on_disconnect(NEUROSTIM_t * p_NEUROSTIM, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_NEUROSTIM->conn_handle = BLE_CONN_HANDLE_INVALID;
}

/**
 * @brief This code is called when BLE writes a characteristic.  Process data
 */
static void on_write(NEUROSTIM_t * p_NEUROSTIM, ble_evt_t * p_ble_evt)
{
    ret_code_t err_code;
    CharIdx_t idx;

    ble_gatts_evt_write_t *p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    NRF_LOG_INFO ("on_write %x %x %x %x", p_evt_write->handle, p_evt_write->op, p_evt_write->len, p_evt_write->data[idx]);

    if ( (p_evt_write->handle == p_NEUROSTIM->NEUROSTIM_char_handles[STREAM_CHAR_IDX].cccd_handle) &&
         (p_evt_write->len == 2) )
    {
        bool notif_enabled;

        notif_enabled = ble_srv_is_notification_enabled(p_evt_write->data);

        if (p_NEUROSTIM->is_stream_notif_enabled != notif_enabled)
        {
            p_NEUROSTIM->is_stream_notif_enabled = notif_enabled;
            notify_flag = notif_enabled;

//            if (p_NEUROSTIM->evt_handler != NULL)
//            {
//                p_NEUROSTIM->evt_handler(p_NEUROSTIM, BLE_TSS_EVT_NOTIF_MIC, p_evt_write->data, p_evt_write->len);
//            }
        }
    }

    if ( (p_evt_write->op == BLE_GATTS_OP_WRITE_REQ) || (p_evt_write->op == BLE_GATTS_OP_WRITE_CMD) )
    {
        for ( idx = REQUEST_CHAR_IDX; idx < NUM_CHAR_IDX; idx++ ) {
          if ( (p_evt_write->handle == p_NEUROSTIM->NEUROSTIM_char_handles[idx].value_handle) && (p_evt_write->len == 4) )
          {
              APP_ERROR_CHECK_BOOL(p_ble_evt->evt.gatts_evt.params.write.len >= sizeof(uint32_t));

              if ( SPARE1_on_request == true )
              {
                  pulse ( SPARE1 );
              }

              switch ( idx )
              {
                  case REQUEST_CHAR_IDX:
                      ble_request = *((uint32_t *)(p_ble_evt->evt.gatts_evt.params.write.data));
                      ble_command = true;
                      restart_treatment_timer(16);    // set timer to approximately 500us
                      break;
                  case RESPONSE_CHAR_IDX:
                      //Response = *((uint32_t *)(p_ble_evt->evt.gatts_evt.params.write.data));
                      break;
              }
          }
       }
    }
}

/**
 * @brief This code is called when a BLE event occurs.  Process accordingly
 */
void NEUROSTIM_on_ble_evt(ble_evt_t * p_ble_evt, void * p_thr)
{
    NEUROSTIM_t * p_NEUROSTIM = (NEUROSTIM_t *)p_thr;
    if (p_NEUROSTIM == NULL || p_ble_evt == NULL)
    {
        return;
    }

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_NEUROSTIM, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_NEUROSTIM, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
            on_write(p_NEUROSTIM, p_ble_evt);
            break;

        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
            break;

        case BLE_EVT_USER_MEM_REQUEST:
            break;

        case BLE_EVT_USER_MEM_RELEASE:
            break;

        default:
            // No implementation needed.
            break;
    }
}

uint32_t transmit_stream ()
{
    uint32_t err_code = NRF_SUCCESS;

    ble_gatts_hvx_params_t hvx_params;
    uint16_t size = 132;

    //toggle_spare ( SPARE3 );

    if (m_NEUROSTIM.conn_handle == BLE_CONN_HANDLE_INVALID)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = m_NEUROSTIM.NEUROSTIM_char_handles[STREAM_CHAR_IDX].value_handle;
    hvx_params.p_data = ble_stream;
    hvx_params.p_len  = &size;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;

    #ifdef TEST_DEBUG
    if ( !test_flag )
    {
        test_flag = true;
    #endif
        transmissions++;
        if ( m_NEUROSTIM.is_stream_notif_enabled )
            err_code = sd_ble_gatts_hvx(m_NEUROSTIM.conn_handle, &hvx_params);
    #ifdef TEST_DEBUG
    }
    #endif

    if ( err_code )
        errors++;

    return err_code;
}
/** @} */