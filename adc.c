/**@brief Copyright 2020 Neurostim Technologies LLC
*/
#include "global.h"

#define MON_AIN_SPARE1_CONFIG                          \
{                                                      \
    .resistor_p = NRF_SAADC_RESISTOR_DISABLED,         \
    .resistor_n = NRF_SAADC_RESISTOR_DISABLED,         \
    .gain       = NRF_SAADC_GAIN1_4,                   \
    .reference  = NRF_SAADC_REFERENCE_INTERNAL,        \
    .acq_time   = NRF_SAADC_ACQTIME_3US,               \
    .mode       = NRF_SAADC_MODE_SINGLE_ENDED,         \
    .pin_p      = MON_AIN_SPARE1,                      \
    .pin_n      = NRF_SAADC_INPUT_DISABLED             \
}

#define MON_AIN_SPARE2_CONFIG 		       \
{                                                      \
    .resistor_p = NRF_SAADC_RESISTOR_DISABLED,         \
    .resistor_n = NRF_SAADC_RESISTOR_DISABLED,         \
    .gain       = NRF_SAADC_GAIN1_4,                   \
    .reference  = NRF_SAADC_REFERENCE_INTERNAL,        \
    .acq_time   = NRF_SAADC_ACQTIME_3US,               \
    .mode       = NRF_SAADC_MODE_SINGLE_ENDED,         \
    .pin_p      = MON_AIN_SPARE2,     		       \
    .pin_n      = NRF_SAADC_INPUT_DISABLED             \
}

#define MON_IBAT_CONFIG 			       \
{                                                      \
    .resistor_p = NRF_SAADC_RESISTOR_DISABLED,         \
    .resistor_n = NRF_SAADC_RESISTOR_DISABLED,         \
    .gain       = NRF_SAADC_GAIN4,                     \
    .reference  = NRF_SAADC_REFERENCE_INTERNAL,        \
    .acq_time   = NRF_SAADC_ACQTIME_3US,               \
    .mode       = NRF_SAADC_MODE_DIFFERENTIAL,         \
    .pin_p      = MON_VBAT,  	        	       \
    .pin_n      = MON_IBAT                             \
}

#define MON_VBAT_CONFIG 			       \
{                                                      \
    .resistor_p = NRF_SAADC_RESISTOR_DISABLED,         \
    .resistor_n = NRF_SAADC_RESISTOR_DISABLED,         \
    .gain       = NRF_SAADC_GAIN1_6,                   \
    .reference  = NRF_SAADC_REFERENCE_INTERNAL,        \
    .acq_time   = NRF_SAADC_ACQTIME_3US,               \
    .mode       = NRF_SAADC_MODE_SINGLE_ENDED,         \
    .pin_p      = MON_VBAT,          		       \
    .pin_n      = NRF_SAADC_INPUT_DISABLED             \
}

static nrf_saadc_channel_config_t MON_AIN_SPARE1_config  = MON_AIN_SPARE1_CONFIG;
static nrf_saadc_channel_config_t MON_AIN_SPARE2_config  = MON_AIN_SPARE2_CONFIG;
static nrf_saadc_channel_config_t MON_IBAT_config     = MON_IBAT_CONFIG;
static nrf_saadc_channel_config_t MON_VBAT_config     = MON_VBAT_CONFIG;

#ifndef BLOCKING
extern nrf_saadc_value_t     ADCbuf[50];
#else
extern nrf_saadc_value_t     ADCbuf[64];
#endif

#ifdef BLOCKING
void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
}
#endif

/**
 * @brief Initialize adc channel passed
 */
void saadc_init( nrf_saadc_input_t channel, uint16_t samples )
{
    nrf_drv_saadc_init(NULL, saadc_callback);

    switch ( channel )
    {
        case MON_AIN_SPARE1:
            nrf_drv_saadc_channel_init(0, &MON_AIN_SPARE1_config);
            break;
        case MON_AIN_SPARE2:
            nrf_drv_saadc_channel_init(0, &MON_AIN_SPARE2_config);
            break;
        case MON_IBAT:
            nrf_drv_saadc_channel_init(0, &MON_IBAT_config);
            break;
        case MON_VBAT:
            nrf_drv_saadc_channel_init(0, &MON_VBAT_config);
            break;
    }
    #ifndef BLOCKING
    nrf_drv_saadc_buffer_convert(ADCbuf, samples);
    #endif
}

/**
 * @brief Uninitialize all adc channels
 */
void saadc_uninit( void )
{
    nrf_drv_saadc_uninit();

    nrf_gpio_cfg_input_disconnect(IO_AIN_SPARE1);
    nrf_gpio_cfg_input_disconnect(IO_AIN_SPARE2);
    nrf_gpio_cfg_input_disconnect(IO_MON_IBAT);
    nrf_gpio_cfg_input_disconnect(IO_MON_VBAT);
}

/** @} */
