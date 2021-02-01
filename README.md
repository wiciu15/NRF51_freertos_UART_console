# NRF51_freertos_UART_console
 Simple demo of FreeRTOS running on NRF51822. 
 
 Features a command line interface on UART pin 0(TX) and 1(RX) - 38400 baudrate. Because of heavy use of escape characters use puTTY for best experience.
 
 # Compile and program
Set up Eclipse Embedded CDT (https://eclipse-embed-cdt.github.io/) and import this project.


Adjust paths to your NRFSDK 8 in Project properties->C/C++ Build -> Settings -> GNU ARM Cross C Compiler -> system include paths

For flashing with ST-Link V2 and openOCD, update path to openOCD executable inside flash.bat. OpenOCD version 0.10.0 or higher features ST-link as a tool, and scripts for NRF51xxx targets so no additional configuration should be needed
