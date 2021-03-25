/**
 * Copyright (c) 2014 - 2017, Nordic Semiconductor ASA
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 * 
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 * 
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 * 
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include <stdbool.h>
#include <stdint.h>
#include <nrfx_saadc.h>
#include "nrfx_timer.h"
#include "nrfx_ppi.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


#define ADC_CHANNELS_IN_USE     6   // Note: If changed, the logging during the NRFX_SAADC_EVT_DONE must be updated.
#define SAADC_BUF_SIZE          ADC_CHANNELS_IN_USE
#define SAADC_BUF_COUNT         2
#define SAADC_SAMPLE_FREQUENCY  8000

static nrf_saadc_value_t samples[SAADC_BUF_COUNT][SAADC_BUF_SIZE];
static const nrfx_timer_t m_sample_timer = NRFX_TIMER_INSTANCE(1);
static nrf_ppi_channel_t m_timer_saadc_ppi_channel;
static nrf_ppi_channel_t m_saadc_internal_ppi_channel;
static const uint32_t saadc_sampling_rate = 1000; // milliseconds (ms)

static const nrf_saadc_input_t ANALOG_INPUT_MAP[NRF_SAADC_CHANNEL_COUNT] = {
    NRF_SAADC_INPUT_AIN0, NRF_SAADC_INPUT_AIN1, NRF_SAADC_INPUT_AIN2, NRF_SAADC_INPUT_AIN3,
    NRF_SAADC_INPUT_AIN4, NRF_SAADC_INPUT_AIN5, NRF_SAADC_INPUT_AIN6, NRF_SAADC_INPUT_AIN7};


// Simple function to provide an index to the next input buffer
// Will simply alernate between 0 and 1 when SAADC_BUF_COUNT is 2
static uint32_t next_free_buf_index(void)
{
    static uint32_t buffer_index = -1;
    buffer_index = (buffer_index + 1) % SAADC_BUF_COUNT;
    return buffer_index;
}
 

static void timer_handler(nrf_timer_event_t event_type, void * p_context)
{
}


static void event_handler(nrfx_saadc_evt_t const * p_event)
{
    ret_code_t err_code;
    switch (p_event->type)
    {
        case NRFX_SAADC_EVT_DONE:
            NRF_LOG_INFO("ADC Values: %6d %6d %6d %6d %6d %6d",
                p_event->data.done.p_buffer[0], p_event->data.done.p_buffer[1], p_event->data.done.p_buffer[2], p_event->data.done.p_buffer[3], p_event->data.done.p_buffer[4], p_event->data.done.p_buffer[5]);
            break;

        case NRFX_SAADC_EVT_BUF_REQ:
            // Set up the next available buffer
            err_code = nrfx_saadc_buffer_set(&samples[next_free_buf_index()][0], SAADC_BUF_SIZE);
            APP_ERROR_CHECK(err_code);
            break;
                default:
            NRF_LOG_INFO("SAADC evt %d", p_event->type);
            break;
    }
}


static void timer_init(void)
{
    nrfx_err_t err_code;

    nrfx_timer_config_t timer_config = NRFX_TIMER_DEFAULT_CONFIG;
    timer_config.frequency = NRF_TIMER_FREQ_31250Hz;
    err_code = nrfx_timer_init(&m_sample_timer, &timer_config, timer_handler);
    APP_ERROR_CHECK(err_code);
    nrfx_timer_extended_compare(&m_sample_timer,
                                NRF_TIMER_CC_CHANNEL0,
                                nrfx_timer_ms_to_ticks(&m_sample_timer, saadc_sampling_rate),
                                NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK,
                                false);

    nrfx_timer_resume(&m_sample_timer);
}



static void ppi_init(void)
{
    // Trigger task sample from timer
    nrfx_err_t err_code = nrfx_ppi_channel_alloc(&m_timer_saadc_ppi_channel);
    APP_ERROR_CHECK(err_code);
    err_code = nrfx_ppi_channel_assign(m_timer_saadc_ppi_channel, 
                                       nrfx_timer_event_address_get(&m_sample_timer, NRF_TIMER_EVENT_COMPARE0),
                                       nrf_saadc_task_address_get(NRF_SAADC_TASK_SAMPLE));
    APP_ERROR_CHECK(err_code);

    err_code = nrfx_ppi_channel_enable(m_timer_saadc_ppi_channel);
    APP_ERROR_CHECK(err_code);
}


static void adc_configure(void)
{
    ret_code_t err_code;

    nrfx_saadc_adv_config_t saadc_adv_config = NRFX_SAADC_DEFAULT_ADV_CONFIG;
    saadc_adv_config.internal_timer_cc = 0;
    saadc_adv_config.start_on_end = true;

    err_code = nrfx_saadc_init(NRFX_SAADC_CONFIG_IRQ_PRIORITY);
    APP_ERROR_CHECK(err_code);

    static nrfx_saadc_channel_t channel_configs[ADC_CHANNELS_IN_USE];

    uint8_t channel_mask = 0;
    for(int i = 0; i < ADC_CHANNELS_IN_USE; i++) {
        nrf_saadc_input_t pin = ANALOG_INPUT_MAP[i];
        // Apply default config to each channel
        nrfx_saadc_channel_t config = NRFX_SAADC_DEFAULT_CHANNEL_SE(pin, i);

        // Replace some parameters in default config
        config.channel_config.reference = NRF_SAADC_REFERENCE_VDD4;          
        config.channel_config.gain = NRF_SAADC_GAIN1_4;

        // Copy to list of channel configs
        memcpy(&channel_configs[i], &config, sizeof(config));

        // Update channel mask
        channel_mask |= 1 << i;
    }

    err_code = nrfx_saadc_channels_config(channel_configs, ADC_CHANNELS_IN_USE);
    APP_ERROR_CHECK(err_code);

    err_code = nrfx_saadc_advanced_mode_set(channel_mask,
                                            NRF_SAADC_RESOLUTION_14BIT,
                                            &saadc_adv_config,
                                            event_handler);
    APP_ERROR_CHECK(err_code);
                                            
    // Configure two buffers to ensure double buffering of samples, to avoid data loss when the sampling frequency is high
    err_code = nrfx_saadc_buffer_set(&samples[next_free_buf_index()][0], SAADC_BUF_SIZE);
    APP_ERROR_CHECK(err_code);

    err_code = nrfx_saadc_buffer_set(&samples[next_free_buf_index()][0], SAADC_BUF_SIZE);
    APP_ERROR_CHECK(err_code);

    err_code = nrfx_saadc_mode_trigger();
    APP_ERROR_CHECK(err_code);
}


int main(void)
{
    ret_code_t err_code;

    // Configure Logging. LOGGING is used to show the SAADC sampled result.
    err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);                       
    NRF_LOG_DEFAULT_BACKENDS_INIT();
    NRF_LOG_INFO("nrfx_saadc_api2 simple SAADC Continuous Sampling Example using timer and PPI.");	

    adc_configure();
    ppi_init();
    timer_init();

    while (1)
    {
        while(NRF_LOG_PROCESS() != NRF_SUCCESS);
        __WFE();
    }  
}
