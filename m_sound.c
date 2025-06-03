/*
  Copyright (c) 2010 - 2017, Nordic Semiconductor ASA
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

  2. Redistributions in binary form, except as embedded into a Nordic
     Semiconductor ASA integrated circuit in a product or a software update for
     such product, must reproduce the above copyright notice, this list of
     conditions and the following disclaimer in the documentation and/or other
     materials provided with the distribution.

  3. Neither the name of Nordic Semiconductor ASA nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

  4. This software, with or without modification, must only be used with a
     Nordic Semiconductor ASA integrated circuit.

  5. Any software provided in binary form under this license must not be reverse
     engineered, decompiled, modified and/or disassembled.

  THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
  OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "..\global.h"
#include "m_sound.h"
#include "drv_mic.h"

extern uint8_t   audio_frequency;
extern uint32_t right_tx_count;
extern uint32_t right_error_count;
extern uint32_t left_tx_count;
extern uint32_t left_error_count;

static uint32_t drv_left_mic_data_handler(m_audio_frame_t * p_frame)
{
    uint32_t err_code;

    #ifdef SP1_TEST
    static uint32_t count = 0;
    count++;
    if ( count & 1 )
        nrf_gpio_pin_set(SP1);
    else
        nrf_gpio_pin_clear(SP1);
    #endif

    if (p_frame->data_size != CONFIG_AUDIO_FRAME_SIZE_BYTES)
    {
        NRF_LOG_WARNING("drv_mic_data_handler: size = %d \r\n", p_frame->data_size);
    }

    left_tx_count++;
    channel_dirty [LEFT_AUDIO] = true;
//DEBUG   err_code = ble_tss_mic_set(&m_tss, p_frame->data, p_frame->data_size);
    if (err_code != NRF_SUCCESS)
    {
        left_error_count++;
        NRF_LOG_ERROR("drv_mic_data_handler: Failed to send\r\n");
    }
    return NRF_SUCCESS;
}

static uint32_t drv_right_mic_data_handler(m_audio_frame_t * p_frame)
{
    uint32_t err_code;

    if (p_frame->data_size != CONFIG_AUDIO_FRAME_SIZE_BYTES)
    {
        NRF_LOG_WARNING("drv_right_mic_data_handler: size = %d \r\n", p_frame->data_size);
    }

    right_tx_count++;
    channel_dirty [RIGHT_AUDIO] = true;
//DEBUG    err_code = ble_tss_right_mic_set(&m_tss, p_frame->data, p_frame->data_size);
    if (err_code != NRF_SUCCESS)
    {
        right_error_count++;
        NRF_LOG_ERROR("drv_right_mic_data_handler: Failed to send\r\n");
    }
    return NRF_SUCCESS;
}

uint32_t m_sound_init( )
{
    return drv_mic_init();
}
