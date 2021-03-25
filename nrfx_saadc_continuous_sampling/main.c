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
/** 
 * Perhipheral: nRF52 SAADC
 * Compatibility: nRF52832/nRF52833/nRF52840, nRF5 SDK 17.0.0
 * Softdevice used: No softdevice
 *
 * This example uses the internal timer feature of the SAADC to trigger sampling at a fixed sample rate, 
 * as set by the SAADC_SAMPLE_FREQUENCY define. 
 *
 * Please note that this feature only supports a limited range of sampling frequencies, and for sampling at frequencies
 * below 8kHz it is necessary to use a dedicated timer as shown in some of the other examples. 
 *
 * The example samples on a single input pin, AIN0, which maps to physical pin P0.02 on the nRF52832/nRF52840 ICs.
 */

#include <stdbool.h>
#include <stdint.h>
#include <nrfx_saadc.h>
#include "nrf_delay.h"
#include "nrf_drv_clock.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
 
#define SAADC_BUF_SIZE         1024
#define SAADC_BUF_COUNT        2
#define SAADC_SAMPLE_FREQUENCY 8000

static nrf_saadc_value_t samples[SAADC_BUF_COUNT][SAADC_BUF_SIZE];
static nrfx_saadc_channel_t channel_config = NRFX_SAADC_DEFAULT_CHANNEL_SE(NRF_SAADC_INPUT_AIN0, 0);


// Simple function to provide an index to the next input buffer
// Will simply alernate between 0 and 1 when SAADC_BUF_COUNT is 2
static uint32_t next_free_buf_index(void)
{
    static uint32_t buffer_index = -1;
    buffer_index = (buffer_index + 1) % SAADC_BUF_COUNT;
    return buffer_index;
}
 

static void event_handler(nrfx_saadc_evt_t const * p_event)
{
    ret_code_t err_code;
    switch (p_event->type)
    {
        case NRFX_SAADC_EVT_DONE:
            NRF_LOG_INFO("DONE. Sample[0] = %i", p_event->data.done.p_buffer[0]);

            // Add code here to process the input
            // If the processing is time consuming execution should be deferred to the main context
            break;

        case NRFX_SAADC_EVT_BUF_REQ:
            // Set up the next available buffer
            err_code = nrfx_saadc_buffer_set(&samples[next_free_buf_index()][0], SAADC_BUF_SIZE);
            APP_ERROR_CHECK(err_code);
            break;
    }
}


static void adc_start(uint32_t cc_value)
{
    ret_code_t err_code;

    nrfx_saadc_adv_config_t saadc_adv_config = NRFX_SAADC_DEFAULT_ADV_CONFIG;
    saadc_adv_config.internal_timer_cc = cc_value;
    saadc_adv_config.start_on_end = true;

    err_code = nrfx_saadc_advanced_mode_set((1<<0),
                                            NRF_SAADC_RESOLUTION_10BIT,
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

    // Configure Logging. LOGGING is used to show the SAADC sampled result. Default is UART, but RTT can be configured in sdk_config.h
    err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);                       
    NRF_LOG_DEFAULT_BACKENDS_INIT();
    NRF_LOG_INFO("nrfx_saadc_api2 simple SAADC Continuous Sampling Example.");	

    err_code = nrfx_saadc_init(NRFX_SAADC_CONFIG_IRQ_PRIORITY);
    APP_ERROR_CHECK(err_code);
 
    err_code = nrfx_saadc_channels_config(&channel_config, 1);
    APP_ERROR_CHECK(err_code);

    uint32_t adc_cc_value = 16000000 / SAADC_SAMPLE_FREQUENCY;
    if(adc_cc_value < 80 || adc_cc_value > 2047)
    {
        NRF_LOG_ERROR("SAMPLERATE frequency outside legal range. Consider using a timer to trigger the ADC instead.");
        APP_ERROR_CHECK(false);
    }
    adc_start(adc_cc_value);

    while (1)
    {
        while(NRF_LOG_PROCESS() != NRF_SUCCESS);
        __WFE();
    }  
}
