nrf51-ADC-examples
==================

 This project contains code examples that show nrf51 internal ADC functionality.
 
Requirements
------------
- nRF51 SDK version 9.0.0
- nRF51-DK or nRF51-Dongle

The project may need modifications to work with other versions or other boards. 

To compile it, clone the repository in the \nRF51_SDK_9.0.0_2e23562\examples\ble_peripheral\ folder.

Documentation with project ble_app_template__ADC_sampling__app_trace__NUS
-----------------
 * This project is an add-on to the ble_app_template application of nRF51 SDK 9.0.0
 * The add-on consists of ADC code in order to sample an analog input pin with the 
 * internal ADC and write the result out on UART. If the PCA_10028 board is used 
 * (nRF51-DK) the 3xLSB of the ADC result is also written out on LED_2, LED_3 and
 * LED_4 on the PCA_10028 board. LED_1 flashes to indicate the device is advertising
 * but is lid when the board is connected.
 *
 * periodic app_timer is used to trigger the ADC sampling. At startup, the app_timer
 * is started. When the app_timer expires, the application receives a timer callback.
 * In the timer callback handler, the ADC sampling is started. when the ADC sampling
 * finishes, the ADC interrupt handler is executed which logs the result on LEDs and 
 * UART and on NUS service. The printout on UART is performed by the app_trace library. 
 * 
 * This file contains source code for a sample application that uses the Nordic UART service (NUS).
 * Connect via BLE with Master Control Panel and the PCA10000/PCA10031 dongle, running
 * the master emulator firmware (MEFW).
 *
 * To see the ADC result that is printed out on UART, you can set up a UART terminal. 
 * You should configure it as described on 
 * http://infocenter.nordicsemi.com/topic/com.nordic.infocenter.sdk51.v9.0.0/lib_trace.html?cp=4_1_0_3_6
 * If you use windows PC, you can e.g. use http://sourceforge.net/projects/realterm/
 * as the UART terminal. To see what port the board connects on, you could open 
 * Device Manager on Windows and see what ports are connected. Unplug the USB plug
 * to the nRF51 development board to see what port disappears in Device Manager, 
 * then connect the USB plug again to connect the board.

About this project
------------------
This application is one of several applications that has been built by the support team at Nordic Semiconductor, as a demo of some particular feature or use case. It has not necessarily been thoroughly tested, so there might be unknown issues. It is hence provided as-is, without any warranty. 

However, in the hope that it still may be useful also for others than the ones we initially wrote it for, we've chosen to distribute it here on GitHub. 

The application is built to be used with the official nRF51 SDK, that can be downloaded from https://www.nordicsemi.no, provided you have a product key for one of our kits.

Please post any questions about this project on https://devzone.nordicsemi.com.
