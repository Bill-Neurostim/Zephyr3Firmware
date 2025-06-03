/* Copyright (c) 2019 NeurostimOAB Inc. All Rights Reserved.
 *
 */
//
// PINS COMMON TO ALL HARDWARE VERSIONS
//
#define IO_AIN_SPARE1           2   // AIN0
#define IO_AIN_SPARE2		3   // AIN1

#define LED                     11
#define DEBUG1                  13

#define MIC_CLK                 20
#define MIC_DOUT		21

#define SPARE3                  25
#define SPARE2                  26
#define SPARE1                  27

#define IO_MON_IBAT		28  // AIN4
#define IO_MON_VBAT		29  // AIN5

// Analog inputs
#define MON_AIN_SPARE1		NRF_SAADC_INPUT_AIN0
#define MON_AIN_SPARE2		NRF_SAADC_INPUT_AIN1
#define	MON_IBAT		NRF_SAADC_INPUT_AIN4
#define	MON_VBAT		NRF_SAADC_INPUT_AIN5

#define RST                     1
#define PROG                    4
#define MFIO                    5
#define CS                      6
#define SDO                     7
#define INT1                    8
#define INT2                    9       
