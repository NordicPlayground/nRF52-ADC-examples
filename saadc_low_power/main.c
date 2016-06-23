/* Copyright (c) 2016 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** 
 * Perhipheral: nRF52 SAADC
 * Compatibility: nRF52 rev 1, nRF5 SDK 11.0.0
 * Softdevice used: No softdevice
 *
 * This example enables the RTC timer to periodically trigger SAADC sampling. RTC is chosen here instead of 
 * TIMER because it is low power. The example samples on a single input pin, the AIN0, which maps to physical pin P0.02 on the nRF52832 IC.
 * This SAADC example shows the following features:
 * - Low Power -> Enabled with initializing SAADC when sampling and uninitializing when sampling is complete.
 *                Low power can only be obtained when UART_PRINTING_ENABLED is not defined and
 *                SAADC_SAMPLES_IN_BUFFER is 1
 * - Oversampling -> This reduces SAADC noise level, especially for higher SAADC resolutions, see
 *                   https://devzone.nordicsemi.com/question/83938/nrf52832-saadc-sampling/?comment=84340#comment-84340
 *                   Configured with the SAADC_OVERSAMPLE constant.
 * - BURST mode -> Burst mode can be combined with oversampling, which makes the SAADC sample all oversamples as fast
 *                 as it can with one SAMPLE task trigger. Set the SAADC_BURST_MODE constant to enable BURST mode.
 * - Offset Calibration -> SAADC needs to be occasionally calibrated. The desired calibration interval depends on the
 *                         expected temperature change rate, see the nRF52832 PS for more information. The
 *                         calibration interval can be adjusted with configuring the SAADC_CALIBRATION_INTERVAL
 *                         constant.
 * The SAADC sample result is printed on UART. To see the UART output, a UART terminal (e.g. Realterm) can be configured on 
 * your PC with the UART configuration set in the uart_config function, which is also described in the saadc example documentation -> 
 * http://infocenter.nordicsemi.com/topic/com.nordic.infocenter.sdk5.v11.0.0/nrf_dev_saadc_example.html?cp=5_0_0_4_5_24
 *
 * Indicators on the nRF52-DK board:
 * LED1: SAADC Sampling triggered 
 * LED2: SAADC sampling buffer full and event received
 * LED3: SAADC Offset calibration complete
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nrf.h"
#include "nrf_drv_saadc.h"
#include "boards.h"
#include "app_uart.h"
#include "app_error.h"
#include "app_util_platform.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_rtc.h"

#define UART_PRINTING_ENABLED                     //Enable to see SAADC output on UART. Comment out for low power operation.
#define UART_TX_BUF_SIZE 256                      //UART TX buffer size. 
#define UART_RX_BUF_SIZE 1                        //UART RX buffer size. 
#define RTC_CC_VALUE 8                            //Determines the RTC interrupt frequency and thereby the SAADC sampling frequency
#define SAADC_CALIBRATION_INTERVAL 5              //Determines how often the SAADC should be calibrated relative to NRF_DRV_SAADC_EVT_DONE event. E.g. value 5 will make the SAADC calibrate every fifth time the NRF_DRV_SAADC_EVT_DONE is received.
#define SAADC_SAMPLES_IN_BUFFER 4                 //Number of SAADC samples in RAM before returning a SAADC event. For low power SAADC set this constant to 1. Otherwise the EasyDMA will be enabled for an extended time which consumes high current.
#define SAADC_OVERSAMPLE NRF_SAADC_OVERSAMPLE_4X  //Oversampling setting for the SAADC. Setting oversample to 4x This will make the SAADC output a single averaged value when the SAMPLE task is triggered 4 times. Enable BURST mode to make the SAADC sample 4 times when triggering SAMPLE task once.
#define SAADC_BURST_MODE 1                        //Set to 1 to enable BURST mode, otherwise set to 0.

void saadc_init(void);

const  nrf_drv_rtc_t           rtc = NRF_DRV_RTC_INSTANCE(2); /**< Declaring an instance of nrf_drv_rtc for RTC2. */
static nrf_saadc_value_t       m_buffer_pool[2][SAADC_SAMPLES_IN_BUFFER];
static uint32_t                m_adc_evt_counter = 0;
static bool                    m_saadc_initialized = false;      
/**
 * @brief UART events handler.
 */
void uart_events_handler(app_uart_evt_t * p_event)
{
}

/**
 * @brief UART initialization.
 */
void uart_config(void)
{
    uint32_t                     err_code;
    const app_uart_comm_params_t comm_params =
    {
        RX_PIN_NUMBER,
        TX_PIN_NUMBER,
        RTS_PIN_NUMBER,
        CTS_PIN_NUMBER,
        APP_UART_FLOW_CONTROL_DISABLED,
        false,
        UART_BAUDRATE_BAUDRATE_Baud115200
    };

    APP_UART_FIFO_INIT(&comm_params,
                       UART_RX_BUF_SIZE,
                       UART_TX_BUF_SIZE,
                       uart_events_handler,
                       APP_IRQ_PRIORITY_LOW,
                       err_code);

    APP_ERROR_CHECK(err_code);
}

static void rtc_handler(nrf_drv_rtc_int_type_t int_type)
{
    uint32_t err_code;
	
    if (int_type == NRF_DRV_RTC_INT_COMPARE0)
    {		
        if(!m_saadc_initialized)
        {
            saadc_init();                                              //Initialize the SAADC. In the case when SAADC_SAMPLES_IN_BUFFER > 1 then we only need to initialize the SAADC when the the buffer is empty.
        }
        m_saadc_initialized = true;                                    //Set SAADC as initialized
        nrf_drv_saadc_sample();                                        //Trigger the SAADC SAMPLE task
			
        LEDS_INVERT(BSP_LED_0_MASK);                                   //Toggle LED1 to indicate SAADC sampling start
			
        err_code = nrf_drv_rtc_cc_set(&rtc,0,RTC_CC_VALUE,true);       //Set RTC compare value. This needs to be done every time as the nrf_drv_rtc clears the compare register on every compare match
        APP_ERROR_CHECK(err_code);
        nrf_drv_rtc_counter_clear(&rtc);                               //Clear the RTC counter to start count from zero
    }
}

static void lfclk_config(void)
{
    ret_code_t err_code = nrf_drv_clock_init();                        //Initialize the clock source specified in the nrf_drv_config.h file, i.e. the CLOCK_CONFIG_LF_SRC constant
    APP_ERROR_CHECK(err_code);

    nrf_drv_clock_lfclk_request(NULL);
}

static void rtc_config(void)
{
    uint32_t err_code;

    //Initialize RTC instance
    err_code = nrf_drv_rtc_init(&rtc, NULL, rtc_handler);              //Initialize the RTC with callback function rtc_handler. The rtc_handler must be implemented in this applicaiton. Passing NULL here for RTC configuration means that configuration will be taken from the nrf_drv_config.h file.
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_rtc_cc_set(&rtc,0,RTC_CC_VALUE,true);           //Set RTC compare value to trigger interrupt. Configure the interrupt frequency by adjust RTC_CC_VALUE and RTC2_CONFIG_FREQUENCY constant in the nrf_drv_config.h file
    APP_ERROR_CHECK(err_code);

    //Power on RTC instance
    nrf_drv_rtc_enable(&rtc);                                          //Enable RTC
}


void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)                                                        //Capture offset calibration complete event
    {
        ret_code_t err_code;
			
        LEDS_INVERT(BSP_LED_1_MASK);                                                                    //Toggle LED2 to indicate SAADC buffer full		

        if((m_adc_evt_counter % SAADC_CALIBRATION_INTERVAL) == 0)                                       //Evaluate if offset calibration should be performed. Configure the SAADC_CALIBRATION_INTERVAL constant to change the calibration frequency
        {
#ifdef UART_PRINTING_ENABLED
            printf("SAADC calibration starting...  \r\n");                                              //Print on UART
#endif //UART_PRINTING_ENABLED						
            NRF_SAADC->EVENTS_CALIBRATEDONE = 0;                                                        //Clear the calibration event flag
            nrf_saadc_task_trigger(NRF_SAADC_TASK_CALIBRATEOFFSET);                                     //Trigger calibration task
            while(!NRF_SAADC->EVENTS_CALIBRATEDONE);                                                    //Wait until calibration task is completed. The calibration tasks takes about 1000us with 10us acquisition time. Configuring shorter or longer acquisition time will make the calibration take shorter or longer respectively.
            while(NRF_SAADC->STATUS == (SAADC_STATUS_STATUS_Busy << SAADC_STATUS_STATUS_Pos));          //Additional wait for busy flag to clear. Without this wait, calibration is actually not completed. This may take additional 100us - 300us
            LEDS_INVERT(BSP_LED_2_MASK);                                                                //Toggle LED3 to indicate SAADC calibration complete
#ifdef UART_PRINTING_ENABLED
            printf("SAADC calibration complete ! \r\n");                                                //Print on UART
#endif //UART_PRINTING_ENABLED	
        }
     
        err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, SAADC_SAMPLES_IN_BUFFER);  //Set buffer so the SAADC can write to it again. This is either "buffer 1" or "buffer 2"
        APP_ERROR_CHECK(err_code);
#ifdef UART_PRINTING_ENABLED
        printf("ADC event number: %d\r\n",(int)m_adc_evt_counter);                                      //Print the event number on UART
        for (int i = 0; i < SAADC_SAMPLES_IN_BUFFER; i++)
        {
            printf("%d\r\n", p_event->data.done.p_buffer[i]);                                           //Print the SAADC result on UART
        }
        m_adc_evt_counter++;
#endif //UART_PRINTING_ENABLED				
				
        nrf_drv_saadc_uninit();                                                                   //Unintialize SAADC to disable EasyDMA and save power
        NRF_SAADC->INTENCLR = (SAADC_INTENCLR_END_Clear << SAADC_INTENCLR_END_Pos);               //Disable the SAADC interrupt
        NVIC_ClearPendingIRQ(SAADC_IRQn);                                                         //Clear the SAADC interrupt if set
        m_saadc_initialized = false;                                                              //Set SAADC as uninitialized
    }
}

void saadc_init(void)
{
    ret_code_t err_code;
    nrf_drv_saadc_config_t saadc_config;
    nrf_saadc_channel_config_t channel_config;
	
    //Configure SAADC
    saadc_config.resolution = NRF_SAADC_RESOLUTION_12BIT;                                 //Set SAADC resolution to 12-bit. This will make the SAADC output values from 0 (when input voltage is 0V) to 2^12=2048 (when input voltage is 3.6V for channel gain setting of 1/6).
    saadc_config.oversample = SAADC_OVERSAMPLE;                                           //Set oversample to 4x. This will make the SAADC output a single averaged value when the SAMPLE task is triggered 4 times.
    saadc_config.interrupt_priority = APP_IRQ_PRIORITY_LOW;                               //Set SAADC interrupt to low priority.
	
    //Initialize SAADC
    err_code = nrf_drv_saadc_init(&saadc_config, saadc_callback);                         //Initialize the SAADC with configuration and callback function. The application must then implement the saadc_callback function, which will be called when SAADC interrupt is triggered
    APP_ERROR_CHECK(err_code);
		
    //Configure SAADC channel
    channel_config.reference = NRF_SAADC_REFERENCE_INTERNAL;                              //Set internal reference of fixed 0.6 volts
    channel_config.gain = NRF_SAADC_GAIN1_6;                                              //Set input gain to 1/6. The maximum SAADC input voltage is then 0.6V/(1/6)=3.6V. The single ended input range is then 0V-3.6V
    channel_config.acq_time = NRF_SAADC_ACQTIME_10US;                                     //Set acquisition time. Set low acquisition time to enable maximum sampling frequency of 200kHz. Set high acquisition time to allow maximum source resistance up to 800 kohm, see the SAADC electrical specification in the PS. 
    channel_config.mode = NRF_SAADC_MODE_SINGLE_ENDED;                                    //Set SAADC as single ended. This means it will only have the positive pin as input, and the negative pin is shorted to ground (0V) internally.
    channel_config.pin_p = NRF_SAADC_INPUT_AIN0;                                          //Select the input pin for the channel. AIN0 pin maps to physical pin P0.02.
    channel_config.pin_n = NRF_SAADC_INPUT_DISABLED;                                      //Since the SAADC is single ended, the negative pin is disabled. The negative pin is shorted to ground internally.
    channel_config.resistor_p = NRF_SAADC_RESISTOR_DISABLED;                              //Disable pullup resistor on the input pin
    channel_config.resistor_n = NRF_SAADC_RESISTOR_DISABLED;                              //Disable pulldown resistor on the input pin

	
    //Initialize SAADC channel
    err_code = nrf_drv_saadc_channel_init(0, &channel_config);                            //Initialize SAADC channel 0 with the channel configuration
    APP_ERROR_CHECK(err_code);
		
    if(SAADC_BURST_MODE)
    {
        NRF_SAADC->CH[0].CONFIG |= 0x01000000;                                            //Configure burst mode for channel 0. Burst is useful together with oversampling. When triggering the SAMPLE task in burst mode, the SAADC will sample "Oversample" number of times as fast as it can and then output a single averaged value to the RAM buffer. If burst mode is not enabled, the SAMPLE task needs to be triggered "Oversample" number of times to output a single averaged value to the RAM buffer.		
    }

    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[0],SAADC_SAMPLES_IN_BUFFER);    //Set SAADC buffer 1. The SAADC will start to write to this buffer
    APP_ERROR_CHECK(err_code);
    
    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[1],SAADC_SAMPLES_IN_BUFFER);    //Set SAADC buffer 2. The SAADC will write to this buffer when buffer 1 is full. This will give the applicaiton time to process data in buffer 1.
    APP_ERROR_CHECK(err_code);
}

/**
 * @brief Function for main application entry.
 */
int main(void)
{	
    LEDS_CONFIGURE(LEDS_MASK);                       //Configure all leds
    LEDS_OFF(LEDS_MASK);                             //Turn off all leds
	
    NRF_POWER->DCDCEN = 1;                           //Enabling the DCDC converter for lower current consumption
	
#ifdef UART_PRINTING_ENABLED
    uart_config();                                   //Configure UART. UART is used to show the SAADC sampled result.
    printf("\n\rSAADC HAL simple example.\r\n");	
#endif //UART_PRINTING_ENABLED	

    lfclk_config();                                  //Configure low frequency 32kHz clock
    rtc_config();                                    //Configure RTC. The RTC will generate periodic interrupts. Requires 32kHz clock to operate.

    while(1)
    {
        __WFE();                                     //These three commands disable the CPU. CPU will wake up again on any event or interrupt.
        __SEV();
        __WFE();
    }
}


/** @} */
