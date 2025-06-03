/**
 * Copyright (c) 2014 - 2018, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/** @file
 * @defgroup nrf_adc_example main.c
 * @{
 * @ingroup nrf_adc_example
 * @brief ADC Example Application main file.
 *
 * This file contains the source code for a sample application using ADC.
 *
 * @image html example_board_setup_a.jpg "Use board setup A for this example."
 */
#include "global.h"
#include "nrfx_saadc.h"

#ifdef BLOCKING
nrf_saadc_value_t            ADCbuf[64];
extern uint16_t              ADCTime;
extern uint16_t              SampleHoldoff;
#else
const nrf_drv_timer_t saadc_timer = NRF_DRV_TIMER_INSTANCE(1);
nrf_saadc_value_t            ADCbuf[50];
static nrf_ppi_channel_t     m_ppi_channel;
static uint32_t              m_adc_evt_counter;
extern uint16_t              ADCTime;
extern uint16_t              SampleHoldoff;

void timer1_handler(nrf_timer_event_t event_type, void * p_context)
{
}

void saadc_sampling_event_init ( void )
{
    ret_code_t err_code;

    err_code = nrf_drv_ppi_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32;
    err_code = nrf_drv_timer_init(&saadc_timer, &timer_cfg, timer1_handler);
    APP_ERROR_CHECK(err_code);

    /* setup saadc_timer for compare event every ADCtime */
    nrf_drv_timer_extended_compare(&saadc_timer,
                                   NRF_TIMER_CC_CHANNEL0,
                                   ADCTime,
                                   NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK,
                                   false);
    nrf_drv_timer_enable(&saadc_timer);

    uint32_t timer_compare_event_addr = nrf_drv_timer_compare_event_address_get(&saadc_timer,
                                                                                NRF_TIMER_CC_CHANNEL0);
    uint32_t saadc_sample_task_addr   = nrf_drv_saadc_sample_task_get();

    /* setup ppi channel so that timer compare event is triggering sample task in SAADC */
    err_code = nrf_drv_ppi_channel_alloc(&m_ppi_channel);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_ppi_channel_assign(m_ppi_channel,
                                          timer_compare_event_addr,
                                          saadc_sample_task_addr);
    APP_ERROR_CHECK(err_code);
}

void saadc_sampling_event_enable(void)
{
    ret_code_t err_code = nrf_drv_ppi_channel_enable(m_ppi_channel);

    APP_ERROR_CHECK(err_code);
}
#endif


static const nrf_drv_timer_t m_timer2 = NRF_DRV_TIMER_INSTANCE(2);

void timer2_handler(nrf_timer_event_t event_type, void* p_context)
{
}

void timer2_init()
{
    ret_code_t err_code;

    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    err_code = nrf_drv_timer_init(&m_timer2, &timer_cfg, timer2_handler);
    APP_ERROR_CHECK(err_code);
}

void timer2_start()
{
    nrf_drv_timer_enable(&m_timer2);
}

uint32_t timer2_stop()
{
    uint32_t timer_value;

    timer_value = nrf_drv_timer_capture(&m_timer2, 0);
    nrf_drv_timer_disable(&m_timer2);

    return ( timer_value );
}

extern STRENGTH_TABLE_ENTRY Strength_table[6][MAX_STRENGTH+1];

extern bool              start_treat;    // BLE Semaphore to Start a Treatment
extern bool              stop_treat;     // BLE Semaphore to Stop  a Treatment
extern bool              get_health;     // BLE Semaphore used to return health of system

extern uint8_t           Strength;       // Pulse amplitude, 5% increments, enforce range 0..20, default (20%)
extern uint8_t           LastStrength;
extern uint8_t           Slope;          // The number of ADC cycles between PWM Ramp Steps.
extern uint16_t          Threshold;
extern bool              Adapt;
extern bool              RefreshThreshold;
extern bool              HVrampStart;
extern uint16_t          ThresholdSave;


extern nrf_saadc_value_t vboost;         // Last reading of VBOOST ADC
extern nrf_saadc_value_t vbat;           // Last reading of VBAT ADC
extern nrf_saadc_value_t ibat;           // Last reading of IBAT ADC
extern nrf_saadc_value_t voltage;        // Last reading of VOLTAGE ADC
extern uint8_t           PWMmax;         // maximum PWM reading at end of treatment
extern uint8_t           Short_Ctr;      // Number of pulses issued at lower voltage than desired
extern uint16_t          Abort_Ctr;      // Number of failures due to low voltage (headroom)
extern uint32_t          temperature;    // last temperature reading
extern int8_t            rssi;           // strength of BLE received signal
extern nrf_saadc_value_t VbatMin;
extern nrf_saadc_value_t IbatMax;
extern nrf_saadc_value_t VBOOSTMax;


extern bool              treat_active;   // Global Semaphore saying Treatment is active
extern uint8_t           PWM_value;      // Current PWM level
extern uint8_t           PWM_limit;      // Maximum PWM level
extern enum HV_MODE      adc_state;

extern uint16_t          Duration;       // default 10 seconds, probably not played with.
extern uint8_t           Frequency;      // default 20 Hz, major issues if changed
extern uint32_t          Timer_period;
extern bool              Battle_Override;
extern uint16_t          TestThreshold;
extern uint8_t           NumSamples;

extern bool              Enable_Adaptive;

extern uint16_t          Pulse_Ctr;      // Pulse counter - number of pulses in the treatment
extern uint16_t          Num_Pulses;     // Number of pulses to issue
extern uint8_t           Step_Ctr;       // Step counter - Counter for ramping up HVRAMP voltage
extern uint8_t           Step_End;       // Step end value - Target count for Step_Ctr

extern uint16_t          LoadR;
extern uint16_t          TargetDeliveredI;
extern uint16_t          DeliveredI;
extern uint32_t          DeliveredIsum;

extern uint16_t          PulseVmax;
extern uint16_t          PulseVsum;

extern uint16_t          DeliveredIave;
extern int16_t           PulseDecay;

uint16_t                 PulseWidth;

static uint8_t           samples;

#ifndef BLOCKING
static bool sampling_init = false;
void saadc_timer_init ()
{
    if ( !sampling_init )
    {
        saadc_sampling_event_init();
        saadc_sampling_event_enable();
        sampling_init= true;
    }

}
#endif
void start_saadc( nrf_saadc_input_t channel, uint8_t num_samples, bool initialize )
{
    #ifndef BLOCKING
    ret_code_t    err_code;

    if ( initialize )
    {
        samples = num_samples;
        saadc_uninit();
        saadc_init( channel, num_samples );
        saadc_timer_init();
    }

    err_code = nrfx_saadc_sample();
    APP_ERROR_CHECK(err_code);
    #else
    saadc_uninit();
    saadc_init( channel, 1 );
    #endif
}
#ifndef BLOCKING
void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
    if (p_event->type == NRFX_SAADC_EVT_DONE )
    {
#else
bool blocking_saadc_callback()
{
    {
#endif
        ret_code_t err_code;
        uint16_t    value;
        uint16_t    delay_loop;

        #ifndef BLOCKING
        err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, samples);
        APP_ERROR_CHECK(err_code);
        #endif

        uint8_t count;

        switch ( adc_state )
            {
            case SAMPLE_BEFORE:
                #ifdef BLOCKING
                //nrfx_saadc_sample_convert ( MON_VBAT, &ADCbuf[0] ); 
                #endif
                vbat = ADCbuf[0];
                save_results ( vbat, 2 );

                if ( vbat <  Strength_table[Table_index][Strength].VBATheadroom )    // if the battery voltage is too low
                {
                    Abort_Ctr++;
                    treat_active = false;
                    disable_PULSE_EN();
                    PulseWidth = timer2_stop();
                    //pwm_stop();
                    saadc_uninit();

                    set_ble_response ( START_TREAT, adc_round (vbat, 2), 0, 0xFF );
                    set_Response_characteristic ();
                    DeliveredIave = DeliveredIsum / Num_Pulses;
                    log_entry( 7, 
                        ((START_TREAT << 24) | (Satur8(Abort_Ctr) << 16)   | (temperature << 8)            | (((uint8_t)rssi) & 0xFF) ),
                        ((START_TREAT << 24) | (adc_round(IbatMax,2) << 16)    | (adc_round(VbatMin,2) << 8)       | adc_round(VBOOSTMax,2) ),
                        ((START_TREAT << 24) | (adc_round(PulseDecay,2) << 16) | (adc_round(PulseVmax,2) << 8)     | 0 ),
                        ((START_TREAT << 24) | (adc_round(Threshold,2) << 16)  | (adc_round(DeliveredIave,2) << 8) | adc_round(LoadR,2))
                        );
                    adc_state = HV_DONE;
                    #ifdef BLOCKING
                    return false;
                    #else
                    return;
                    #endif
                }
                else
                {


                    RefreshThreshold = true;
                    HVrampStart = true;
                    adc_state = HVRAMP;
                    //start_saadc ( MON_VBOOST, 1, true );

                    if ( Pulse_Ctr == 1 )
                        PWM_value = 1;
                    else
                        PWM_value = Strength_table[Table_index][Strength].PWMstart;
                    //pwm_initialize();
                    //pwm_set_level( PWM_value );
                    //pwm_start();
                }
                break;

            case HVRAMP:
                #ifdef BLOCKING
                //nrfx_saadc_sample_convert ( MON_VBOOST, &ADCbuf[0] ); 
                #endif
                  vboost = ADCbuf[0];

                  if ( vboost >= Threshold )
                  {
                      if ( HVrampStart )
                      {
                          RefreshThreshold = false;
                          ThresholdSave = Threshold;
                      }  
                      Threshold = vboost;
                      save_results( Threshold, 2 );
                      if ( vboost > VBOOSTMax) VBOOSTMax = vboost;
                      Step_Ctr = 0;                                         // HVRAMP step counter restarts at zero when we exceed desired threshold (for next ramp)
                      adc_state = SAMPLE_AFTER;
                      //start_saadc ( MON_VBAT, 1, true );
                  }
                  else
                  {
                      if ( ++Step_Ctr >= Step_End )                         // Increment HVRAMP step counter until we reach target (Step_End)
                      {                                                     // Once we reach target
                          if ( PWM_value < PWM_limit )                      // increment PWM_value (VBOOST), if we exceed the limit
                          {
                              PWM_value++;
                              if ( PWM_value > PWMmax) PWMmax = PWM_value;  // record this PWM_value if it is largest since we started testing
                              //pwm_set_level( PWM_value );                   // set PWM at PWM_Limit
                              //start_saadc ( MON_VBOOST, 1, false );
                          }
                          else
                          {
                              if ( Short_Ctr < 255 ) Short_Ctr++;           // increment 'short' pulse count
                              Threshold = vboost;
                              save_results( Threshold, 2 );
                              if ( vboost > VBOOSTMax) VBOOSTMax = vboost;
                              adc_state = SAMPLE_AFTER;
                              //start_saadc ( MON_VBAT, 1, true );
                          }
                          Step_Ctr = 0;                                     // HVRAMP step counter restarts at zero when we exceed desired threshold (for next ramp)
                      }
                      //else
                      //    start_saadc ( MON_VBOOST, 1, false );
                  }
                break;

            case SAMPLE_AFTER:
                if ( !Step_Ctr++ )                                        // if this is the first time into SAMPLE_BEFORE (Step_Ctr is zero)
                {                                                         // (increment Step_Ctr) and read VBAT
                    #ifdef BLOCKING
                    //nrfx_saadc_sample_convert ( MON_VBAT, &ADCbuf[0] ); 
                    #endif
                    vbat = ADCbuf[0];
                    if ( vbat < VbatMin ) VbatMin = vbat;
                    save_results ( vbat, 2 );
                    //start_saadc( MON_IBAT, 1, true );

                }
                else
                {
                    #ifdef BLOCKING
                    //nrfx_saadc_sample_convert ( MON_IBAT, &ADCbuf[0] ); 
                    #endif
                    ibat = ADCbuf[0];
                    if ( ibat > IbatMax ) IbatMax = ibat;
                    save_results ( ibat, 2 );
                    //pwm_stop();
                    adc_state = PULSE;

                    Step_Ctr = 0;                                         // Restart Step_Ctr for PULSE MODE. Start at 1?, initializing takes 10us!

                    for ( delay_loop = 0; delay_loop < SampleHoldoff; delay_loop++ )
                        ticks = delay_loop;

                    //start_saadc ( MON_VOLTAGE, NumSamples, true );
                    timer2_start();
                    enable_PULSE_EN();
                }
                break;

            case PULSE:
                #ifdef BLOCKING
                //for ( count = 0; count < NumSamples; count++ )
                    //nrfx_saadc_sample_convert ( MON_VOLTAGE, &ADCbuf[count] ); 
                #endif
                disable_PULSE_EN();
                PulseWidth = timer2_stop();
                PulseVmax = ADCbuf[0];

                for ( count = 0; count < 4 ; count++ )
                    save_results ( ADCbuf[count], 2 );
                for ( count = NumSamples-4; count < NumSamples ; count++ )
                    save_results ( ADCbuf[count], 2 );

                PulseVsum = 0;
                for ( count = 0; count < NumSamples; count++ )
                    PulseVsum += ADCbuf[count];
                save_results( (PulseVsum / 256),  0 );
                save_results( (PulseVsum & 0xFF), 0 );

                PulseDecay = PulseVmax - ADCbuf[NumSamples-1];
                LoadR = ComputeLoadR ( Threshold, (PulseDecay+2)/4 );
                save_results( LoadR, 2 );
                DeliveredI = ComputeDI( LoadR, PulseVsum );

                value = PulseWidth / 16;
                if ( value < 150 )
                    value = 0;
                else
                if ( value < 277 )
                    value = value - 149;
                else
                if ( value < 530 ) 
                    value = 128 + (value-276)/2;
                else
                    value = 0xFF;
                save_results ( value, 0 );

                DeliveredIsum += DeliveredI;
                saadc_uninit();

                if ( Adapt )
                {
                    if ( RefreshThreshold )
                        Threshold = ComputeThreshold( LoadR, adc_round(TargetDeliveredI,2) );
                    else
                        Threshold = ThresholdSave;
                }
                else
                {
                    Threshold = Strength_table[Table_index][Strength].Threshold;
                }

                if ( ++Pulse_Ctr > Num_Pulses )        // if we reach number of pulses
                {
                    adc_state = HV_DONE;
                    treat_active = false;
                    read_temperature(); 
                    temperature = adc_round ( temperature, 2 );
                    set_ble_response ( START_TREAT, adc_round (vbat, 2), PWMmax, Short_Ctr );
                    set_Response_characteristic ();
                    DeliveredIave = DeliveredIsum / Num_Pulses;
                    TargetDeliveredI = DeliveredIave;
                    log_entry( 7, 
                        ((START_TREAT << 24) | (Satur8(Abort_Ctr) << 16)   | (temperature << 8)            | (((uint8_t)rssi) & 0xFF) ),
                        ((START_TREAT << 24) | (adc_round(IbatMax,2) << 16)    | (adc_round(VbatMin,2) << 8)       | adc_round(VBOOSTMax,2) ),
                        ((START_TREAT << 24) | (adc_round(PulseDecay,2) << 16) | (adc_round(PulseVmax,2) << 8)     | 0 ),
                        ((START_TREAT << 24) | (adc_round(Threshold,2) << 16)  | (adc_round(DeliveredIave,2) << 8) | adc_round(LoadR,2))
                        );

                    if ( !Adapt )
                        TargetDeliveredI = DeliveredIave;
                }
                Step_Ctr = 0;                           // after processing all steps, clear Step_Ctr
                return ( false );
                break;

            case HEALTH_MODE:
                vbat = ADCbuf[0];
                read_temperature();
                temperature = adc_round ( temperature, 2 );
                set_ble_response ( GET_HEALTH, adc_round (vbat, 2), temperature, rssi );
                set_Response_characteristic ();
                log_entry( 3, 0, 0, 0, 0 );
                saadc_uninit();
                adc_state = HV_DONE;
                #ifdef BLOCKING
                return false;
                #endif
                break;
    
            default:
                #ifdef BLOCKING
                return false;
                #else
                return;
                #endif
            }
        }
        #ifdef BLOCKING
        return true;
        #endif
}

uint8_t Satur8( uint32_t value )
{
    if ( value > 0xFF )
        value = 0xFF;
    return (value);
}

/** @} */
