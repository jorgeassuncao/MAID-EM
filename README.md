# ESP8266 Energy Monitor

The **ESP Energy Monitor** is a wifi energy monitor based on a ESP8266 board that uses XTM18S (or compatible) single phase energy meter to get energy readings. The energy monitor is enabled with MQTT and sends readings for power (W), consumption (kWh) and accumulated consumption (kWh), but with a couple tricks you can also get apparent power (kVA) and current (A).

### Version
Current version is: _ESP Energy Monitor v0.6.6 (14/10/2017)_

### Features
+ Works on NodeMCU boards. [View compatible boards](https://github.com/jorgeassuncao/ESP8266-Energy-Monitor/wiki/Parts-List)
+ MQTT enabled
+ Configurable topics to publish
+ Publishes various information over MQTT:
  + Current kWh
  + Current Watt
  + Accum kWh (pulses)
+ Retains the last accumulated consumption value even if the board is rebooted, reset or powered off
+ Debug info via serial interface
  + IP address
  + MAC address
  + Current kWh
  + Current Watt
  + Accum kWh (pulses)
+ Telnet server
  + Set CPU speed for ESP8266 (80MHz/160MHz)
  + Remote reset of the board
+ Remote debug via Telnet server with configurable debug level (verbose, debug, info, warning and errors), profiler and filter
  + Host name
  + IP address
  + MAC address
  + Free Heap RAM
  + Current kWh
  + Current Watt
  + Accum kWh (pulses)
+ Visual confirmation of operation using the board internal blue LED


### To-do
+ ESP8266 Webserver to display information about the board as the MQTT topics, and serial port, on a web browser
+ Use a non-invasive current sensor to get current (Amps) value and make reliable calculation for Apparent Power (kVA)
