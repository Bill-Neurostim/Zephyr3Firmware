/*  Copyright 2020 Neurostim Technologies LLC

Global variables and constants for Patch Nordic nRF52832 Microcontroller
*/
#define DFU_ENABLE
//#define DEBUG_TEST
#define TWI
//#define TEST_DEBUG

#include <stdbool.h>
#include <stdint.h>
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_drv_saadc.h"
#include "pin_definitions.h"		// Patch definitions for port pins
#include "Stream_control.h"
#include "nrf_drv_timer.h"
#include "nrf_log.h"
#include "modules\m_sound.h"
#include "modules\drv_mic.h"
#ifdef TWI
#include "nrf_drv_twi.h"
#endif

// TAG is used by global.h to make sure that global variables are declared in main.c and declared as extern in any other
// file that includes globals.h.  Since main.c has the #define MAIN it will declare all global variables in globals.h,
// all other files using globals.h will declare the variables as extern (other files do not #define MAIN)
#ifdef MAIN
    #define TAG
#else
    #define TAG	extern
#endif

//
// Declare enumerations
//
enum LED_FSM_STATE {LED_IDLE, LED_SEQ_START, LED_ON, LED_OFF, LED_DECODE_BIT};
enum LED_MODE_STATE {LED_BURST, LED_DECODE};                            // Burst of flashes or flash binary value

enum HV_MODE { 
    HVRAMP,
    SAMPLE_BEFORE,
    PULSE,
    SAMPLE_AFTER,
    HEALTH_MODE,
    HV_DONE
};

enum COMMAND {
    IDLE_MODE       = 0x0F,
    STOP_TREAT      = 0x1E,
    START_TREAT     = 0x2D,
    SET_VARIABLE    = 0x3C,
    GET_FWVERSION   = 0x4B,
    GET_HEALTH      = 0x5A,
    GET_TIME        = 0x69,
    GET_RESULT      = 0x78,
    GET_LOG         = 0x87,
    RESERVED_96     = 0x96,
    SET_MFGREC      = 0xA5,
    GET_MFGREC      = 0xB4,
    RESERVED_C3     = 0xC3,
    RESERVED_D2     = 0xD2,
    SET_STREAMCTL   = 0xE1,
    GET_STREAMCTL   = 0xF0
    };

enum STREAM_SOURCE {
    RFU_0 = 0,            // 0
    LEFT_AUDIO,           // 1
    RIGHT_AUDIO,          // 2
    THREE_AXIS_ACC,       // 3
    EMG_SENSOR,           // 4
    EKG_SENSOR,           // 5
    EEG_SENSOR,           // 6
    RESULTS_VECTOR,       // 7
    SPO2_SENSOR,          // 8
    NINE_AXIS_SENSOR,     // 9
    RFU_A,                // 10
    RFU_B,                // 11
    RFU_C,                // 12
    RFU_D,                // 13
    STREAM_STATUS,        // 14
    FILLER                // 15
    };

typedef struct { 
    uint16_t    Threshold;
    uint8_t     Step_End;
    uint8_t     PWMstart;
    uint8_t     VBATheadroom;
} STRENGTH_TABLE_ENTRY;

#define SYSTEM_VERSION                  0x02
#define FIRMWARE_VERSION_MSB            0x01
#define FIRMWARE_VERSION_LSB            0x14
#define FIRMWARE_VERSION_STRING         "NOSA V114-15"
#define BLOCKING

//
// Define system constants
//
#define PWR_UP_DLY 			200				// mS delay after wake up to start initializing
#define FREQUENCY_DEFAULT		20                              // Herz
#define PWM_FREQUENCY			400000				// Desired PWM frequency (400Khz)
#define PWM_TOPVALUE			(16000000/PWM_FREQUENCY)	// for 400Khz should be 40
#define PWM_LIMIT_CONSTANT              30
#define DEFAULT_STRENGTH		4				// default table entry
#define MAX_STRENGTH                    20				// Maximum strength value
#define DEFAULT_DURATION		10				// duration in seconds

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(8, UNIT_1_25_MS)              /**< Minimum acceptable connection interval. */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(16, UNIT_1_25_MS)             /**< Maximum acceptable connection interval. */
#define SLAVE_LATENCY                   0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds). */

#define FLEX6   0x60
#define FLEX7B  0x7B
#define FLEX7C  0x7C
#define FLEX7D  0x7D
#define NOSA_3C 0x3C

#define TESTTHRESHOLD_DEFAULT  434
#define NUMSAMPLES_DEFAULT     22
#define ADCTIME_DEFAULT        (12*16)   // 12 microseconds (times 16 clock ticks)
#ifdef BLOCKING
#define SAMPLEHOLDOFF_DEFAULT  0
#else
#define SAMPLEHOLDOFF_DEFAULT  16
#endif

#define NUM_THRESHOLD       32
#define NUM_RESISTANCE      32
#define STREAM_DATA_SIZE    131
#define THREE_AXIS_ACC_SIZE 10
#define SPO2_SENSOR_SIZE    10
//
// Macro
//
#define GET_BIT00_07_BYTE(x)  (x & 0xFF)
#define GET_BIT08_15_BYTE(x)  ((x >> 8) & 0xFF)
#define GET_BIT16_23_BYTE(x)  ((x >> 16) & 0xFF)
#define GET_BIT24_31_BYTE(x)  (x >> 24)
#define min(X,Y) (X<Y?X:Y)
#define max(X,Y) (X>Y?X:Y)

TAG bool      ble_command;            // flag indicating a ble command is ready
TAG uint32_t  ble_request;
TAG uint32_t  ble_response;
TAG uint8_t   ble_stream[132];
TAG bool      notify_flag;            // set if notifications are enabled

TAG uint32_t  log_wrptr;
TAG uint32_t  last_log_entry;

TAG uint32_t  seconds;
TAG uint32_t  test_value;

TAG uint8_t   FlexNum;
TAG uint16_t  Min_Conn_Interval;
TAG uint16_t  Max_Conn_Interval;
TAG bool      SPARE1_on_request;
TAG bool      SPARE1_on_response;
TAG uint8_t   ClkMicDivisor;
TAG uint8_t   Table_index;
TAG uint8_t   Test_Ctr;
TAG uint32_t  ticks;
TAG uint32_t  ix, iy;

#ifdef TEST_DEBUG
TAG uint32_t  test_tx_count;
TAG bool      test_flag;
#endif

TAG uint32_t  transmissions;
TAG uint32_t  errors;

TAG uint32_t  right_tx_count;
TAG uint32_t  left_tx_count;

TAG uint16_t SCR[16][16]
#ifdef MAIN
=
{
  { RFU_0_0,        RFU_0_1,        0,                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00 },
  { LEFT_AUDIO_0,   LEFT_AUDIO_1,   LEFT_AUDIO_2,         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x10 },
  { RIGHT_AUDIO_0,  RIGHT_AUDIO_1,  RIGHT_AUDIO_2,        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x20 },
  { THREE_AXIS_0,   THREE_AXIS_1,   0,                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x30 },
  { EMG_SENSOR_0,   EMG_SENSOR_1,   0,                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x40 },
  { EKG_SENSOR_0,   EKG_SENSOR_1,   0,                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x50 },
  { EEG_SENSOR_0,   EEG_SENSOR_1,   0,                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x60 },
  { RESULT_VECT_0,  RESULT_VECT_1,  0,                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x70 },
  { SPO2_SENSOR_0,  SPO2_SENSOR_1,  0,                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80 },
  { NINE_AXIS_0,    NINE_AXIS_1,    0,                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x90 },
  { RFU_A_0,        RFU_A_1,        0,                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xA0 },
  { RFU_B_0,        RFU_B_1,        0,                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xB0 },
  { RFU_C_0,        RFU_C_1,        0,                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xC0 },
  { RFU_D_0,        RFU_D_1,        0,                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xD0 },
  { STATUS_0,       STATUS_1,       STATUS_2,             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xE0 },
  { FILLER_0,       FILLER_1,       0,                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xF0 },
}
#endif
;
TAG bool channel_dirty[16]
#ifdef MAIN
=
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
#endif
;

TAG m_audio_frame_t   left_frame;
TAG m_audio_frame_t   right_frame;
TAG uint16_t          left_frame_size;
TAG uint16_t          right_frame_size;
TAG uint8_t           three_axis_acc_data[THREE_AXIS_ACC_SIZE];
TAG uint8_t           spo2_sensor_data[SPO2_SENSOR_SIZE];

// Log Array
#ifdef LOG_TEST
#define RESULTS_SIZE 0x1000                   // 4K Results log
#else
#define RESULTS_SIZE 0x2000                   // 8K Results log
#define INCREMENTAL_LOG_SIZE 64
#endif
TAG uint8_t results_vector [RESULTS_SIZE];    // log array of words for selftest and future logging
TAG int32_t results_wrptr;

#define BIT0  0x00000001
#define BIT1  0x00000002
#define BIT2  0x00000004
#define BIT3  0x00000008
#define BIT4  0x00000010
#define BIT5  0x00000020
#define BIT6  0x00000040
#define BIT7  0x00000080
#define BIT8  0x00000100
#define BIT9  0x00000200
#define BIT10 0x00000400
#define BIT11 0x00000800
#define BIT12 0x00001000
#define BIT13 0x00002000
#define BIT14 0x00004000
#define BIT15 0x00008000
#define BIT16 0x00010000
#define BIT17 0x00020000
#define BIT18 0x00040000
#define BIT19 0x00080000
#define BIT20 0x00100000
#define BIT21 0x00200000
#define BIT22 0x00400000
#define BIT23 0x00800000
#define BIT24 0x01000000
#define BIT25 0x02000000
#define BIT26 0x04000000
#define BIT27 0x08000000
#define BIT28 0x10000000
#define BIT29 0x20000000
#define BIT30 0x40000000
#define BIT31 0x80000000

const TAG uint32_t BIT_MASK[]
#ifdef MAIN
={ BIT0,  BIT1,  BIT2,  BIT3,  BIT4,  BIT5,  BIT6,  BIT7, 
   BIT8,  BIT9,  BIT10, BIT11, BIT12, BIT13, BIT14, BIT15, 
   BIT16, BIT17, BIT18, BIT19, BIT20, BIT21, BIT22, BIT23, 
   BIT24, BIT25, BIT26, BIT27, BIT28, BIT29, BIT30, BIT31, 
}
#endif
;

// adc.c
void saadc_init( nrf_saadc_input_t, uint16_t );
void saadc_uninit(void);
nrf_saadc_value_t read_adc( nrf_saadc_input_t );

// arbiter.c
void init_arbiter_timer( void );
void restart_arbiter_timer( void );
void stop_arbiter_timer( void );
void set_arbiter_timer ( uint8_t );
void arbiter_timer_handler( void * );
void increment_packet ( uint8_t );
void generate_pattern ( uint16_t, uint16_t, uint8_t *, uint8_t );
void generate_tone ( uint8_t, uint8_t, uint8_t * );

// drv_mic.c
void m_left_audio_process ( void );
void m_right_audio_process ( void );
uint32_t drv_mic_start(void);
uint32_t drv_mic_stop(void);

// flash.c
void flash_log_init ( void );
void flash_check( void );
void flash_log_erase ( void );
uint32_t flash_log_read ( uint32_t );
void flash_log_write( uint32_t *, uint32_t );
void flash_log_buffer_read ( uint32_t *, uint32_t, uint32_t );
uint16_t flash_mfg_read ( uint16_t );
void flash_mfg_write( uint8_t, uint16_t );
void log_entry ( uint8_t, uint32_t, uint32_t, uint32_t, uint32_t );

// Initialize.c
void Initialize_Patch ( void );
void initialize_globals ( void );
void initialize_io ( void );

// led_handler.c
void led_handler( void );
void led_seq_init ( enum LED_FSM_STATE, enum LED_MODE_STATE, uint16_t, uint16_t );

// main.c
void initialize_ble( void );

// m_sound.c
uint32_t m_sound_init();

// neurostim_ble.c
uint32_t ble_resp_send( void );
void set_ble_response ( uint8_t, uint8_t, uint8_t, uint8_t );
void set_Response_characteristic ( void );
ret_code_t NEUROSTIM_ble_init();
uint32_t transmit_stream ( void );

// pwm.c
void pwm_initialize ( void );
void pwm_uninitialize ( void );
void pwm_set_level( uint8_t );
void pwm_start ( void );
void pwm_stop ( void );

// saadc.c
void saadc_timer_init ( void );
void saadc_sampling_event_init ( void );
void saadc_sampling_event_enable( void );
void start_saadc( nrf_saadc_input_t, uint8_t, bool );
void saadc_callback(nrf_drv_saadc_evt_t const * );
bool blocking_saadc_callback ( void );
void timer2_init();
void timer2_start();
uint32_t timer2_stop();
uint8_t Satur8( uint32_t );

// subs.c
void spare_on( void );
void spare_off( void );
void nrf_gpio_cfg_output_strength(uint32_t, nrf_gpio_pin_drive_t);
void nrf_gpio_cfg_input_disconnect(uint32_t);
void pulse ( uint16_t );
void toggle_spare ( uint16_t );
bool read_pin ( uint16_t );
uint8_t adc_round ( uint16_t, uint8_t );
void generate_pattern ( uint16_t, uint16_t, uint8_t *, uint8_t );

// timer_handler.c
void init_treatment_timer( void );
void restart_treatment_timer( uint16_t );
void stop_treatment_timer( void );
void init_seconds_timer( void );
void start_seconds_timer( void );
void init_audio_timer( void );
void restart_audio_timer( uint16_t );
void stop_audio_timer( void );
void enable_outputs( bool );
void set_table_index( void );
void ble_parser ( void );
void treatment_timer_handler( void * );
void seconds_timer_handler( void * );
void audio_timer_handler( void * );

// treatment_manager.c prototypes
void treatment_manager_init( void );
void start_treatment ( uint8_t, uint8_t, uint8_t );
void set_parameters ( uint8_t, uint8_t, uint8_t );
void save_results ( nrf_saadc_value_t, uint8_t );
void treatment_manager( void );
void read_temperature( void );
void enable_PULSE_EN( void );
void disable_PULSE_EN( void );

uint16_t ComputeThreshold( uint16_t, uint8_t );
uint16_t Interpolate( uint16_t *, uint16_t, uint8_t *, uint8_t );
uint16_t ComputeLoadR( uint16_t, uint8_t );
uint16_t ComputeDI ( uint16_t, uint16_t );

#ifdef TWI
// twi.c prototypes
void twi_handler(nrf_drv_twi_evt_t const *, void * );
ret_code_t SPO2_write ( uint8_t, uint8_t, uint8_t );
ret_code_t SPO2_read  ( uint8_t, uint8_t index_byte );
ret_code_t SPO2_read_long ( uint8_t, uint8_t, uint8_t, uint8_t, uint16_t );
ret_code_t LIS3DH_write ( uint8_t, uint8_t );
ret_code_t LIS3DH_read ( uint8_t, uint8_t );
void twi_init ( void );
void spo2_init ( void );
void spo2_deinit ( void );
void accel_init ( void );
void accel_deinit ( void );
uint8_t readBPM ( void );
uint8_t readAccel( void );
#endif