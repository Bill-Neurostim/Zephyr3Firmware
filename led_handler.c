/**@brief Copyright 2020 Neurostim Technologies LLC
*/
#include "global.h"

// Two modes, determined by variable led_mode
//
// LED_BURST mode
//    A burst of LED lightings with duration (led_value * 50msec) and the number of lightings in led_value.
//    The time between lightings is the same as the lighting.
//      * Set led_length to how many ON pulses to flash
//	* Set led_value to how long to leave led ON
//      * Started by setting led_fsm (led finite state machine) state to LED_SEQ_START.
//
// LED_DECODE mode
//    A LED_DECODE Sequence flashes a binary code based on led_value. The number of bits is in led_length.
//    A one is a flash of 0.5 Sec,  and a zero is a flash of 0.2 Sec. The pause between bits is 0.5 seconds.
//	This can be used to indicate Strength during a Treatment.
//
//	* Set led_length to how many bits to flash
//	* Set led_value is value to flash (up to 16 bits)
//	* Started by setting led_fsm (led finite state machine) state to LED_SEQ_START.

// LED variables are global so any module can initiate LED Sequence. See SDD1604
enum LED_FSM_STATE      led_fsm;
enum LED_MODE_STATE     led_mode;
uint16_t                led_length;		// contains the number of flashes in a LED Sequence
uint16_t                led_value;		// contains the duration of a flash (*50 uS) in LED_BURST
						// or the binary value to flash In LED_DECODE.
static int16_t		led_time_ctr;
static int16_t		led_pulse_ctr;
static uint16_t		led_bit_mask;

/**@brief Turn LED on/off based on led_mode, led_length and led_value
*/
void led_handler()
{
    switch ( led_fsm )
    {
        case LED_IDLE:
            break;
        case LED_SEQ_START:					// if led_fsm is set to LED_SEQ_START this
            switch ( led_mode )					// starts a sequence
            {							// depending on the led_mode
                case LED_BURST:					// if LED_BURST mode
                    led_pulse_ctr = led_length;			// set number of pulses to generate
                    led_time_ctr = led_value;			// set length of time for pulse
                    nrf_gpio_pin_clear ( LED );                 // turn ON LED
                    led_fsm = LED_ON;				// set state to go to ON
                    break;
                case LED_DECODE:				// if LED_DECODE mode
                    led_pulse_ctr = led_length;			// set number of pulsed to generate
                    led_bit_mask = (1 << (led_length-1));	// set bit mask to most significant bit
                    led_fsm = LED_DECODE_BIT;
                    break;
            }
            break;
        case LED_ON:
            if ( --led_time_ctr <= 0 ) {                    // decrement time counter, if it goes to zero
                if ( led_mode == LED_DECODE )               // if we are in LED_DECODE mode
                    led_time_ctr = 10;                      // set the timer back to 10 (how long the OFF period is)
                else                                        // if we are in LED_BURST mode
                    led_time_ctr = led_value;               // set the timer to led value (how long the OFF period is)
                nrf_gpio_pin_set ( LED );                   // turn OFF LED
                led_fsm = LED_OFF;                          // go to OFF state
            }
            break;
        case LED_OFF:
            if ( --led_time_ctr <= 0 ) {                        // decrement time counter, if it goes to zero

                if ( --led_pulse_ctr <= 0 )                     // decrement pulse counter, if it goes to zero
                    led_fsm = LED_IDLE;                         // we are done, go to LED_IDLE mode
                else {
                    switch ( led_mode )                         // we get here because we finished a ON/OFF pulse
                    {
                        case LED_BURST:				// if in LED_BURST mode
                            led_time_ctr = led_value;		// reload timer
                            nrf_gpio_pin_clear ( LED );         // turn ON LED
                            led_fsm = LED_ON;			// do the next ON
                            break;
                        case LED_DECODE:			// if in LED_DECODE mode
                            led_bit_mask = led_bit_mask >> 1;	// move to next bit in mask
                            led_fsm = LED_DECODE_BIT;
                            break;
                    }
                }
            }

            break;
        case LED_DECODE_BIT:
            if ( led_value & led_bit_mask )		// if bit is ON
                led_time_ctr = 10;			// turn ON for 10 periods
            else					// if bit is OFF
                led_time_ctr = 4;			// turn ON for 4 periods
            nrf_gpio_pin_clear ( LED );                 // turn ON LED
            led_fsm = LED_ON;				// go to ON
            break;
        default:
            break;
    }
}

/**@brief Initialize led sequence
*/
void led_seq_init ( enum LED_FSM_STATE state, enum LED_MODE_STATE mode, uint16_t length, uint16_t value)
{
	if ( state > LED_DECODE_BIT ) state = LED_IDLE;	// check for valid values
	if ( mode > LED_DECODE ) mode = LED_BURST;
	if ( (mode == LED_DECODE) && (length > 16) ) length = 0;

	led_fsm = state;			// update led finite state machine state
	led_mode = mode;			// update led mode (LED_BURST or LED_DECODE)
	led_length = length;                    // update led_length (number of bursts or bits)
	led_value = value;			// update led_value - timer for LED_BURST for value to flash for LED_DECODE
}
/** @} */
