saadc_low_power example
==================

 This project contains code examples that shows various nrf52 SAADC functionality and low power operation.
 
Requirements
------------
- nRF5 SDK version 11.0.0
- nRF52-DK

To compile it, clone the repository in the \nRF5_SDK_11.0.0_89a8197\examples\peripheral\ folder.  If you download the zip, place the saadc_low_power folder into the \nRF5_SDK_11.0.0_89a8197\examples\peripheral\ folder.

Documentation
-----------------
- Perhipheral: nRF52 SAADC
- Compatibility: nRF52 rev 1, nRF5 SDK 11.0.0
- Softdevice used: No softdevice
  
This example enables the RTC timer to periodically trigger SAADC sampling. RTC is chosen here instead of TIMER because it is low power. The example samples on a single input pin, the AIN0, which maps to physical pin P0.02 on the nRF52832 IC.

This example consumes ~4uA when you have made the following two code modifications. You need to comment out the following code line:

    #define UART_PRINTING_ENABLED

You may want to have the line uncommented at first to see the SAADC output on UART. Also set the SAADC buffer size to 1 with the following code: 

    #define SAADC_SAMPLES_IN_BUFFER 1


This SAADC example shows the following features:
- **Low Power ->**    
    1) Initializing SAADC when sampling and uninitializing when sampling is complete. This will enable EasyDMA only during sampling, but leave it disabled when not sampling. The EasyDMA consumes around 1.5mA when enabled.    
    2) Low power can only be obtained when UART_PRINTING_ENABLED is not defined and SAADC_SAMPLES_IN_BUFFER is 1. If the UART is enabled, it will add around 700uA current consumption.  
    3) Enable DCDC converter at startup with `NRF_POWER->DCDCEN = 1;`  
    4) Use RTC instead of TIMER periperal. That will save ~300uA.
- **Oversampling ->** This reduces SAADC noise level, especially for higher SAADC resolutions, see https://devzone.nordicsemi.com/question/83938/nrf52832-saadc-sampling/?comment=84340#comment-84340   Configured with the SAADC_OVERSAMPLE constant.
- **BURST mode ->** Burst mode can be combined with oversampling, which makes the SAADC sample all oversamples as fast as it can with one SAMPLE task trigger. Set the SAADC_BURST_MODE constant to enable BURST mode.
- **Offset Calibration ->** SAADC needs to be occasionally calibrated. The desired calibration interval depends on the expected temperature change rate, see the nRF52832 PS for more information. The calibration interval can be adjusted with configuring the SAADC_CALIBRATION_INTERVAL constant.

The SAADC sample result is printed on UART. To see the UART output, a UART terminal (e.g. Realterm or Termite) can be configured on your PC with the UART configuration set in the uart_config function, which is also described in the saadc example documentation -> http://infocenter.nordicsemi.com/topic/com.nordic.infocenter.sdk5.v11.0.0/nrf_dev_saadc_example.html?cp=5_0_0_4_5_24
  
Indicators on the nRF52-DK board:
- LED1: SAADC Sampling triggered 
- LED2: SAADC sampling buffer full and event received
- LED3: SAADC Offset calibration complete

The **offset calibration** task is triggered and when calibration is done, the SAADC throws a "calibration done" event. However, the calibration is note complete until the "saadc busy" flag is cleared. In this code example, we wait for the offset calibration event and additionally we wait for the saadc busy flag to clear. The table below shows how long it typically takes to calibrate the saadc for different acquisition time setting.

Acquisition time | Start cal until end cal | Start cal until saadc ready
--------- | --------- | ---------
**3 us** | 102 us | 118 us
**10 us** | 279 us | 314 us
**40 us** | 988 us | 1152 us

About this project
------------------
This application is one of several applications that has been built by the support team at Nordic Semiconductor, as a demo of some particular feature or use case. It has not necessarily been thoroughly tested, so there might be unknown issues. It is hence provided as-is, without any warranty. 

However, in the hope that it still may be useful also for others than the ones we initially wrote it for, we've chosen to distribute it here on GitHub. 

The application is built to be used with the official nRF5 SDK, that can be downloaded from http://developer.nordicsemi.com/

Please post any questions about this project on https://devzone.nordicsemi.com.
