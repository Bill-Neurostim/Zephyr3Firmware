/**@brief Copyright 2020 Neurostim Technologies LLC
*/
#include "global.h"
#include "app_timer.h"
#include "nrf_soc.h"
#include "ada_tables.h"
#include "string.h"

//
// Strength Table entries are
// Threshold, Step_End, PWMstart and VBATheadroom
//
STRENGTH_TABLE_ENTRY Strength_table[6][MAX_STRENGTH+1] =
{
    { // Strength Table 5Hz (same as 20)
        {0,	0,	0,	  0},	// 0: 0V
        {45,	50,	1,	133},	// 1: 4.25V
        {90,	60,	2,	135},	// 2: 8.5V
        {135,	70,	3,	137},	// 3: 12.75V
        {180,	80,	4,	140},	// 4: 17V
        {225,	90,	5,	142},	// 5: 21.25V
        {270,	100,	6,	144},	// 6: 25.5V
        {315,	110,	6,	146},	// 7: 29.75V
        {360,	120,	6,	148},	// 8: 34V
        {406,	130,	7,	150},	// 9: 38.25V
        {451,	140,	7,	152},	// 10: 42.5V
        {496,	150,	7,	154},	// 11: 46.75V
        {541,	160,	8,	157},	// 12: 51V
        {586,	170,	8,	159},	// 13: 55.25V
        {631,	180,	8,	161},	// 14: 59.5V
        {676,	190,	9,	163},	// 15: 63.75V
        {721,	200,	9,	165},	// 16: 68V
        {766,	210,	9,	167},	// 17: 72.25V
        {812,	220,	10,	169},	// 18: 76.5V
        {857,	230,	10,	171},	// 19: 80.75V
        {902,	240,	10,	174},	// 20: 85V
    },
    { // Strength Table 10Hz (same as 20)
        {0,	0,	 0,	  0},	// 0: 0V
        {45,	50,	 1,	133},	// 1: 4.25V
        {90,	60,	 2,	135},	// 2: 8.5V
        {135,	70,	 3,	137},	// 3: 12.75V
        {180,	80,	 4,	140},	// 4: 17V
        {225,	90,	 5,	142},	// 5: 21.25V
        {270,	100,	 6,	144},	// 6: 25.5V
        {315,	110,	 6,	146},	// 7: 29.75V
        {360,	120,	 6,	148},	// 8: 34V
        {406,	130,	 7,	150},	// 9: 38.25V
        {451,	140,	 7,	152},	// 10: 42.5V
        {496,	150,	 7,	154},	// 11: 46.75V
        {541,	160,	 8,	157},	// 12: 51V
        {586,	170,	 8,	159},	// 13: 55.25V
        {631,	180,	 8,	161},	// 14: 59.5V
        {676,	190,	 9,	163},	// 15: 63.75V
        {721,	200,	 9,	165},	// 16: 68V
        {766,	210,	 9,	167},	// 17: 72.25V
        {812,	220,	10,	169},	// 18: 76.5V
        {857,	230,	10,	171},	// 19: 80.75V
        {902,	240,	10,	174},	// 20: 85V
    },
    { // Strength Table 20Hz
        {0,	0,	 0,	  0},	// 0: 0V
        {45,	50,	 1,	133},	// 1: 4.25V
        {90,	60,	 2,	135},	// 2: 8.5V
        {135,	70,	 3,	137},	// 3: 12.75V
        {180,	80,	 4,	140},	// 4: 17V
        {225,	90,	 5,	142},	// 5: 21.25V
        {270,	100,	 6,	144},	// 6: 25.5V
        {315,	110,	 6,	146},	// 7: 29.75V
        {360,	120,	 6,	148},	// 8: 34V
        {406,	130,	 7,	150},	// 9: 38.25V
        {451,	140,	 7,	152},	// 10: 42.5V
        {496,	150,	 7,	154},	// 11: 46.75V
        {541,	160,	 8,	157},	// 12: 51V
        {586,	170,	 8,	159},	// 13: 55.25V
        {631,	180,	 8,	161},	// 14: 59.5V
        {676,	190,	 9,	163},	// 15: 63.75V
        {721,	200,	 9,	165},	// 16: 68V
        {766,	210,	 9,	167},	// 17: 72.25V
        {812,	220,	10,	169},	// 18: 76.5V
        {857,	230,	10,	171},	// 19: 80.75V
        {902,	240,	10,	174},	// 20: 85V
    },
    { // Strength Table 30Hz (same as 50hz)
        {0,	0,	 0,	  0},	// 0: 0V
        {42,	7,	 1,	133},	// 1: 4V
        {85,	4,	 1,	135},	// 2: 8V
        {127,	21,	 2,	137},	// 3: 12V
        {170,	28,	 2,	140},	// 4: 16V
        {212,	35,	 3,	142},	// 5: 20V
        {254,	42,	 3,	144},	// 6: 24V
        {297,	49,	 4,	146},	// 7: 28V
        {339,	56,	 4,	148},	// 8: 32V
        {382,	63,	 5,	150},	// 9: 36V
        {424,	70,	 5,	152},	// 10: 40V
        {467,	77,	 6,	154},	// 11: 44V
        {509,	84,	 6,	157},	// 12: 48V
        {551,	91,	 7,	159},	// 13: 52V
        {594,	98,	 7,	161},	// 14: 56V
        {636,	105,	 8,	163},	// 15: 60V
        {679,	112,	 8,	165},	// 16: 64V
        {721,	119,	 9,	167},	// 17: 68V
        {763,	126,	 9,	169},	// 18: 72V
        {806,	133,	10,	171},	// 19: 76V
        {848,	140,	10,	174},	// 20: 80V
    },
    { // Strength Table 50Hz Rev 1
        {0,	0,	 0,	  0},	// 0: 0V
        {42,	7,	 1,	133},	// 1: 4V
        {85,	4,	 1,	135},	// 2: 8V
        {127,	21,	 2,	137},	// 3: 12V
        {170,	28,	 2,	140},	// 4: 16V
        {212,	35,	 3,	142},	// 5: 20V
        {254,	42,	 3,	144},	// 6: 24V
        {297,	49,	 4,	146},	// 7: 28V
        {339,	56,	 4,	148},	// 8: 32V
        {382,	63,	 5,	150},	// 9: 36V
        {424,	70,	 5,	152},	// 10: 40V
        {467,	77,	 6,	154},	// 11: 44V
        {509,	84,	 6,	157},	// 12: 48V
        {551,	91,	 7,	159},	// 13: 52V
        {594,	98,	 7,	161},	// 14: 56V
        {636,	105,	 8,	163},	// 15: 60V
        {679,	112,	 8,	165},	// 16: 64V
        {721,	119,	 9,	167},	// 17: 68V
        {763,	126,	 9,	169},	// 18: 72V
        {806,	133,	10,	171},	// 19: 76V
        {848,	140,	10,	174},	// 20: 80V
    },
    {
        // Strength Table 100Hz Rev 2
        {0,	 0,	 0,       0},	// 0: 0.00V
        {40,	16,	 1,	133},	// 1:  3.75V
        {80,	20,	 1,	135},	// 2: 7.50V
        {119,	23,	 2,	137},	// 3:  11.25V
        {159,	26,	 2,	140},	// 4:  15.00V
        {199,	30,	 3,	142},	// 5:  18.75V
        {239,	33,	 3,	144},	// 6:  22.50V
        {278,	36,	 4,	146},	// 7:  26.25V
        {318,	40,	 4,	148},	// 8: 30.00V
        {358,	43,	 5,	150},	// 9:  33.75V
        {398,	46,	 5,	152},	// 10: 37.50V
        {437,	50,	 6,	154},	// 11: 41.25V
        {477,	53,	 6,	157},	// 12: 45.00V
        {517,	56,	 7,	159},	// 13: 48.75V
        {557,	60,	 7,	161},	// 14: 52.50V
        {596,	63,	 9,	163},	// 15: 56.25V
        {636,	66,	 9,	165},	// 16: 60.00V
        {676,	70,	10,	167},	// 17: 63.75V
        {716,	73,	10,	169},	// 18: 67.50V
        {756,	76,	11,	171},	// 19: 71.25V
        {795,	80,	11,	174},	// 20: 75.00V
    },
};

extern uint8_t            Table_index;

// BLE REQUEST parameters
bool              start_treat;    // BLE Semaphore to Start a Treatment
bool              stop_treat;     // BLE Semaphore to Stop  a Treatment
bool              get_health;     // BLE Semaphore used to return health of system

uint8_t           Strength;       // Pulse amplitude, 5% increments, enforce range 0..20, default (20%)
uint8_t           LastStrength;
uint8_t           Slope;          // The number of ADC cycles between PWM Ramp Steps.
uint16_t          Threshold;
bool              Adapt;
bool              RefreshThreshold;
bool              HVrampStart;
uint16_t          ThresholdSave;

// BLE RESPONSE parameters
nrf_saadc_value_t vboost;         // Last reading of VBOOST ADC
nrf_saadc_value_t vbat;           // Last reading of VBAT ADC
nrf_saadc_value_t ibat;           // Last reading of IBAT ADC
nrf_saadc_value_t voltage;        // Last reading of VOLTAGE ADC
uint8_t           PWMmax;         // maximum PWM reading at end of treatment
uint8_t           Short_Ctr;      // Number of pulses issued at lower voltage than desired
uint16_t          Abort_Ctr;      // Number of failures due to low voltage (headroom)
uint32_t          temperature;    // last temperature reading
int8_t            rssi;           // strength of BLE received signal
nrf_saadc_value_t VbatMin;
nrf_saadc_value_t IbatMax;
nrf_saadc_value_t VBOOSTMax;

// Control parameters
bool              treat_active;   // Global Semaphore saying Treatment is active
uint8_t           PWM_value;      // Current PWM level
uint8_t           PWM_limit;      // Maximum PWM level
enum HV_MODE      adc_state;

uint16_t          Duration;       // default 10 seconds, probably not played with.
uint8_t           Frequency;      // default 20 Hz, major issues if changed
uint32_t          Timer_period;
bool              Battle_Override;
uint16_t          TestThreshold;
uint8_t           NumSamples;
uint16_t          ADCTime;
uint16_t          SampleHoldoff;

bool              Enable_Adaptive;

uint16_t          Pulse_Ctr;      // Pulse counter - number of pulses in the treatment
uint16_t          Num_Pulses;     // Number of pulses to issue
uint8_t           Step_Ctr;       // Step counter - Counter for ramping up HVRAMP voltage
uint8_t           Step_End;       // Step end value - Target count for Step_Ctr

uint16_t          LoadR;
uint16_t          TargetDeliveredI;
uint16_t          DeliveredI;
uint32_t          DeliveredIsum;

uint16_t          PulseVmax;
uint16_t          PulseVsum;

uint16_t          DeliveredIave;
int16_t           PulseDecay;

/**
 * @brief Initialize treatment manager parameters
 */
void treatment_manager_init()
{
    start_treat   = false;
    stop_treat    = false;
    get_health    = false;

    Strength      = DEFAULT_STRENGTH;
    LastStrength  = DEFAULT_STRENGTH;
    Slope         = 0;
    Threshold     = 0;
    Adapt         = false;


    vboost        = 0;
    vbat          = 0;
    ibat          = 0;
    voltage       = 0;
    PWMmax        = 0;
    Short_Ctr     = 0;
    Abort_Ctr     = 0;
    temperature   = 0;
    rssi          = 0;
    VbatMin       = INT16_MAX;
    IbatMax       = 0;
    VBOOSTMax     = 0;


    treat_active  = false;
    PWM_value     = 0;
    PWM_limit     = PWM_LIMIT_CONSTANT;
    adc_state     = SAMPLE_BEFORE;

    Duration      = DEFAULT_DURATION;
    Frequency     = FREQUENCY_DEFAULT;
    Timer_period  = 1000/FREQUENCY_DEFAULT;
    set_table_index();
    
    TestThreshold = TESTTHRESHOLD_DEFAULT;
    NumSamples    = NUMSAMPLES_DEFAULT;
    ADCTime       = ADCTIME_DEFAULT;
    SampleHoldoff = SAMPLEHOLDOFF_DEFAULT;
  
    Enable_Adaptive = false;

    Pulse_Ctr     = 0;
    Num_Pulses    = 0;
    Step_Ctr      = 0;                                      // HV ramp step counter starts at 0 upon initialization
    Step_End      = 0;                                      // Step end value - Target count for Step_Ctr

    LoadR            = 0;
    TargetDeliveredI = 0;
    DeliveredI       = 0;
    DeliveredIsum    = 0;
    PulseVmax        = 0;
    PulseVsum        = 0;
}

/**
 * @brief Start a treatment
 */
void start_treatment ( uint8_t param1, uint8_t param2, uint8_t param3 )
{
    Strength    = param1;
    Num_Pulses  = param2;
    Slope       = param3;
    start_treat = true;
}

/**
 * @brief Set PWM_limit (originally Frequency), Battle Override (Duration) and Pulsewidth
 */

/**
 * @brief Save reading to results buffer
 */
void save_results ( nrf_saadc_value_t reading, uint8_t digits )
{
    reading = adc_round ( reading, digits );

    // save data and increment write pointer
    results_vector[results_wrptr++] = reading;
    // if write pointer goes past valid data area, reset it
    if ( results_wrptr >= (RESULTS_SIZE-INCREMENTAL_LOG_SIZE) )
        results_wrptr = (RESULTS_SIZE-INCREMENTAL_LOG_SIZE-1);
}

/**@brief treatment_manager().  Generate high voltage pulses
 */
void treatment_manager ( void ) {
    uint32_t          temp;

    if ( stop_treat ) {						// if we are supposed to stop a treatment
        start_treat = false;                                    // clear treatment flags
        stop_treat = false;
        treat_active = false;
        disable_PULSE_EN();
        //pwm_stop();
        saadc_uninit();
        set_ble_response ( STOP_TREAT, adc_round (vbat,2), PWMmax, Short_Ctr );
        set_Response_characteristic ();
        DeliveredIave = DeliveredIsum / Num_Pulses;
        log_entry( 7,
            ((STOP_TREAT << 24) | (Satur8(Abort_Ctr) << 16)   | (temperature << 8)            | (((uint8_t)rssi) & 0xFF) ),
            ((STOP_TREAT << 24) | (adc_round(IbatMax,2) << 16)    | (adc_round(VbatMin,2) << 8)       | adc_round(VBOOSTMax,2) ),
            ((STOP_TREAT << 24) | (adc_round(PulseDecay,2) << 16) | (adc_round(PulseVmax,2) << 8)     | 0 ),
            ((STOP_TREAT << 24) | (adc_round(Threshold,2) << 16)  | (adc_round(DeliveredIave,2) << 8) | adc_round(LoadR,2))
            );
        return;							// and leave
    }
    else
    if ( start_treat ) 
    {                                                           // if we are supposed to start a treatment (and one not active)
        start_treat = false;					// clear start_treat flag
        stop_treat = false;					// clear stop_treat flag (just in case it is set)
        treat_active = true;					// mark that we are now in a treatment

        Pulse_Ctr = 1;
        Step_Ctr = 0;                                           // HV ramp step counter starts at 0 upon initialization
        DeliveredIsum = 0;
        PWMmax = 0;

        //if ( ++Test_Ctr == 2 )
        //    Test_Ctr++;
        Short_Ctr = 0;
        Step_End = Slope;
        if ( !Slope )
            Step_End = Strength_table[Table_index][Strength].Step_End;
        VbatMin = INT16_MAX;
        IbatMax = 0;
        VBOOSTMax = 0;

        led_seq_init ( LED_SEQ_START, LED_DECODE, 5, Strength );

        if ( !Num_Pulses )
        {
            temp = (Duration * Frequency);    // save number of pulses to u32
            Num_Pulses = 0xFFFF;              // check for roll-over
            if  ( temp < 0xFFFF )
               Num_Pulses = temp;             // save number of pulses
        }

        if ( (Strength == LastStrength) && Enable_Adaptive )
        {
            Adapt = true;
            Threshold = TestThreshold;
        }
        else
        {
            Adapt = false;
            LastStrength = Strength;
            Threshold = Strength_table[Table_index][Strength].Threshold;
        }
        adc_state = SAMPLE_BEFORE;
        //start_saadc ( MON_VBAT, 1, true );

        //while ( adc_state != HV_DONE );
    }
    else
    if ( get_health ) 
    {
        get_health = false;
        adc_state = HEALTH_MODE;
        Step_Ctr = 0;                                           // HV ramp step counter set to 0 when GET_HEALTH is requested
        //start_saadc ( MON_VBAT, 1, true );
    }
    else
    if ( treat_active ) 
    {                                                           // if we are in a treatment
        adc_state = SAMPLE_BEFORE;
        //start_saadc ( MON_VBAT, 1, true );
    }
    else
        return;

    #ifdef BLOCKING
    while ( blocking_saadc_callback() );
    #endif
}

/**@brief Read temperature
 */
void read_temperature()
{
    uint32_t err_code = sd_temp_get( &temperature );
}

/**@brief enable_PULSE_EN().  Turn ON EN_PULSE
 */
void enable_PULSE_EN()
{
//    nrf_gpio_cfg_output( EN_PULSE_1 );      // set EN_PULSE_1 as output
//    nrf_gpio_pin_set( EN_PULSE_1 );		// make sure EN_PULSE_1 is ON
}

/**@brief disable_PULSE_EN().  Turn OFF EN_PULSE
 */
void disable_PULSE_EN()
{
//    nrf_gpio_cfg_output( EN_PULSE_1 );      // set EN_PULSE_1 as output
//    nrf_gpio_pin_clear ( EN_PULSE_1 );      // make sure EN_PULSE_1 is OFF
 }

//--------------------------------------------------------------------------------------------------
// Adaptive algorithm routines
//--------------------------------------------------------------------------------------------------

//
// This function scans the Threshold Table to find the bounds of Indices that include Threshold
// It scans the table from lowest to highest. When it find a value higher than Threshold,
// that index is the higher bound and the lower bound is index-1. It test for extremes.
//
uint8_t *LookupThresholdIdx( uint16_t local_Threshold )
{
  static uint8_t indices[2];
  uint8_t ThresholdIdx;

  indices[0] = NUM_THRESHOLD-1;    // Default to threshold value > maximum table entry, return (max, max)
  indices[1] = NUM_THRESHOLD-1;

  for ( ThresholdIdx = 0; ThresholdIdx < NUM_THRESHOLD; ThresholdIdx++ )
  {
     if (ThresholdTable[ThresholdIdx] > local_Threshold )
     {
        if ( ThresholdIdx == 0 )    // Threshold value is below minumum table entry, return (0, 0)
        {
           indices[0] = 0;
           indices[1] = 0;
        }
        else
        {
           indices[0] = ThresholdIdx - 1;
           indices[1] = ThresholdIdx;
        }
        break; // since we found an entry, leave 'for' loop
     }
  }
  return ( indices );
}

//
// This function scans the LoadR Table to find the bounds of Indices that include LoadR
// It scans the table from lowest to highest. When it find a value higher than LoadR,
// that index is the higher bound and the lower bound is index-1. It test for extremes.
//
uint8_t *LookupLoadRIdx( uint16_t local_LoadR )
{
  static uint8_t indices[2];
  uint8_t LoadRIdx;

  indices[0] = NUM_RESISTANCE-1;    // Default to threshold value > maximum table entry, return (max, max)
  indices[1] = NUM_RESISTANCE-1;

  for ( LoadRIdx = 0; LoadRIdx < NUM_RESISTANCE; LoadRIdx++ )
  {
     if (LoadRtable[LoadRIdx] > local_LoadR )
     {
        if ( LoadRIdx == 0 )    // Threshold value is below minumum table entry, return (0, 0)
        {
           indices[0] = 0;
           indices[1] = 0;
        }
        else
        {
           indices[0] = LoadRIdx - 1;
           indices[1] = LoadRIdx;
        }
        break; // since we found an entry, leave 'for' loop
     }
  }
  return ( indices );
}
// end of function LookupLoadRIdx

// This function calculates the Threshold as a function of Load Resistance and Delivered Current.
// ADAtable has 1024 points (32 Thresholds by 32 Loads) of DeliveredI samples in a 3 dimensional space. 
// From AAtable, 4 bounding values define a plane, an Interpolated point in that plane is the Result.
// It traverses the Threshold vs. DeliveredI line for each AATable Entry at the enclosing LoadR, 
// finding the 4 Threshold bounding values in ADAarray.
// It then linealy extrapolates between the two Threshold Bounds, handling values outside of ADAarray.
// Assumes DeliveredI monitonically increases with Threshold and monitonically decreases with LoadR
// This is enforced while making an ADAT file from which the ADAarray is loaded.
// Mapping to common Interpolate plane means swapping X (the known) and Y (the unknown) axis.
//
uint16_t ComputeThreshold( uint16_t local_LoadR, uint8_t local_DeliveredI )
{
    uint16_t Vertices[6];      // This stores vertices for enclosing plane: X0, X1, Y0, Y1, Y2, Y3
    uint8_t  Indices [6];      // This stores ADAarray indices for X0, X1, Y0, Y1, Y2, Y3
    uint8_t  DIvalues[4];      // This stores PulseDecay at (X0,YO), (X0,Y1), (X1,Y2), (X1,Y3))

    uint8_t BoundsSel, ThresholdCtr, VertexCtr, Zctr;
    uint8_t *LoadRBounds;
    uint8_t DeliveredIentry;
    bool BoundsFound;
    uint16_t local_Threshold;

    // set X Indices and Vertices to map to Interpolate Plane X0 <= X1
    // Get indices that are above and below the desired Threshold
    LoadRBounds = LookupLoadRIdx( local_LoadR );                // Lookup Indices of X0 and X1 (X0 >= LoadR)
    for ( BoundsSel = 0; BoundsSel < 2; BoundsSel++ )
    {
        Indices [BoundsSel] = LoadRBounds[1-BoundsSel];         // X0 and X1 Indices swapped
        Vertices[BoundsSel] = LoadRtable[Indices[BoundsSel]];   // Set X0 and X1 Vertices
    }

    // find the Y Indices and Vertices to map to Interpolate Plane Y0 <= Y1, Y2 <= Y3
    // loop thru the two Resistances returned to find corresponding ???
    for ( BoundsSel = 0; BoundsSel < 2; BoundsSel++ )
    {
        BoundsFound = false;
        for ( ThresholdCtr = 0; ThresholdCtr < NUM_THRESHOLD; ThresholdCtr++ )   // Starts at min Threshold = lowest DeliveredI
        {
            DeliveredIentry = ADAarray[ThresholdCtr][Indices[BoundsSel]][1];
            if ( DeliveredIentry >= local_DeliveredI )        // found higher point, its upper bound is Threshold
            {
                BoundsFound = true; 
                if ( ThresholdCtr == 0 )                      // Threshold is less than min, extrapolate to min
                {
                    Indices[2 + 2 * BoundsSel] = 0;
                    Indices[3 + 2 * BoundsSel] = 0;
                }
                else
                {
                    Indices[2 + 2 * BoundsSel] = ThresholdCtr;      // first value with Delivered Current >= ADAarray
                    Indices[3 + 2 * BoundsSel] = ThresholdCtr - 1;
                }
                break;
            }
        }
        if ( !BoundsFound )
        {
            Indices[2 + 2 * BoundsSel] = NUM_THRESHOLD-1;
            Indices[3 + 2 * BoundsSel] = NUM_THRESHOLD-1;
        }
    }

    // Write Y Vertices
    for ( VertexCtr = 0; VertexCtr < 4; VertexCtr++ )                           // Write Y0, Y1, Y2, Y3, Vertices 2-5
        Vertices[2 + VertexCtr] = ThresholdTable[Indices[2 + VertexCtr]];

    // Write PDvalues: PulseDecay at (X0,YO), (X0,Y1), (X1,Y2), (X1,Y3)
    // Get Pulse Decay values
    //                Threshold      Resistance   Delivered Current
    // get ADAarray [ Indicies[2] ][ Indices[0] ][ 0 ]
    // get ADAarray [ Indicies[3] ][ Indices[0] ][ 0 ]
    // get ADAarray [ Indicies[4] ][ Indices[1] ][ 0 ]
    // get ADAarray [ Indicies[5] ][ Indices[1] ][ 0 ]
    for ( Zctr = 0; Zctr < 4; Zctr++ )                                          // Indicies are (X0, X1, Y0, Y1, Y2, Y3)
        DIvalues[Zctr] = ADAarray[Indices[Zctr + 2]][Indices[Zctr/2]][1];       // X0 and X1 

    local_Threshold = Interpolate(Vertices, local_LoadR, DIvalues, local_DeliveredI);
    return local_Threshold;                                                     // returns 10 bit Threshold
}
// end of function ComputeThreshold

//
// This function calculates the Load Resistance as a function of Threshold and PulseDecay.
//  ADAtable has 1024 points (32 Thresholds X 32 Loads) of ValueList making a 3 dimensional space. 
//  The Value List contains {PulseDecay, DeliveredI, PulseVmax}
//  From ADAtable, 4 bounding values define a plane, an Interpolated point in that plane is the Result.
//  It traverses the Load vs. Pulse Decay line for each AATable Entry at the enclosing Thresholds, 
//  finding the 4 LoadR bounding values in ADAarray.
//  It then linealy extrapolates between the two Load Bounds, handling values outside of ADAarray.
//  Assumes Pulse Decay monitonically increases with Threshold and monitonically decreases with LoadR
//  This is enforced while making an ADAT file from which the ADAarray is loaded.
//
uint16_t ComputeLoadR( uint16_t local_Threshold, uint8_t local_PulseDecay )
{
    uint16_t Vertices[6];     // This stores vertices for enclosing plane: X0, X1, Y0, Y1, Y2, Y3
    uint8_t Indices [6];      // This stores ADAarray indices for X0, X1, Y0, Y1, Y2, Y3
    uint8_t PDvalues[4];      // This stores PulseDecay at (X0,YO), (X0,Y1), (X1,Y2), (X1,Y3))

    uint8_t BoundsSel, LoadCtr, VertexCtr, Zctr;
    uint8_t *ThresholdBounds;
    uint8_t PulseDecayEntry;
    bool BoundsFound;
    uint16_t local_LoadR;

    // set X Indices and Vertices (Threshold)
    // Get indices that are above and below the desired Threshold
    ThresholdBounds = LookupThresholdIdx( local_Threshold );      // Lookup X0,X1 Indices; X0 <= Threshold
    for ( BoundsSel = 0; BoundsSel < 2; BoundsSel++ )
    {
        Indices[BoundsSel]  = ThresholdBounds[BoundsSel];         // Set X0, X1 Indices
        Vertices[BoundsSel] = ThresholdTable[Indices[BoundsSel]]; // Set X0, X1 Vertices
    }

    // find the Y Indices (Resistance)
    // loop thru the two Thresholds returned to find corresponding resistances
    for ( BoundsSel = 0; BoundsSel < 2; BoundsSel++ )             // 0: check at Lower Threshold Bound, 1: check at Upper bound
    {
        BoundsFound = false;
        for ( LoadCtr = 0; LoadCtr < NUM_RESISTANCE; LoadCtr++ )  // Starts at lowest load, therefore highest PulseDecay
        {
            PulseDecayEntry = ADAarray[ThresholdBounds[BoundsSel]][LoadCtr][0];
            if ( PulseDecayEntry < local_PulseDecay )             // found lower point, its upper bound is LoadR
            {
                BoundsFound = true;
                if ( LoadCtr == 0 )                               // LoadR is at extreme low, extrapolate at lowest LoadR
                {
                    Indices[ 2 + 2 * BoundsSel ] = 0;
                    Indices[ 3 + 2 * BoundsSel ] = 0;
                }
                else
                {
                    Indices[ 2 + 2 * BoundsSel ] = LoadCtr - 1;
                    Indices[ 3 + 2 * BoundsSel ] = LoadCtr;
                }
                break;
            }
        }
        if ( !BoundsFound )
        {
            Indices[ 2 + 2 * BoundsSel ] = NUM_RESISTANCE-1;
            Indices[ 3 + 2 * BoundsSel ] = NUM_RESISTANCE-1;
        }
    }

    // Get Vertices for Y Indices (Resistance values)
    for ( VertexCtr = 0; VertexCtr < 4; VertexCtr++ )
        Vertices [ 2 + VertexCtr ] = LoadRtable[Indices[2 + VertexCtr]];

    // Get Pulse Decay values
    //                Threshold      Resistance   PulseDecay Value
    // get ADAarray [ Indicies[0] ][ Indices[2] ][ 0 ]
    // get ADAarray [ Indicies[0] ][ Indices[3] ][ 0 ]
    // get ADAarray [ Indicies[1] ][ Indices[4] ][ 0 ]
    // get ADAarray [ Indicies[1] ][ Indices[5] ][ 0 ]
    for ( Zctr = 0; Zctr < 4; Zctr++ )
        PDvalues[Zctr] = ADAarray[ Indices[Zctr/2] ][ Indices[Zctr+2] ][ 0 ];

    local_LoadR = Interpolate( Vertices, local_Threshold, PDvalues, local_PulseDecay );  // Interpolate returns scaled integer
    return local_LoadR;   // returns value
}

// This function calculated the Delivered Current as a function of LoadR and a PulseResultsList which
// contains VBATbefore, IBATbefore, 22 ADC Pulse Voltages and VBATafter
// See Adaptive Algorithm calcs.xlsx for constant derivation 
// This is a combination of all the integer to float conversions
#define MIKES_MIRACLE_CONSTANT 25264
#define DIVIDER_CONSTANT  256
uint16_t ComputeDI ( uint16_t local_LoadR, uint16_t local_PulseVsum )
{
    uint32_t value;

    value = (local_PulseVsum * MIKES_MIRACLE_CONSTANT);
    value /= NumSamples;
    value += (local_LoadR / 2);   // add in 1/2 the divisor so we round
    value /= local_LoadR;
    value += (DIVIDER_CONSTANT/2);
    value /= DIVIDER_CONSTANT;
    return ( value );
}

// This function divides the number by the divisor and returns a rounded value.  If the divisor is
// zero it returns 0.  The code divides the passed number by the divisor rounding appropriately.
int32_t int32_round ( int32_t number, int32_t divisor )
{
    int32_t  result = 0;

    if ( divisor != 0 )
        result = (number + (divisor/2)) / divisor;

    return ( result );
}

// end of function LookupThresholdValue

#define U15MAX    32768
#define U16MAX    65536
// This function linearly Interpolates in a plane defined by X0,Y0 X0,Y1, X1,Y2 and X1,Y3
// The parameter Vertices is a list (X0, X1, Y0, Y1, Y2, Y3) 
// Where X is known and Y is the unknown to be Interpolated.
// It calculates the Y point on the X line that most closely matches Z
// The value of Z at each vertex is in parameter Zvalues a list:(Z(X0,YO), Z(X0,Y1), Z(X1,Y2), Z(X1,Y3))  
// Z is either Pulse Decay or Delivered Current
uint16_t Interpolate( uint16_t *Vertices, uint16_t X, uint8_t *Zpoints, uint8_t Z )   //  Z values from byte Table
{
    int32_t  X0, X1, Y0, Y1, Y2, Y3, Z00, Z01, Z12, Z13;
    int32_t  Xscale, Xdiff;
    int32_t  Z0scale, Z1scale, Zdiff;
    int32_t  YZ0, YZ1;
    uint16_t  Y;

    X0 = Vertices[0];
    X1 = Vertices[1];
    Y0 = Vertices[2];
    Y1 = Vertices[3];
    Y2 = Vertices[4];
    Y3 = Vertices[5];
    Z00 = Zpoints[0];
    Z01 = Zpoints[1];
    Z12 = Zpoints[2];
    Z13 = Zpoints[3];

    Xscale = int32_round ( ((X - X0) << 16), X1 - X0 );

    Z0scale = int32_round ( ((Z - Z00) << 16), Z01 - Z00 );

    YZ0 = Y0 + ( ((Y1-Y0)*Z0scale + U15MAX) >> 16 );

    Z1scale = int32_round ( ((Z - Z12) << 16), Z13 - Z12 );

    YZ1 = Y2 + ( ((Y3-Y2)*Z1scale + U15MAX) >> 16 );

    Y = ( (( YZ0*(U16MAX-Xscale) + YZ1*Xscale ) + U15MAX) >> 16);

    return ( Y );
}
// end of function Interpolate
/** @} */