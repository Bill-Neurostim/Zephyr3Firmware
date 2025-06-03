/*  Copyright 2020 Neurostim Technologies LLC

This is one of several selftests available for inclusion in a Patch compilation.
A specification of this code is in SDD-1605 Selftest.docx
The purpose of this selftest is to characterize the VBOOST PWM generation and
the MON_ADC function using the SAADC.
It sequences through the allowable PWM duty cycles, allowing the experimenter to record VBOOST.
It also samples MON_VBOOST with the SAADC and logs the values in RAM.

*/
#include "global.h"

extern bool    treat_active;
extern enum SELFTEST_STATE  selftest;



/**@brief selftest1
 *
 */
bool selftest1 ( void )
{
    // if a treatment is active, quit and run treatment manager
    if ( treat_active )
        return ( false );

    switch ( selftest )
    {
        case SELFTEST_1_START:
            set_ble_response ( RUN_SELF, 0xFA, 0, 0 );    // Send FAIL code... test removed
            set_Response_characteristic ();
            log_entry( 3, 0, 0, 0 );
            break;

        case SELFTEST_2_START:
            set_ble_response ( RUN_SELF, 0xFA, 0, 0 );    // Send FAIL code... test removed
            set_Response_characteristic ();
            break;
    }

    return ( true );
}
