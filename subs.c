/**@brief Copyright 2020 Neurostim Technologies LLC
*/
#include "global.h"

void spare_on( void )
{
    nrf_gpio_cfg_output( SPARE2 );
    nrf_gpio_pin_set( SPARE2 );
}

void spare_off( void )
{
    nrf_gpio_cfg_output( SPARE2 );
    nrf_gpio_pin_clear( SPARE2 );
}

/**@brief Configure an I/O pin for output and desired strength
*/
void nrf_gpio_cfg_output_strength(uint32_t pin_number, nrf_gpio_pin_drive_t strength)
{
    nrf_gpio_cfg(
        pin_number,
        NRF_GPIO_PIN_DIR_OUTPUT,
        NRF_GPIO_PIN_INPUT_DISCONNECT,
        NRF_GPIO_PIN_NOPULL,
                    strength,
        NRF_GPIO_PIN_NOSENSE);
}

/**@brief Configure an I/O pin for input, disconnected
*/
void nrf_gpio_cfg_input_disconnect(uint32_t pin_number)
{
    nrf_gpio_cfg(
        pin_number,
        NRF_GPIO_PIN_DIR_INPUT,
        NRF_GPIO_PIN_INPUT_DISCONNECT,
        NRF_GPIO_PIN_NOPULL,
        NRF_GPIO_PIN_S0D1,
        NRF_GPIO_PIN_NOSENSE);
}

/*
    NRF_GPIO->PIN_CNF[5] = ((NRF_GPIO_PIN_DIR_OUTPUT << GPIO_PIN_CNF_DIR_Pos)
                        | (NRF_GPIO_PIN_INPUT_DISCONNECT << GPIO_PIN_CNF_INPUT_Pos)
                        | (NRF_GPIO_PIN_NOPULL << GPIO_PIN_CNF_PULL_Pos)
                        | (NRF_GPIO_PIN_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
                        | (NRF_GPIO_PIN_NOSENSE << GPIO_PIN_CNF_SENSE_Pos));
    NRF_GPIO->OUTCLR = (1UL << 5);

    NRF_GPIO->PIN_CNF[22] = ((NRF_GPIO_PIN_DIR_OUTPUT << GPIO_PIN_CNF_DIR_Pos)
                        | (NRF_GPIO_PIN_INPUT_DISCONNECT << GPIO_PIN_CNF_INPUT_Pos)
                        | (NRF_GPIO_PIN_NOPULL << GPIO_PIN_CNF_PULL_Pos)
                        | (NRF_GPIO_PIN_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
                        | (NRF_GPIO_PIN_NOSENSE << GPIO_PIN_CNF_SENSE_Pos));
    NRF_GPIO->OUTCLR = (1UL << 22);

    NRF_GPIO->PIN_CNF[27] = ((NRF_GPIO_PIN_DIR_OUTPUT << GPIO_PIN_CNF_DIR_Pos)
                        | (NRF_GPIO_PIN_INPUT_DISCONNECT << GPIO_PIN_CNF_INPUT_Pos)
                        | (NRF_GPIO_PIN_NOPULL << GPIO_PIN_CNF_PULL_Pos)
                        | (NRF_GPIO_PIN_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
                        | (NRF_GPIO_PIN_NOSENSE << GPIO_PIN_CNF_SENSE_Pos));
    NRF_GPIO->OUTSET = (1UL << 27);
*/

/**@brief Turn I/O pin ON then OFF
*/
void pulse ( uint16_t pin )
{
    nrf_gpio_cfg_output( pin );
    nrf_gpio_pin_set ( pin );
    nrf_delay_us(100);
    nrf_gpio_pin_clear ( pin );
    nrf_gpio_cfg_input_disconnect( pin );
}

/**@brief toggle spare pin
*/
void toggle_spare ( uint16_t pin )
{
    static bool status = 0;

    nrf_gpio_cfg_output( pin );
    if ( status )
    {
      status = false;
      nrf_gpio_pin_clear ( pin );
    }
    else
    {
      status = true;
      nrf_gpio_pin_set ( pin );
    }
}

/**@brief Return the current state of the I/O pin passed
*/
bool read_pin ( uint16_t pin )
{
    bool retval;

    nrf_gpio_cfg_input(pin, NRF_GPIO_PIN_PULLUP);
    nrf_delay_us(50);
    if ( nrf_gpio_pin_read(pin) )
      retval = true;
    else
      retval = false;
    nrf_gpio_cfg_input_disconnect(pin);

    return ( retval );
}

/**@brief reduce value by number of digits
*/
uint8_t adc_round ( uint16_t value, uint8_t digits )
{
    uint8_t divisor = 2;

    if ( digits )
    {
        while ( --digits ) divisor *= 2;
        value = ((value + (divisor/2)) / divisor);
        if (value > 255 ) value = 255;
    }

    return ( value );
}
/** @} */