/**@brief Copyright 2020 Neurostim Technologies LLC
*/
#include "global.h"
#include "app_timer.h"
#include "ble_gap.h"
#include "string.h"

extern STRENGTH_TABLE_ENTRY Strength_table[][MAX_STRENGTH+1];
extern bool                 Battle_Override;
extern bool                 stop_treat;
extern bool                 get_health;
extern uint8_t              Strength;
extern nrf_saadc_value_t    vbat;
extern uint8_t              PWMmax;
extern uint8_t              Short_Ctr;
extern uint16_t             Abort_Ctr;
extern uint8_t              rssi;

extern bool                 treat_active;
extern uint8_t              PWM_limit;
extern uint16_t             Duration;
extern uint8_t              Frequency;
extern uint32_t             Timer_period;
extern uint16_t             TestThreshold;
extern uint16_t             ADCTime;
extern uint16_t             SampleHoldoff;
extern uint8_t              NumSamples;

extern bool                 Enable_Adaptive;

extern enum LED_FSM_STATE   led_fsm;
extern uint16_t             m_conn_handle;
extern uint8_t              LastStrength;
#ifndef BLOCKING
extern const nrf_drv_timer_t      saadc_timer;
#endif
APP_TIMER_DEF(treatment_timer); 
APP_TIMER_DEF(seconds_timer); 
APP_TIMER_DEF(audio_timer);

/**
 * @brief Initialize main timer for software interrupts
 */
void init_treatment_timer()
{
    uint32_t err_code;

    err_code = app_timer_create(&treatment_timer, APP_TIMER_MODE_REPEATED, treatment_timer_handler);
    APP_ERROR_CHECK(err_code);
}

/**
 * @brief Re-start main timer for software interrupts
 */
void restart_treatment_timer( uint16_t timer )
{
    stop_treatment_timer();
    enable_outputs( true );
    app_timer_start(treatment_timer,timer,NULL);
}

/**
 * @brief Stop main timer for software interrupts
 */
void stop_treatment_timer()
{
    enable_outputs( false );
    app_timer_stop(treatment_timer);
}

/**
 * @brief Initialize seconds timer for software interrupts
 */
void init_seconds_timer()
{
    uint32_t err_code;

    err_code = app_timer_create(&seconds_timer, APP_TIMER_MODE_REPEATED, seconds_timer_handler);
    APP_ERROR_CHECK(err_code);
}

/**
 * @brief Start seconds timer
 */
void start_seconds_timer()
{
    app_timer_start(seconds_timer,APP_TIMER_TICKS(125),NULL);
}

/**
 * @brief Initialize audio timer for software interrupts
 */
void init_audio_timer()
{
    uint32_t err_code;

    err_code = app_timer_create(&audio_timer, APP_TIMER_MODE_REPEATED, audio_timer_handler);
    APP_ERROR_CHECK(err_code);
}

/**
 * @brief Re-start audio timer for software interrupts
 */
void restart_audio_timer( uint16_t timer )
{
    stop_audio_timer();
    app_timer_start(audio_timer,APP_TIMER_TICKS(timer),NULL);
}

/**
 * @brief Stop audio timer for software interrupts
 */
void stop_audio_timer()
{
    app_timer_stop(audio_timer);
}


/**@brief Initialization of I/O pins
	set port pins to input or output then turn them ON or OFF
*/
void enable_outputs( bool flag )
{
    if ( flag ) 
    {                         // set pins as outputs
      nrf_gpio_cfg_output(LED);

//      nrf_gpio_cfg_output(SPARE1);
    }
    else 
    {                              // disconnect pins
      nrf_gpio_cfg_input_disconnect(LED);

//      nrf_gpio_cfg_input_disconnect(SPARE1);
    }
}

void set_table_index()
{
    switch ( Frequency )
    {
        case 5:
            Table_index = 0;
            break;
        case 10:
            Table_index = 1;
            break;
        case 20:
            Table_index = 2;
            break;
        case 30:
            Table_index = 3;
            break;
        case 50:
            Table_index = 4;
            break;
        case 100:
            Table_index = 5;
            break;
        default:
            Table_index = 2;
            break;
    }
}

/**
 * @brief Process the request that was written by client to Request characteristic
 */
void ble_parser ()
{
    enum COMMAND command;
    uint8_t param1, response_param1;
    uint8_t param2, response_param2;
    uint8_t param3, response_param3;
    uint32_t idle_response;
    double dVBAT;
    uint8_t uVBAT;
    bool    write_flag;
    uint8_t variable_class;
    uint8_t variable;
    uint16_t data;
    uint8_t size;
    uint8_t index;
    uint8_t field_index;

    ble_gap_conn_params_t   gap_conn_params;
    ret_code_t err_code;

    param3  = GET_BIT00_07_BYTE ( ble_request );
    param2  = GET_BIT08_15_BYTE ( ble_request );
    param1  = GET_BIT16_23_BYTE ( ble_request );
    command = GET_BIT24_31_BYTE ( ble_request );
    switch ( command )
    {
        case IDLE_MODE:   // IDLE_MODE
            idle_response = 0;
            idle_response  = (((rssi - 90) << 1)  & 0x00003E); // put RSSI in bits 1 to 5
            idle_response |= ((Short_Ctr << 6)    & 0x0007C0); // put Short pulses in bits 6 to 10
            idle_response |= ((PWMmax << 11)      & 0x00F800); // put PWMmax in bits 15 to 11
            dVBAT = (1.8 + (.1 * (double)vbat));
            uVBAT = (uint8_t)dVBAT;
            idle_response |= ((uVBAT << 16)       & 0x0F0000); // put VBAT in bits 19 to 16
            set_ble_response ( IDLE_MODE, GET_BIT16_23_BYTE(idle_response), GET_BIT08_15_BYTE(idle_response), GET_BIT00_07_BYTE(idle_response) );
            set_Response_characteristic ();
            break;
        case STOP_TREAT:  // STOP_TREATMENT
            stop_treat = true;
            break;
        case START_TREAT: // START_TREATMENT
            start_treatment ( param1, param2, param3 );
            results_wrptr = 0;
            break;
        case SET_VARIABLE: // SET_VARIABLE
            write_flag     = (param1 & 0x80) ? true : false;
            variable_class = (param1 & 0x70) >> 4;
            variable       = (param1 & 0x0F);

            //
            // first, get the existing data out of the variable, save in response_param2, response_param3
            //
            response_param1 = param1;
            response_param2 = 0;
            response_param3 = 0;
            switch ( variable_class )
            {
              case 1:   // Treatment

                  switch ( variable )
                  {
                      case 0:   // Strength
                          response_param3 = Strength;
                          break;
                      case 1:   // Duration
                          response_param2 = Duration >> 8;
                          response_param3 = Duration & 0xFF;
                          break;
                      case 2:   // Num Samples
                          response_param3 = NumSamples;
                          break;
                      case 3:   // Frequency
                          response_param3 = Frequency;
                          break;
                      case 4:   // PWM_limit
                          response_param3 = PWM_limit;
                          break;
                      case 5:   // Flags
                          response_param3 |= Enable_Adaptive ? 0x40 : 0x00;
                          response_param3 |= Battle_Override ? 0x80 : 0x00;
                          break;
                      case 6:   // TestThreshold
                          response_param2 = TestThreshold >> 8;
                          response_param3 = TestThreshold & 0xFF;
                          break;
                      case 7:   // ADCTime
                          response_param2 = ADCTime >> 8;
                          response_param3 = ADCTime & 0xFF;
                          break;
                      case 8:   // SampleHoldoff
                          response_param2 = SampleHoldoff >> 8;
                          response_param3 = SampleHoldoff & 0xFF;
                          break;
                      default:
                          break;
                  }
                  break;
              case 2:   // strength table
                  switch ( variable )
                  {
                      case 0:
                          response_param2 = Strength_table[Table_index][Strength].Threshold / 256;
                          response_param3 = Strength_table[Table_index][Strength].Threshold % 256;
                          break;
                      case 1:
                          response_param3 = Strength_table[Table_index][Strength].Step_End;
                          break;
                      case 2:
                          response_param3 = Strength_table[Table_index][Strength].PWMstart;
                          break;
                      case 3:
                          response_param3 = Strength_table[Table_index][Strength].VBATheadroom;
                          break;
                      default:
                          break;
                  }
                  break;
              case 3:   // status
                  switch ( variable )
                  {
                      case 0:
                          response_param2 = Abort_Ctr / 256;
                          response_param3 = Abort_Ctr % 256;
                          break;
                      case 1:
                          response_param3 = LastStrength;
                          break;
                      case 3:
                          response_param3 = FlexNum; // FlexNum
                          break;
                      default:
                          break;
                  }
                  break;
              case 4:
                  switch ( variable )
                  {
                      case 0:       // MIN_CONN_INTERVAL
                          response_param2 = Min_Conn_Interval / 256;
                          response_param3 = Min_Conn_Interval % 256;
                          break;
                      case 1:       // MAX_CONN_INTERVAL
                          response_param2 = Max_Conn_Interval / 256;
                          response_param3 = Max_Conn_Interval % 256;
                          break;
                  }
                  break;
              case 5:
                  switch ( variable )
                  {
                      case 0:       // Pulse SPARE1 on Request
                          response_param3 = SPARE1_on_request;
                          break;
                      case 1:       // Pulse SPARE1 on Response
                          response_param3 = SPARE1_on_response;
                          break;
                      case 2:       // ClkMicDivisor
                          response_param3 = ClkMicDivisor;
                          break;
                  }
                  break;
              default:  // 0 - illegal, 4 - F reserved
                  break;
            }
                      
            //
            // After we read existing data, if we are writing, then update variables
            //
            if ( write_flag )   // if writing
            {
                switch ( variable_class )
                {
                  case 1:   // Treatment
                      switch ( variable )
                      {
                          case 0:   // Strength
                              Strength = param3;
                              break;
                          case 1:   // Duration
                              Duration = (param2 << 8) | param3;
                              break;
                          case 2:   // Num Samples
                              NumSamples = param3;
                              break;
                          case 3:   // Frequency
                              Frequency = param3;
                              Timer_period = 1000/Frequency;
                              set_table_index();
                              restart_treatment_timer( APP_TIMER_TICKS(Timer_period) );
                              break;
                          case 4:   // PWM_limit
                              PWM_limit = param3;
                              break;
                          case 5:   // Flags
                              Enable_Adaptive = ( param3 & 0x40 ) ? true : false;
                              Battle_Override = ( param3 & 0x80 ) ? true : false;
                              break;
                          case 6:   // TestThreshold
                              TestThreshold = (param2 << 8) | param3;
                              break;
                          case 7:   // ADCTime
                              ADCTime = (param2 << 8) | param3;
                              #ifndef BLOCKING
                              nrf_drv_timer_disable(&saadc_timer);
                              /* setup saadc_timer for compare event every ADCtime */
                              nrf_drv_timer_extended_compare(&saadc_timer,
                                                             NRF_TIMER_CC_CHANNEL0,
                                                             ADCTime,
                                                             NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK,
                                                             false);
                              nrf_drv_timer_enable(&saadc_timer);
                              #endif
                              break;
                          case 8:   // SampleHoldoff
                              SampleHoldoff = (param2 << 8) | param3;
                              break;
                          default:
                              break;
                      }
                      break;
                  case 2:   // strength table
                      switch ( variable )
                      {
                          case 0:
                              Strength_table[Table_index][Strength].Threshold = ((param2*256)+param3);
                              break;
                          case 1:
                              Strength_table[Table_index][Strength].Step_End = param3;
                              break;
                          case 2:
                              Strength_table[Table_index][Strength].PWMstart = param3;
                              break;
                          case 3:
                              Strength_table[Table_index][Strength].VBATheadroom = param3;
                              break;
                          default:
                              break;
                      };
                      break;
                  case 3:   // status
                      switch ( variable )
                      {
                          case 3:
                              FlexNum = response_param3;    // FlexNum
                              break;
                          default:
                              break;
                      }
                      break;
                  case 4:
                      switch ( variable )
                      {
                          case 0:       // MIN_CONN_INTERVAL
                              Min_Conn_Interval = (param2 * 256) + param3;
                              memset(&gap_conn_params, 0, sizeof(gap_conn_params));
                              gap_conn_params.min_conn_interval = Min_Conn_Interval;
                              gap_conn_params.max_conn_interval = Max_Conn_Interval;
                              gap_conn_params.slave_latency     = SLAVE_LATENCY;
                              gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;
                              err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
                              if (err_code == NRF_SUCCESS)
                              {
                                  err_code = sd_ble_gap_conn_param_update( m_conn_handle, &gap_conn_params );
                              }
                              break;
                          case 1:       // MAX_CONN_INTERVAL
                              Max_Conn_Interval = (param2 * 256) + param3;
                              memset(&gap_conn_params, 0, sizeof(gap_conn_params));
                              gap_conn_params.min_conn_interval = Min_Conn_Interval;
                              gap_conn_params.max_conn_interval = Max_Conn_Interval;
                              gap_conn_params.slave_latency     = SLAVE_LATENCY;
                              gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;
                              err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
                              if (err_code == NRF_SUCCESS)
                              {
                                  err_code = sd_ble_gap_conn_param_update( m_conn_handle, &gap_conn_params );
                              }
                              break;
                      }
                      break;
                  case 5:
                      switch ( variable )
                      {
                          case 0:       // Pulse SPARE1 on Request
                              SPARE1_on_request = (bool)param3;
                              break;
                          case 1:       // Pulse SPARE1 on Response
                              SPARE1_on_response = param3;
                              break;
                          case 2:       // ClkMicDivisor
                              ClkMicDivisor = param3;
                              break;
                      }
                      break;
                  default:  // 0 - illegal, 4 - F reserved
                      break;
                }
            }
  
            set_ble_response ( SET_VARIABLE, response_param1, response_param2, response_param3 );
            set_Response_characteristic ();
            log_entry( 3, 0, 0, 0, 0 );
            break;
        case GET_FWVERSION: // GET_FWVERSION
            set_ble_response ( GET_FWVERSION, SYSTEM_VERSION, FIRMWARE_VERSION_MSB, FIRMWARE_VERSION_LSB );
            set_Response_characteristic ();
            log_entry( 3, 0, 0, 0, 0 );
            break;
        case GET_HEALTH:  // GET_HEALTH
            // param1 : Control byte - FUTURE OPTION
            get_health = true;
            break;
        case GET_TIME:    // GET_TIME
            set_ble_response ( GET_TIME, GET_BIT16_23_BYTE(seconds), GET_BIT08_15_BYTE(seconds), GET_BIT00_07_BYTE(seconds) );
            set_Response_characteristic ();
            break;
        case GET_RESULT:  // GET_RESULT
            data = (param2*256) + param3;
            switch ( param1 )
            {
                case 0x1E:
                    param1 = results_vector[data+0];
                    param2 = results_vector[data+1];
                    param3 = results_vector[data+2];
                    set_ble_response ( GET_RESULT, param1, param2, param3 );
                    set_Response_characteristic ();
                    break;
                case 0x2D:
                    // Start Streaming by saving pointer and number of packets
                    SCR[RESULTS_VECTOR][3] = GET_RESULT_VECTOR_POINTER;               // save beginning pointer into results vector
                    SCR[RESULTS_VECTOR][4] = data / STREAM_DATA_SIZE;                 // save number of packets to transmit
                    if ( data % STREAM_DATA_SIZE ) SCR[RESULTS_VECTOR][4]++;          // include any partial data
                    channel_dirty[RESULTS_VECTOR] = true;
                    break;
                case 0x3C:
                    // Test Pattern
                    generate_pattern( data, RESULTS_SIZE, results_vector, 1 );
                    break;
            }
            break;
        case GET_LOG:     // GET_LOG
            if ( param3 > 0 )          // if param3 set, then erase and we are done
            {
                flash_log_erase();
                set_ble_response ( GET_LOG, 0, 0, 0 );
            }
            else                       // else param3 is zero
            {
                if ( param2 > 0 )      // if param2 is set, then return log length
                {
                    set_ble_response ( GET_LOG, 0, GET_BIT08_15_BYTE(log_wrptr), GET_BIT00_07_BYTE(log_wrptr) );
                }
                else                   // else param2 is zero
                {
                    if ( param1 > 0 )  // if param1 is set, then get last entry - deleted by Mike?
                    {
                        // put last entry in results log
                        flash_log_buffer_read ( (uint32_t *)results_vector, last_log_entry, (log_wrptr - last_log_entry) );
                        set_ble_response ( GET_LOG, 0, GET_BIT08_15_BYTE(log_wrptr), GET_BIT00_07_BYTE(log_wrptr) );
                    }
                    else            // if param1 is zero (therefore all params zero), copy entire log to results vector
                    {
                        flash_log_buffer_read ( (uint32_t *)results_vector, 0, log_wrptr );
                        //results_rdptr = 0;
                        results_wrptr = log_wrptr * sizeof(uint32_t);
                        set_ble_response ( GET_LOG, 0, GET_BIT08_15_BYTE(log_wrptr), GET_BIT00_07_BYTE(log_wrptr) );
                    }
                }
            }
            set_Response_characteristic ();
            break;
        case SET_MFGREC:
            // param1 : HWord Offset
            // param2 : 16 bit Halfword Write Data MSB
            // param3 : 16 bit Halfword Write Data LSB
            flash_mfg_write ( param1, ((param2*256)+param3) );
            set_ble_response ( SET_MFGREC, param1, param2, param3 );
            set_Response_characteristic ();
            break;
        case GET_MFGREC:
            // param1 : HWord Offset
            // param2 : Length
            // param3 : Erase
            if ( param2 > 0 )         // if there is a length
            {                         // transfer mfg record to results_vector
                //results_rdptr = 0;
                results_wrptr = 0;
                index = param1;
                size = param2;
                while ( size-- )
                {
                    data = flash_mfg_read ( index++ );
                    results_vector[results_wrptr++] = GET_BIT08_15_BYTE( data );
                    results_vector[results_wrptr++] = GET_BIT00_07_BYTE( data );
                }
            }
            set_ble_response ( GET_MFGREC, param1, param2, param3 );
            set_Response_characteristic ();
            break;
        case SET_STREAMCTL:
            index       = param1 >> 4;
            field_index = param1 &  0x0F;
            SCR[index][field_index] = (param2*256)+param3;
            if ( (index == STREAM_STATUS) && (field_index == 2) )
                restart_arbiter_timer();
            set_ble_response ( SET_STREAMCTL, param1, param2, param3 );
            set_Response_characteristic ();
            if ( GET_CHANNEL_ACTIVE(LEFT_AUDIO) || GET_CHANNEL_ACTIVE(RIGHT_AUDIO) )
                drv_mic_start();
            else
                drv_mic_stop();
            if ( GET_CHANNEL_ACTIVE(THREE_AXIS_ACC) )
            {
                accel_init();
                NRF_LOG_INFO("ACC START");
            }
            else
            {
                accel_deinit();
                NRF_LOG_INFO("ACC STOP");
            }
            if ( GET_CHANNEL_ACTIVE(SPO2_SENSOR) )
            {
                spo2_init();
                NRF_LOG_INFO("SPO2 START");
            }
            else
            {
                spo2_deinit();
                NRF_LOG_INFO("SPO2 STOP");
            }
            break;
        case GET_STREAMCTL:
            index       = param1 >> 4;
            field_index = param1 &  0x0F;
            param2 = SCR[index][field_index] >> 8;
            param3 = SCR[index][field_index] & 0xFF;
            set_ble_response ( GET_STREAMCTL, param1, param2, param3 );
            set_Response_characteristic ();
            break;
        default:
            break;
    }
}

/**@brief Main timer handler.  Called every 50ms (current default)
 */
void treatment_timer_handler(void * p_context)
{
    pulse ( SPARE2 );		// pulse SPARE2 pin

    led_handler();		// Call led handler

    if ( ble_command )
      {
          ble_command = false;
          
          restart_treatment_timer(APP_TIMER_TICKS(Timer_period));
          ble_parser();
      }

    treatment_manager();

    if ( !treat_active )
       if ( led_fsm == LED_IDLE )
          stop_treatment_timer();     // disconnect outputs, disable timer, wait for BLE wakeup
}

/**@brief seconds timer handler.  Called every second
 */
void seconds_timer_handler(void * p_context)
{
    static uint8_t eighths;

    three_axis_acc_data[0] = (seconds >>  0) & 0x0FF;
    three_axis_acc_data[1] = (seconds >>  8) & 0x0FF;
    three_axis_acc_data[2] = (seconds >> 16) & 0x0FF;
    three_axis_acc_data[3] = (seconds >> 24) & 0x0FF;

    spo2_sensor_data[0] = (seconds >>  0) & 0x0FF;;
    spo2_sensor_data[1] = (seconds >>  8) & 0x0FF;
    spo2_sensor_data[2] = (seconds >> 16) & 0x0FF;
    spo2_sensor_data[3] = (seconds >> 24) & 0x0FF;

    eighths++;
    switch ( eighths % 8 )
    {
      case 0:                                     // once per second (on eights count of 0)
        seconds++;                                // increment seconds
        if ( GET_CHANNEL_ACTIVE(THREE_AXIS_ACC) )
        {
          memset ( three_axis_acc_data, 0 , sizeof(three_axis_acc_data) );
          readAccel();                            // read accelerometer
          channel_dirty [THREE_AXIS_ACC]  = true; // trigger accelerometer to transmit
        }
        break;
      case 1:                                     // every quarter second
      case 3:
      case 5:
      case 7:
        if ( GET_CHANNEL_ACTIVE(SPO2_SENSOR)  ) 
        {
          toggle_spare( SPARE1 );
          //memset ( spo2_sensor_data, 0 , sizeof(spo2_sensor_data) );
          readBPM();                              // read SPO2 sensor
        }
        break;
      case 4:                                     // once per second (on eighths count of 4 - the 1/2 second) 
        if ( GET_CHANNEL_ACTIVE(SPO2_SENSOR)  )                              
          channel_dirty [SPO2_SENSOR]  = true;    // trigger SPO2 to transmit
        break;
      default:
        break;
      }
}

/**@brief audio timer handler.
 */
void audio_timer_handler(void * p_context)
{
    if ( GET_CHANNEL_ACTIVE(LEFT_AUDIO) )
        m_left_audio_process();
    if ( GET_CHANNEL_ACTIVE(RIGHT_AUDIO) )
        m_right_audio_process();
}
/** @} */
