/**@brief Copyright 2020 Neurostim Technologies LLC
*/
#include "global.h"
#include "app_timer.h"
#include "string.h"

extern uint32_t Timer_period;

/**@brief Initialize Patch 
 *
 * @details Execute startup procedure for Patch
 */
void Initialize_Patch ( void )
{
    // Patch Initialization
    initialize_io();						// Initialize general purpose IO, SDD1601 paragraph 5.0, 5.1
    								// code for setting i/o ports is in nrf_gpio.h
    pulse ( SPARE1 );						// Pulse SPARE1, to show the beginning of initialization

    // Debounce
    nrf_delay_ms(PWR_UP_DLY);                                   // Debounce Delay at power up, SDD-1601, SDD1601 paragraph 4

    initialize_globals();					// initialize global variables

    flash_log_init();

    m_sound_init();

    #ifdef TWI
    twi_init();
    #endif

    initialize_ble();

    flash_check();

    restart_treatment_timer(APP_TIMER_TICKS(Timer_period));     // set main timer.  Initially 50ms
    start_seconds_timer();
    restart_audio_timer( 16 );
    set_arbiter_timer ( GET_ARBITER_TIMER );
    restart_arbiter_timer();

    sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);

    ble_command = true;
    ble_request = GET_HEALTH << 24;

    led_seq_init ( LED_SEQ_START, LED_BURST, 2, 5 );    // 2 pulses @ 250ms (5 50ms interrupts)

    pulse ( SPARE1 );
}


/**@brief Initialize global variables
*/
void initialize_globals ( void )
{
    ble_command   = false;
    ble_request   = 0;
    ble_response  = 0;

    notify_flag = false;

    log_wrptr      = 0;
    last_log_entry = 0;

    seconds        = 0;

    FlexNum        = NOSA_3C;
    Min_Conn_Interval  = MIN_CONN_INTERVAL;
    Max_Conn_Interval  = MAX_CONN_INTERVAL;
    SPARE1_on_request  = false;
    SPARE1_on_response = false;
    ClkMicDivisor      = 0;
    Table_index        = 0;

    transmissions      = 0;
    errors             = 0;

    results_wrptr = 0;
    treatment_manager_init();

    led_seq_init ( LED_IDLE, LED_BURST, 0, 0);
}

/**@brief Initialization of I/O pins
	set port pins to input or output then turn them ON or OFF
*/
void initialize_io(void)
{
    //nrf_gpio_pin_clear ( EN_VBOOST1 );		// turn OFF all VBOOST pins (just in case)
    //nrf_gpio_pin_clear ( EN_VBOOST2 );		// turn OFF all VBOOST pins (just in case)
    //nrf_gpio_pin_clear ( EN_VBOOST3 );          // turn OFF all VBOOST pins (just in case)
    //nrf_gpio_pin_clear ( EN_VBOOST4 );          // turn OFF all VBOOST pins (just in case)

    nrf_gpio_pin_set ( LED );                   // turn OFF led (on LED)

    //nrf_gpio_pin_clear ( EN_PULSE_2 );		// turn OFF EN_PULSE
    nrf_gpio_pin_clear ( SPARE3 );		// turn OFF SPARE3
    nrf_gpio_pin_clear ( SPARE2 );		// turn OFF SPARE2
    nrf_gpio_pin_clear ( SPARE1 );		// turn OFF SPARE1

    nrf_gpio_cfg_input_disconnect(IO_AIN_SPARE1);
    nrf_gpio_cfg_input_disconnect(IO_AIN_SPARE2);
    //nrf_gpio_cfg_input_disconnect(EN_VBOOST1);
    //nrf_gpio_cfg_input_disconnect(EN_VBOOST2);
    //nrf_gpio_cfg_input_disconnect(EN_VBOOST3);
    //nrf_gpio_cfg_input_disconnect(EN_VBOOST4);
    nrf_gpio_cfg_output(LED);
    //nrf_gpio_cfg_input_disconnect(DEBUG2_FLEX7);
    nrf_gpio_cfg_input_disconnect(DEBUG1);
    //nrf_gpio_cfg_input_disconnect(SDA_3AXIS);
    //nrf_gpio_cfg_input_disconnect(SCL_3AXIS);
    //nrf_gpio_cfg_input_disconnect(INT_3AXIS);
    //nrf_gpio_cfg_input_disconnect(INT_9AXIS);
    //nrf_gpio_cfg_input_disconnect(SDA_9AXIS);
    //nrf_gpio_cfg_input_disconnect(SCL_9AXIS);
    nrf_gpio_cfg_input_disconnect(MIC_CLK);
    nrf_gpio_cfg_input_disconnect(MIC_DOUT);
    //nrf_gpio_cfg_input_disconnect(EXT_SPARE1);
    //nrf_gpio_cfg_input_disconnect(EXT_SPARE2);
    //nrf_gpio_cfg_input_disconnect(EN_PULSE_2);
    nrf_gpio_cfg_input_disconnect(SPARE3);
    nrf_gpio_cfg_input_disconnect(SPARE2);
    nrf_gpio_cfg_input_disconnect(SPARE1);
    nrf_gpio_cfg_input_disconnect(IO_MON_IBAT);
    nrf_gpio_cfg_input_disconnect(IO_MON_VBAT);
};
/** @} */
