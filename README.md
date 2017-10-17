# MAID - Energy Monitor

The **MAID - Energy Monitor** is an energy monitor based on a ESP8266 board that uses the XTM18S (or compatible) single phase energy meter to get energy readings. The energy monitor is MQTT enabled and sends readings for power (W), consumption (kWh) and accumulated consumption (kWh), and with a couple tricks you can also get apparent power (kVA) and current (A).

The **MAID** acronym stands for **M**qtt en**A**bled w**I**fi no**D**e.

### Version
Current version is: _MAID - Energy Monitor v0.6.6 (15/10/2017)_

### Features
+ Works on ESP8266-based boards. [View compatible boards](https://github.com/jorgeassuncao/MAID-EM/#)
+ Separate file with all the variables that need to be configured, so you do not have to mess with all the code
+ MQTT enabled with the PubSubClient library
+ Configurable MQTT topics to fit your needs
+ Publishes various information over MQTT:
  + Current Consumption (kWh)
  + Current Power (Watt)
  + Accumulated Consumption (kWh)
  + IP address
  + MAC address
+ Retains the last accumulated consumption value even if the device is rebooted, reset or powered off
+ ESP8266 Webserver to create a web interface
+ Web interface uses Bootstrap framework
+ Information available on the web interface:
  + Current Consumption (kWh)
  + Current Power (Watt)
  + Accumulated Consumption (kWh)
+ A button on the web interface allows to reset the device remotely
+ Debug info via serial interface
+ Information available on the serial interface:
  + IP address
  + MAC address
  + Current Consumption (kWh)
  + Current Power (Watt)
  + Accumulated Consumption (kWh)
+ Telnet server
  + Set CPU speed for ESP8266 (80MHz/160MHz)
  + Remote reset of the device
+ Remote debug via Telnet server with configurable debug level (verbose, debug, info, warning and errors), profiler and filter
+ Information available on the telnet interface
  + Host name
  + IP address
  + MAC address
  + Free Heap RAM
  + Current Consumption (kWh)
  + Current Power (Watt)
  + Accumulated Consumption (kWh)
+ Visual confirmation of operation using the device internal blue LED


### To-do
+ Use a non-invasive current sensor to get current (Amps) value and make reliable calculation for Apparent Power (kVA)
