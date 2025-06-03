#include "global.h"
#include "sdk_config.h"

#ifdef TWI
/* TWI instance ID. */
#define TWI_INSTANCE_ID     0

#define TWI_SCL_PIN         19
#define TWI_SDA_PIN         18

#include "SPO2.h"
#include "LIS3DH.h"

/* Indicates if operation on TWI has ended. */
static volatile bool m_xfer_done = false;

static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

static uint8_t sample_size;

uint8_t rx_buffer[512];
uint8_t tx_buffer[10];

uint8_t accel_rx_buffer[10];
uint8_t accel_tx_buffer[10];

ret_code_t err_code;

/**
 * @brief TWI events handler.
 */
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context ) {
  switch ( p_event->type ) 
  {
    case NRF_DRV_TWI_EVT_DONE:
      // Handle event when TWI transfer is done.  e.g. set a flag
      m_xfer_done = true;
      if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_RX)
      {
          if ( (p_event->xfer_desc).address == LIS3DH_ADDR )
            ;//memcpy ( 
      }
      break;
    default:
      break;
  }
}

/**
 * @brief mfio_wait(), wait for MFIO to go low or a timeout (20ms)
 */
void mfio_wait ()
{
  uint8_t timeout = 20;
  while ( nrf_gpio_pin_read(MFIO) && timeout-- )
      nrf_delay_ms( 1 );
}

/**
 * @brief xfer_wait(), wait for I2C completion or a timeout (20ms)
 */
void xfer_wait ()
{
  uint8_t timeout = 20;
  do {                  // wait for handler response or timeout
    nrf_delay_ms( 1 );
  }while ( (m_xfer_done == false) && timeout-- );
}

/**
 * @brief write data to SPO2 sensor
 */
ret_code_t SPO2_write ( uint8_t family_byte, uint8_t index_byte, uint8_t output_type_byte )
{
  #ifdef DEBUG_TEST
  memset ( tx_buffer, 0, sizeof(tx_buffer) );
  #endif
  tx_buffer[0] = family_byte;
  tx_buffer[1] = index_byte;
  tx_buffer[2] = output_type_byte;
 
  m_xfer_done = false;
  err_code = nrf_drv_twi_tx(&m_twi, SPO2_ADDR, tx_buffer, 3, false);
  APP_ERROR_CHECK(err_code);
  xfer_wait();

  switch (family_byte)
  {
      case OUTPUT_MODE:                 // 0x10
        nrf_delay_ms(CMD_DELAY);
        break;
      case ENABLE_ALGORITHM:            // 0x52
      case ENABLE_SENSOR:               // 0x44
        nrf_delay_ms(ALGO_CMD_DELAY);
        break;
  }
  
  #ifdef DEBUG_TEST
  memset ( rx_buffer, 0xAA, sizeof(rx_buffer) );
  #endif
  m_xfer_done = false;
  err_code = nrf_drv_twi_rx(&m_twi, SPO2_ADDR, rx_buffer, 1);
  APP_ERROR_CHECK(err_code);
  xfer_wait();

  return rx_buffer[1];
}

/**
 * @brief read data from SPO2 sensor
 */
ret_code_t SPO2_read ( uint8_t family_byte, uint8_t index_byte )
{
  tx_buffer[0] = family_byte;
  tx_buffer[1] = index_byte;
  
  m_xfer_done = false;
  err_code = nrf_drv_twi_tx(&m_twi, SPO2_ADDR, tx_buffer, 2, false);
  APP_ERROR_CHECK(err_code);
  xfer_wait();

  nrf_delay_ms(CMD_DELAY);

  #ifdef DEBUG_TEST
  memset ( rx_buffer, 0xAA, sizeof(rx_buffer));
  #endif
  m_xfer_done = false;
  err_code = nrf_drv_twi_rx(&m_twi, SPO2_ADDR, rx_buffer, 2);
  APP_ERROR_CHECK(err_code);
  xfer_wait();

  return ( err_code );
}

/**
 * @brief read data from SPO2 sensor
 */
ret_code_t SPO2_read_long ( uint8_t family_byte, uint8_t index_byte, uint8_t data_byte, uint8_t tx_size, uint16_t rx_size )
{
  bool flag = false;
  if ( rx_size == (MAXFAST_ARRAY_SIZE + 1) ) flag = true;

  tx_buffer[0] = family_byte;
  tx_buffer[1] = index_byte;
  tx_buffer[2] = data_byte;

  m_xfer_done = false;
  err_code = nrf_drv_twi_tx(&m_twi, SPO2_ADDR, tx_buffer, tx_size, false);
  APP_ERROR_CHECK(err_code);
  xfer_wait();

  nrf_delay_ms(CMD_DELAY);
  
  mfio_wait();

  #ifdef DEBUG_TEST
  memset ( rx_buffer, 0xAA, sizeof(rx_buffer)); 
  #endif
  m_xfer_done = false;
  err_code = nrf_drv_twi_rx(&m_twi, SPO2_ADDR, rx_buffer, rx_size);
  APP_ERROR_CHECK(err_code);
  xfer_wait();

  return ( err_code );
}

/**
 * @brief write data to LIS3DH sensor
 */
ret_code_t LIS3DH_write ( uint8_t _register, uint8_t data )
{
  accel_tx_buffer[0] = _register;
  accel_tx_buffer[1] = data;

  m_xfer_done = false;
  err_code = nrf_drv_twi_tx(&m_twi, LIS3DH_ADDR, accel_tx_buffer, 2, false);
  APP_ERROR_CHECK(err_code);
  xfer_wait();
  
  return ( err_code );
}

/**
 * @brief read data from LIS3DH sensor
 */
ret_code_t LIS3DH_read ( uint8_t _register, uint8_t rx_size )
{
  if ( rx_size > 1 ) _register |= 0x80;   // if more than 1 bytes, set auto-increment
  accel_tx_buffer[0] = _register;

  m_xfer_done = false;
  err_code = nrf_drv_twi_tx(&m_twi, LIS3DH_ADDR, accel_tx_buffer, 1, false);
  APP_ERROR_CHECK(err_code);
  xfer_wait();

  m_xfer_done = false;
  err_code = nrf_drv_twi_rx(&m_twi, LIS3DH_ADDR, accel_rx_buffer, rx_size);
  APP_ERROR_CHECK(err_code);
  xfer_wait();
  
  return ( err_code );
}

/**
 * @brief TWI initialization.
 */
void twi_init(void) 
{
  const nrf_drv_twi_config_t twi_config = {
    .scl                = TWI_SCL_PIN,
    .sda                = TWI_SDA_PIN,
    .frequency          = NRF_DRV_TWI_FREQ_100K,
    .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
    .hold_bus_uninit    = false
  };

  err_code = nrf_drv_twi_init ( &m_twi, &twi_config, twi_handler, NULL );
  APP_ERROR_CHECK(err_code);

  nrf_drv_twi_enable(&m_twi);                       // enable I2C port
}

/**
 * @brief initialize sensor hub.
 */
void spo2_init(void) 
{
  // Initialize and configure sensor hub
  nrf_gpio_cfg_output(RST);                         // set reset pin as output
  nrf_gpio_cfg_output(PROG);                        // set program pin as output
  nrf_gpio_cfg_input(MFIO, NRF_GPIO_PIN_NOPULL);    // set mfio as input

  nrf_gpio_pin_clear(RST);                          // set reset low
  nrf_delay_ms(10);                                 // delay 10ms
  nrf_gpio_pin_set(RST);                            // set reset high
  nrf_delay_ms(1000);                               // delay 1 second

  err_code = 0;
  // Read Device          0x02, 0x00
  SPO2_read ( READ_DEVICE_MODE, 0x00 );
  err_code = rx_buffer[0];

  // Set output mode            0x10, 0x00, 0x02 - Set output mode - Algorithm
  SPO2_write ( OUTPUT_MODE, SET_FORMAT, ALGO_DATA );
  err_code = rx_buffer[0];
  #ifdef DEBUG_TEST
  // Read output mode           0x11, 0x00 - Read output mode
  SPO2_read ( READ_OUTPUT_MODE, SET_FORMAT ); // verify I2C readback
  err_code = rx_buffer[0];
  #endif

  // Set FIFO threshold         0x10, 0x01, 0x01 - Set the threshold for the FIFO interrupt (1)
  SPO2_write ( OUTPUT_MODE, WRITE_SET_THRESHOLD, 1 );
  err_code = rx_buffer[0];
  #ifdef DEBUG_TEST
  // Read FIFO threshold        0x11, 0x01 - Read the threshold for the FIFO interrupt (1)
  SPO2_read ( READ_OUTPUT_MODE, WRITE_SET_THRESHOLD ); // verify I2C readback
  err_code = rx_buffer[0];
  #endif
  
  // Enable Algorithm Control   0x52, 0x00, 0x01 - enable the AGC algorithm
  SPO2_write ( ENABLE_ALGORITHM, ENABLE_AGC_ALGO, ENABLE );
  err_code = rx_buffer[0];

  // Enable MAX30101            0x44, 0x03, 0x01 - 
  SPO2_write ( ENABLE_SENSOR, ENABLE_MAX30101, ENABLE );
  err_code = rx_buffer[0];
  #ifdef DEBUG_TEST
  // Read Enable MAX30101       0x45, 0x03
  SPO2_read  ( READ_SENSOR_MODE, ENABLE_MAX30101 );
  err_code = rx_buffer[0];
  #endif

  // Enable MAX Fast Algorithm  0x52, 0x02, 0x01
  SPO2_write ( ENABLE_ALGORITHM, ENABLE_WHRM_ALGO, ENABLE );
  err_code = rx_buffer[0];

  // Get number of samples      0x51, 0x00, 0x03 - read number of samples to average
  SPO2_read_long ( READ_ALG_CONFIG, READ_AGC_NUM_SAMPLES, READ_AGC_NUM_SAMPLES_ID, 3, 2 );
  err_code = rx_buffer[0];

  //nrf_delay_ms(5000);                               // delay 5 second

  // Read output mode           0x11, 0x00 - Read output mode
  SPO2_read ( READ_OUTPUT_MODE, SET_FORMAT ); // verify I2C readback
  err_code = rx_buffer[0];
  // Read FIFO threshold        0x11, 0x01 - Read the threshold for the FIFO interrupt (1)
  SPO2_read ( READ_OUTPUT_MODE, WRITE_SET_THRESHOLD ); // verify I2C readback
  err_code = rx_buffer[0];
  // Read Enable MAX30101       0x45, 0x03
  SPO2_read  ( READ_SENSOR_MODE, ENABLE_MAX30101 );
  err_code = rx_buffer[0];
  // Read firmware version
  SPO2_read_long ( 0xFF, 0x03, 0x00, 2, 4 );
  err_code = rx_buffer[0];
  if ( rx_buffer[1] == 10 )
    sample_size = 9;
  else
    sample_size = 6; 

  // Initialize and configure accelerometer
  nrf_gpio_cfg_output(CS);                          // set chip select pin as output
  nrf_gpio_cfg_output(SDO);                         // set SDO pin as output
  nrf_gpio_cfg_input(INT1, NRF_GPIO_PIN_PULLUP);    // set INT1 as input with pullup
  nrf_gpio_cfg_input(INT2, NRF_GPIO_PIN_PULLUP);    // set INT2 as input with pullup

  nrf_gpio_pin_clear(SDO);                          // set SDO low - I2C address 0x18
  nrf_gpio_pin_set(CS);                             // set CS high - enable chip

  nrf_delay_ms(10);                                 // delay 10ms
}

/**
 * @brief de-initialize sensor hub.
 */
void spo2_deinit (void)
{
  nrf_gpio_cfg_output(RST);                         // set reset pin as output
  nrf_gpio_cfg_output(PROG);                        // set program pin as output
  nrf_gpio_cfg_input(MFIO, NRF_GPIO_PIN_NOPULL);    // set mfio as input

  nrf_gpio_pin_clear(RST);                          // set reset low
  nrf_delay_ms(10);                                 // delay 10ms
  nrf_gpio_pin_set(RST);                            // set reset high
  nrf_gpio_cfg_input(MFIO, NRF_GPIO_PIN_NOPULL);    // set mfio as input with pullup (hardware)
}

/**
 * @brief initialize acceleromenter.
 */
void accel_init(void) 
{
  // Initialize and configure accelerometer
  nrf_gpio_cfg_output(CS);                          // set chip select pin as output
  nrf_gpio_cfg_output(SDO);                         // set SDO pin as output
  nrf_gpio_cfg_input(INT1, NRF_GPIO_PIN_PULLUP);    // set INT1 as input with pullup
  nrf_gpio_cfg_input(INT2, NRF_GPIO_PIN_PULLUP);    // set INT2 as input with pullup

  nrf_gpio_pin_clear(SDO);                          // set SDO low - I2C address 0x18
  nrf_gpio_pin_set(CS);                             // set CS high - enable chip

  nrf_delay_ms(10);                                 // delay 10ms

  // read if accelerometer is ok
  LIS3DH_read ( LIS3DH_WHO_AM_I, 1 );
  if ( accel_rx_buffer[0] != 0x33 ) 
    err_code = 1;

  // enable temp and adc
  LIS3DH_write ( LIS3DH_TEMP_CFG_REG, 0xC0 );
  //LIS3DH_read ( LIS3DH_TEMP_CFG_REG, 1 );

  // set sample rate (50), enable x, y, z axis
  LIS3DH_write ( LIS3DH_CTRL_REG1, 0x47 );
  //LIS3DH_read ( LIS3DH_CTRL_REG1, 1 );

  // Set block update, high resolution
  LIS3DH_write ( LIS3DH_CTRL_REG4, 0x88 );
  //LIS3DH_read ( LIS3DH_CTRL_REG4, 1 );

  return;
}

/**
 * @brief de-initialize acceleromenter.
 */
void accel_deinit ( void )
{
  nrf_gpio_cfg_output(CS);                          // set chip select pin as output
  nrf_gpio_cfg_output(SDO);                         // set SDO pin as output
  nrf_gpio_cfg_input(INT1, NRF_GPIO_PIN_PULLUP);    // set INT1 as input with pullup
  nrf_gpio_cfg_input(INT2, NRF_GPIO_PIN_PULLUP);    // set INT2 as input with pullup

  nrf_gpio_pin_clear(SDO);                          // set SDO low - I2C address 0x18
  nrf_gpio_pin_clear(CS);                           // set CS lowh - disable chip
}

//int8_t iz;
/**
 * @brief Read SPO2 sensor data.
 */
uint8_t readBPM ()
{
  uint8_t num_samples;
  uint8_t status;
  uint8_t hr_msb, hr_lsb;
  uint8_t conf;
  uint8_t oxy_msb, oxy_lsb;
  uint8_t finger;
  uint8_t ix;
  uint8_t *ptr;

  SPO2_read ( HUB_STATUS, 0x00 );
  err_code = rx_buffer[0];

  if ( rx_buffer[1] == 1 )
    {
      memset ( spo2_sensor_data, 0x00, sizeof(spo2_sensor_data) );
      return 0;
    }

  SPO2_read ( READ_DATA_OUTPUT, NUM_SAMPLES );
  err_code = rx_buffer[0];
  num_samples =  rx_buffer[1];
  NRF_LOG_DEBUG("readBPM: %u samples, %u seconds ", num_samples, seconds );

  SPO2_read_long ( READ_DATA_OUTPUT, READ_DATA, 0x00, 2, (num_samples*sample_size)+1 );

  ptr = rx_buffer+1;
  for ( ix = 0; ix < num_samples; ix++ )
  {
  finger = ptr[5];
  if ( finger == 3 )
    memcpy ( spo2_sensor_data + 4, ptr, sample_size );
  ptr += sample_size;
  }

  return 1;
}

/**
 * @brief Read SPO2 sensor data.
 */
uint8_t readAccel ()
{
  memset ( three_axis_acc_data, 0, sizeof(three_axis_acc_data));

  LIS3DH_read ( LIS3DH_OUT_X_L, 2 );
  memcpy ( three_axis_acc_data + 4, accel_rx_buffer, 2 );

  LIS3DH_read ( LIS3DH_OUT_Y_L, 2 );
  memcpy ( three_axis_acc_data + 6, accel_rx_buffer, 2 );

  LIS3DH_read ( LIS3DH_OUT_Z_L, 2 );
  memcpy ( three_axis_acc_data + 8, accel_rx_buffer, 2 );
}
#endif