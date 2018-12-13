ble_app_uart__saadc_timer_driven__scan_mode example
==================

 This project contains a code example that shows nrf52 SAADC scan mode and how to integrate SAADC with softdevice.
 
Requirements
------------
- nRF5 SDK version 15.2.0
- nRF52-DK / nRF52840-DK
- Softdevice S132 v6.1.0 / S140 v6.1.0

To compile it, clone the repository in the \nRF5_SDK_15.2.0_9412b96\examples\nRF52-ADC-examples folder. If you download the zip, place the ble_app_uart__saadc_timer_driven__scan_mode folder into the \nRF5_SDK_15.2.0_9412b96\examples\ble_peripheral\ folder.

Documentation
-----------------
- Perhipheral: nRF52 SAADC
- Compatibility: nRF52832 rev. 1 and 2/nRF52832 rev. 1, nRF5 SDK 15.2.0
- Softdevice used: S132 v6.1.0 / S140 v6.1.0

This SAADC example samples on 4 different input pins, and enables scan mode to do that. It is otherwise an offspring from the standard ble_app_uart example available in nRF5 SDK 15.2.0

- Transmits SAADC output to hardware UART and over BLE via Nordic UART Servive (NUS).
- Info on NUS -> https://www.nordicsemi.com/en/DocLib/Content/SDK_Doc/nRF5_SDK/v15-2-0/ble_sdk_app_nus_eval
- Info on hardware UART settings -> https://www.nordicsemi.com/en/DocLib/Content/SDK_Doc/nRF5_SDK/v15-2-0/uart_example

About this project
------------------
This application is one of several applications that has been built by the support team at Nordic Semiconductor, as a demo of some particular feature or use case. It has not necessarily been thoroughly tested, so there might be unknown issues. It is hence provided as-is, without any warranty. 

However, in the hope that it still may be useful also for others than the ones we initially wrote it for, we've chosen to distribute it here on GitHub. 

The application is built to be used with the official nRF5 SDK, that can be downloaded from http://developer.nordicsemi.com/

Please post any questions about this project on https://devzone.nordicsemi.com.
