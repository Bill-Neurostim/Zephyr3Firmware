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
#include "drv_audio.h"
#include "drv_audio_coder.h"
#include "drv_mic.h"

STATIC_ASSERT(CONFIG_PDM_BUFFER_SIZE_SAMPLES == (1 * CONFIG_AUDIO_FRAME_SIZE_SAMPLES));

int8_t  audio_frequency = FLEX_OPTION;

typedef struct
{
    int16_t  lbuf[CONFIG_PDM_BUFFER_SIZE_SAMPLES];    // 256 bytes
    int16_t  rbuf[CONFIG_PDM_BUFFER_SIZE_SAMPLES];    // 256 bytes
    uint16_t samples;
    bool     free;
}pdm_buf_t;

#define PDM_BUF_NUM 6

static bool         m_audio_enabled;          ///< Audio enabled flag.
static pdm_buf_t    m_pdm_buf[PDM_BUF_NUM];   // buffers holding audio data

static bool         audio_left_ready;         // flag indicating left data ready, set in m_audio_buffer_handler
static bool         audio_right_ready;        // flag indicating right data ready, set in m_audio_buffer_handler
static void *       event_data;               // pointer to new data
static uint16_t     event_size;               // size of new data

/****************************************************************************************************/
/* hander called by PDM hardware that saves data
/****************************************************************************************************/
static void m_audio_buffer_handler(int16_t *p_buffer, uint16_t samples)
{
    uint32_t            err_code;
    static uint32_t     pdm_buf_num = 0;
    static uint32_t     pdm_buf_addr = 0;
    static uint32_t     count = 0;
    uint32_t            i, j, r_index, l_index;

    static uint32_t buffer_index  = 0;
    static uint32_t pdm_count = 0;

    if ( pdm_count & 1 )
        spare_on();
    else
        spare_off();
    // if 'new' packet (filling in at buffer_index == 0), find a free buffer
    if ( !buffer_index )
    {
        for( i = 0; i < PDM_BUF_NUM; i++ )
        {
            if ( m_pdm_buf[i].free == true )              // find unused buffer
            {
                m_pdm_buf[i].free    = false;             // set to used
                m_pdm_buf[i].samples = samples;           // set number of samples in buffer

                pdm_buf_num  = i;                         // save buffer number for use
                pdm_buf_addr = (uint32_t)&m_pdm_buf[i];   // save address of buffer
                break;
            }
        }
    }

    if ( pdm_buf_num < PDM_BUF_NUM )
    {
        uint16_t audio_freq_increment;
        switch ( audio_frequency )
        {
            case 0:
                audio_freq_increment = 2;
                break;
            case 1:
                audio_freq_increment = 4;
                break;
            case 2:
                audio_freq_increment = 8;
                break;
        }

        for ( j = 0; j < samples; j+=audio_freq_increment  )
        {
            m_pdm_buf[pdm_buf_num].lbuf[buffer_index] = p_buffer[j+0];   // save left buffer
            m_pdm_buf[pdm_buf_num].rbuf[buffer_index] = p_buffer[j+1];   // save right buffer
            buffer_index++;
        }

        pdm_count++;

        if ( buffer_index >= samples )
        {
            buffer_index = 0;
            audio_left_ready = true;           // set the audio_event_ready flag to tell audio timer routine data is available
            audio_right_ready = true;
            event_data = &pdm_buf_addr;
            event_size = sizeof(pdm_buf_t *);
        }
    }
    else
    {
        buffer_index = 0;
    }
}

/****************************************************************************************************/
/* Code called from audio timer,  takes raw data an encodes it then sets dirty flag for transmission
/****************************************************************************************************/
void m_left_audio_process()
{
    int16_t         * p_buffer;
    pdm_buf_t       * p_pdm_buf = (pdm_buf_t *)(*(uint32_t *)event_data);

    if ( audio_left_ready )
    {
        APP_ERROR_CHECK_BOOL(event_data != NULL);
        APP_ERROR_CHECK_BOOL(event_size > 0);
        p_buffer = p_pdm_buf->lbuf;

        uint8_t nested;
        app_util_critical_region_enter(&nested);
        drv_audio_coder_encode(p_buffer, &left_frame);
        p_pdm_buf->free = true;
        audio_left_ready = false;
        app_util_critical_region_exit(nested);

        channel_dirty [LEFT_AUDIO]  = true;
    }
}

void m_right_audio_process()
{
    int16_t         * p_buffer;
    pdm_buf_t       * p_pdm_buf = (pdm_buf_t *)(*(uint32_t *)event_data);

    if ( audio_right_ready )
    {
        APP_ERROR_CHECK_BOOL(event_data != NULL);
        APP_ERROR_CHECK_BOOL(event_size > 0);
        p_buffer = p_pdm_buf->rbuf;

        uint8_t nested;
        app_util_critical_region_enter(&nested);
        drv_audio_coder_encode(p_buffer, &right_frame);
        p_pdm_buf->free = true;
        audio_right_ready = false;
        app_util_critical_region_exit(nested);

        channel_dirty [RIGHT_AUDIO]  = true;
    }
}

/****************************************************************************************************
/* Generic routines
****************************************************************************************************/

static void mic_power_on(void)
{
    nrf_gpio_cfg_input(MIC_DOUT, NRF_GPIO_PIN_NOPULL);
}

static void mic_power_off(void)
{
    nrf_gpio_cfg_input(MIC_DOUT, NRF_GPIO_PIN_PULLDOWN);
}

uint32_t drv_mic_start(void)
{
    ret_code_t status;

    NRF_LOG_DEBUG("m_audio: Enabled\r\n");

    if(m_audio_enabled == true)
    {
        return NRF_SUCCESS;
    }

    mic_power_on();
    #ifdef SP1_TEST
    nrf_gpio_cfg_output(SP1);
    #endif

    status = drv_audio_enable();
    if (status == NRF_SUCCESS)
    {
        m_audio_enabled = true;
    }

    return status;
}

uint32_t drv_mic_stop(void)
{
    ret_code_t status;

    NRF_LOG_DEBUG("m_audio: Disabled\r\n");

    if(m_audio_enabled == false)
    {
        return NRF_SUCCESS;
    }

    status = drv_audio_disable();
    if (status == NRF_SUCCESS)
    {
        m_audio_enabled = false;
    }

    mic_power_off();

    return status;
}

uint32_t drv_mic_init()
{
    uint32_t err_code;

    m_audio_enabled = false;

    for(uint32_t i = 0; i < PDM_BUF_NUM; i++)
    {
        m_pdm_buf[i].free = true;
    }

    mic_power_off();

    return drv_audio_init(m_audio_buffer_handler);
}