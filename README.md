nrf52-ADC-examples
==================

 This project contains code examples that show nrf52 SAADC functionality.
 
Requirements
------------
- nRF5 SDK version 11.0.0
- nRF52-DK

To compile it, clone the repository in the \nRF5_SDK_11.0.0_89a8197\examples\peripheral\ folder.  If you download the zip, place the saadc_low_power folder into the \nRF5_SDK_11.0.0_89a8197\examples\peripheral\ folder.

Documentation
-----------------
Perhipheral: nRF52 SAADC
Compatibility: nRF52 rev 1, nRF5 SDK 11.0.0
Softdevice used: No softdevice
  
This example enables the RTC timer to periodically trigger SAADC sampling. RTC is chosen here instead of TIMER because it is low power. The example samples on a single input pin

This SAADC example shows the following features:
- Low Power -> Enabled with initializing SAADC when sampling and uninitializing when sampling is complete. Low power can only be obtained when UART_PRINTING_ENABLED is not defined and SAADC_SAMPLES_IN_BUFFER is 1
- Oversampling -> This reduces SAADC noise level, especially for higher SAADC resolutions, see https://devzone.nordicsemi.com/question/83938/nrf52832-saadc-sampling/?comment=84340#comment-84340   Configured with the SAADC_OVERSAMPLE constant.
- BURST mode -> Burst mode can be combined with oversampling, which makes the SAADC sample all oversamples as fast as it can with one SAMPLE task trigger. Set the SAADC_BURST_MODE constant to enable BURST mode.
- Offset Calibration -> SAADC needs to be occasionally calibrated. The desired calibration interval depends on the expected temperature change rate, see the nRF52832 PS for more information. The calibration interval can be adjusted with configuring the SAADC_CALIBRATION_INTERVAL constant.

The SAADC sample result is printed on UART. To see the UART output, a UART terminal (e.g. Realterm) can be configured on your PC with the UART configuration set in the uart_config function, which is also described in the saadc example documentation -> http://infocenter.nordicsemi.com/topic/com.nordic.infocenter.sdk5.v11.0.0/nrf_dev_saadc_example.html?cp=5_0_0_4_5_24
  
Indicators on the nRF52-DK board:
- LED1: SAADC Sampling triggered 
- LED2: SAADC sampling buffer full and event received
- LED3: SAADC Offset calibration complete

About this project
------------------
This application is one of several applications that has been built by the support team at Nordic Semiconductor, as a demo of some particular feature or use case. It has not necessarily been thoroughly tested, so there might be unknown issues. It is hence provided as-is, without any warranty. 

However, in the hope that it still may be useful also for others than the ones we initially wrote it for, we've chosen to distribute it here on GitHub. 

The application is built to be used with the official nRF5 SDK, that can be downloaded from http://developer.nordicsemi.com/

Please post any questions about this project on https://devzone.nordicsemi.com.
