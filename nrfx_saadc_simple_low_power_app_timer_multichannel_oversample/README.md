nrfx_saadc_simple_low_power_app_timer_multichannel_oversample example
==================

 This project contains code examples that shows how to use the nrfx SAADC driver with API v2. The nrfx_saadc driver API was completely rewritten for nrfx v2.0.0 release, but this release has not been supported in nRF5 SDK. With the release of SDK 17.0.0, the new driver API was backported from nrfx v2.x.0 to nrfx v1.8.4. There are no examples of how to use this driver API in the nRF5 SDK itself, but some simple examples are provided in the nRF5 SDK v17.0.0 migration guide (https://infocenter.nordicsemi.com/topic/sdk_nrf5_v17.0.0/migration.html#migr_lib_nrfx) / nrfx GitHub Wiki page (https://github.com/NordicSemiconductor/nrfx/wiki/nrfx-1.8.1-to-2.0.0#saadc-1). This example is based on the example "Example code for SAADC in simple mode with IRQs", but is adapted to work with nRF5 SDK and implements some additional features.
 
Requirements
------------
- nRF5 SDK version 17.0.0
- nRF52-DK/nRF52840-DK

To compile it, clone the repository in the \nRF5_SDK_17.0.0_9d13099\examples\peripheral\ folder. If you download the zip, place the nrfx_saadc_simple_low_power_app_timer_multichannel_oversample folder into the \nRF5_SDK_17.0.0_9d13099\examples\peripheral\ folder.

Documentation
-----------------
- Perhipheral: nRF52 SAADC
- Compatibility: nRF52832 rev. 1 and 2/nRF52840 rev. 1 and 2, nRF5 SDK 17.0.0
- Softdevice used: No softdevice
  
This example uses the application timer library to periodically trigger SAADC sampling. App_timer runs off the 32.768kHz LFCLK, providing low power consumption. DCDC regulator is also enabled, to get the lowest current consumption possible. The example samples on four separate input pins, the AIN0-AIN3, which maps to physical pins P0.02-P0.05 on the nRF52832/nRF52840 ICs. This example also implements OVERSAMPLING and BURST mode, combined with SCAN mode.

About this project
------------------
This application is one of several applications that has been built by the support team at Nordic Semiconductor, as a demo of some particular feature or use case. It has not necessarily been thoroughly tested, so there might be unknown issues. It is hence provided as-is, without any warranty. 

However, in the hope that it still may be useful also for others than the ones we initially wrote it for, we've chosen to distribute it here on GitHub. 

The application is built to be used with the official nRF5 SDK, that can be downloaded from http://developer.nordicsemi.com/

Please post any questions about this project on https://devzone.nordicsemi.com.
