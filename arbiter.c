/**@brief Copyright 2020 Neurostim Technologies LLC
*/
#include "global.h"
#include "app_timer.h"
#include "string.h"

APP_TIMER_DEF(arbiter_timer);

/**
 * @brief Initialize arbiter timer for software interrupts
 */
void init_arbiter_timer()
{
    uint32_t err_code;

    err_code = app_timer_create(&arbiter_timer, APP_TIMER_MODE_REPEATED, arbiter_timer_handler);
    APP_ERROR_CHECK(err_code);
}

/**
 * @brief Re-start arbiter timer for software interrupts
 */
void restart_arbiter_timer()
{
    stop_arbiter_timer();
    app_timer_start(arbiter_timer,APP_TIMER_TICKS(GET_ARBITER_TIMER),NULL);
}

/**
 * @brief Stop arbiter timer for software interrupts
 */
void stop_arbiter_timer()
{
    app_timer_stop(arbiter_timer);
}

void set_arbiter_timer ( uint8_t new_time )
{
    SCR[STREAM_STATUS][2] &= ~ARBITER_TIME_BITS;
    SCR[STREAM_STATUS][2] |= (new_time << ARBITER_TIME_SHIFT);
}

static uint32_t arbiter_count;

/**@brief arbiter timer handler.
 */
void arbiter_timer_handler(void * p_context)
{
    uint8_t   channel;
    uint8_t   num_packets;
    uint8_t   *buffer;
    uint8_t   new_channel;
    uint8_t   new_priority;
    bool      active;
    uint8_t   ix;

    //toggle_spare ( SPARE3 );

    if ( notify_flag == true )
    {
        // Loop for the number of packets to transmit per arbiter call
        // Currently ARBITER_NUM_PKT_DEFAULT in Stream_control.h (value is V114 is 1)
        for ( num_packets = 0; num_packets < GET_ARBITER_NUM_PACKET; num_packets++ )
        {
            new_priority = 0;
            new_channel  = 0;
            // find highest rated priority channel
            for ( channel = RFU_0; channel <= FILLER; channel++ )         // loop thru all channels
            {
                active = (bool)GET_CHANNEL_ACTIVE(channel);
                if ( active == true )                                     // if channel is active
                {
                    if ( channel_dirty[channel] == true )
                    {
                        if ( GET_CHANNEL_PRIORITY(channel) > new_priority )   // see if the channel has higher priority than previous stored
                        {
                            new_channel = channel;                            // if higher priority, save channel, priority
                            new_priority = GET_CHANNEL_PRIORITY(channel);
                        }
                    }
                }
            }
            // if we found a channel, transmit it
            if ( new_priority > 0 )                                   // if we found a channel that needs to be transmitted
            {
                switch ( new_channel )                                // go to channel
                {
                    case LEFT_AUDIO:
                        ble_stream[0] = GET_PACKET_HEADER(LEFT_AUDIO);
                        if ( GET_AUDIO_PCM_SOURCE(LEFT_AUDIO) == 0 )
                        {
                            memcpy ( &ble_stream[1],                  // copy left audio buffer into ble_stream, starting at index 1
                                     &left_frame,                     // 
                                     STREAM_DATA_SIZE );              // transfer STREAM_DATA_SIZE of data (131)
                        }
                        else
                        {
                            ble_stream[1] = 0;
                            ble_stream[2] = 0;
                            ble_stream[3] = 0x80;
                            generate_tone( GET_AUDIO_PCM_SOURCE(LEFT_AUDIO),
                                           GET_CHANNEL_DECIMATION(LEFT_AUDIO),
                                           &ble_stream[4] ); 
                        }
                        transmit_stream ();
                        increment_packet ( LEFT_AUDIO );
                        channel_dirty[LEFT_AUDIO] = false;
                        break;
                    case RIGHT_AUDIO:
                        ble_stream[0] = GET_PACKET_HEADER(RIGHT_AUDIO);
                        if ( GET_AUDIO_PCM_SOURCE(RIGHT_AUDIO) == 0 )
                        {
                            memcpy ( &ble_stream[1],                  // copy right audio buffer into ble_stream, starting at index 1
                                     &right_frame,                    // 
                                     STREAM_DATA_SIZE );              // transfer STREAM_DATA_SIZE of data (131)
                        }
                        else
                        {
                            ble_stream[1] = 0;
                            ble_stream[2] = 0;
                            ble_stream[3] = 0x80;
                            generate_tone( GET_AUDIO_PCM_SOURCE(RIGHT_AUDIO),
                                           GET_CHANNEL_DECIMATION(RIGHT_AUDIO),
                                           &ble_stream[4] ); 
                        }
                        transmit_stream ();
                        increment_packet ( RIGHT_AUDIO );
                        channel_dirty[RIGHT_AUDIO] = false;
                        break;
                    case THREE_AXIS_ACC:
                        ble_stream[0] = GET_PACKET_HEADER(THREE_AXIS_ACC);
                        memcpy ( &ble_stream[1],                // copy three_axis_acc_data into ble_stream, starting at index 1
                                 &three_axis_acc_data,          // 
                                 THREE_AXIS_ACC_SIZE );         // transfer THREE_AXIS_ACC_SIZE of data (10)
                        transmit_stream ();
                        increment_packet ( THREE_AXIS_ACC );
                        channel_dirty[THREE_AXIS_ACC] = false;
                        break;
                    case SPO2_SENSOR:
                        ble_stream[0] = GET_PACKET_HEADER(SPO2_SENSOR);
                        memcpy ( &ble_stream[1],                // copy spo2_sensor_data into ble_stream, starting at index 1
                                 &spo2_sensor_data,             // 
                                 SPO2_SENSOR_SIZE );            // transfer SPO2_SENSOR_SIZE_SIZE of data (10)
                        transmit_stream ();
                        increment_packet ( SPO2_SENSOR );
                        channel_dirty[SPO2_SENSOR] = false;
                        memset ( spo2_sensor_data, 0 , sizeof(spo2_sensor_data) );  // clear data buffer
                        break;
                    case RESULTS_VECTOR:
                        if ( SCR[RESULTS_VECTOR][4] != 0)                           // if packets to transmit
                        {
                            ble_stream[0] = GET_PACKET_HEADER(RESULTS_VECTOR);      // Put header into ble_stream[0]
                            memcpy ( &ble_stream[1],                                // copy results vector int ble_stream, starting at index 1
                                     &results_vector[SCR[RESULTS_VECTOR][3]],       // index into results vector baed on SCR[RV][3]
                                     STREAM_DATA_SIZE );                            // transfer STREAM_DATA_SIZE of data (131)

                            SCR[RESULTS_VECTOR][3] += STREAM_DATA_SIZE;             // increment pointer
                            SCR[RESULTS_VECTOR][3] %= RESULTS_SIZE;                 // make sure no wrap overs
                            SCR[RESULTS_VECTOR][4]--;                               // decrement packet count

                            #ifdef TEST_DEBUG
                            test_tx_count++;
                            #endif
                            transmit_stream ();                                     // transmit data
                            increment_packet ( RESULTS_VECTOR );                    // increment sequence number

                            if ( SCR[RESULTS_VECTOR][4] == 0 )                      // if we are done sending
                                channel_dirty[RESULTS_VECTOR] = false;              // clear dirty bit
                        }
                        break;
                }
            }
        }
    }
}

void increment_packet ( uint8_t channel )
{
    SCR[channel][SCR_PACKET_HEADER]++;
}

void generate_pattern ( uint16_t seed, uint16_t length, uint8_t *buffer, uint8_t size )
{
    uint16_t index;
    uint16_t Pattern;

    Pattern = seed;
    for ( index = 0; index < length; index++ )
    {
        Pattern = (Pattern << 1) + ((Pattern >> 15)^(Pattern & 1));
        switch ( size )
        {
            case 1:
                ((uint8_t *) buffer)[index] = Pattern;
                break;
            case 2:
                ((uint16_t *)buffer)[index] = Pattern;
                break;
            case 4:
                ((uint32_t *)buffer)[index] = Pattern;
                break;
        }
    }
}

uint16_t SinTable[] = 
{
37576, 40730, 43807, 46778, 49614, 52288, 54774, 57047,
59088, 60874, 62390, 63621, 64554, 65181, 65497, 65497,
65181, 64554, 63621, 62390, 60874, 59088, 57047, 54774,
52288, 49614, 46778, 43807, 40730, 37576, 34376, 31160,
27960, 24806, 21729, 18758, 15922, 13248, 10762,  8489,
 6448,  4662,  3146,  1915,   982,   355,    39,    39,
  355,   982,  1915,  3146,  4662,  6448,  8489, 10762,
13248, 15922, 18758, 21729, 24806, 27960, 31160, 34376
};

void generate_tone ( uint8_t Freq, uint8_t Decimation, uint8_t *buffer )
{
    // The assumption must be that we are interrupting at 16ms
    // The SinTable defines 64 samples in one cycle
    // For  125 hz, the code will create         one period of 64 16 bit samples.  NumSamples: 64
    // For  250 hz, the code will create        two periods of 32 16 bit samples.  NumSamples: 32
    // For  500 hz, the code will create       four periods of 16 16 bit samples.  NumSamples: 16
    // For 1000 hz, the code will create      eight periods of  8 16 bit samples.  NumSamples: 8
    // For 2000 hz, the code will create    sixteen periods of  4 16 bit samples.  NumSamples: 4
    // For 4000 hz, the code will create thirty-two periods of  2 16 bit samples.  NumSamples: 2
    // For 8000 hz, the code will create sixty-four periods of  1 16 bit samples.  NumSamples: 1 - ALWAYS THE SAME!
    uint16_t  NumSamples;
    uint16_t  PeriodCtr;
    uint16_t  SampleCtr;

    Freq--;
    NumSamples = 64 >> (Freq + Decimation);         // Number of Samples in SinTable for a Period

    for ( PeriodCtr = 0; PeriodCtr < 64/NumSamples; PeriodCtr++ )   // Packet contains 1-32 Periods
    {
        for ( SampleCtr = 0; SampleCtr < NumSamples; SampleCtr++ )
        {
            buffer[2*(PeriodCtr * NumSamples + SampleCtr) + 0] = SinTable[SampleCtr * 64/NumSamples] >> 8;
            buffer[2*(PeriodCtr * NumSamples + SampleCtr) + 1] = SinTable[SampleCtr * 64/NumSamples] & 0xFF;
        }
    }
}
/** @} */
