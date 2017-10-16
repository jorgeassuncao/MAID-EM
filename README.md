# WEM - Wi-Fi Energy Monitor

The **Wi-Fi Energy Monitor** is an energy monitor based on a ESP8266 board that uses XTM18S (or compatible) single phase energy meter to get energy readings. The energy monitor is MQTT enabled and sends readings for power (W), consumption (kWh) and accumulated consumption (kWh), but with a couple tricks you can also get apparent power (kVA) and current (A).

### Version
Current version is: _WEM - Wi-Fi Energy Monitor v0.6.6 (14/10/2017)_

### Features
+ Works on NodeMCU boards. [View compatible boards](https://github.com/jorgeassuncao/ESP8266-Energy-Monitor/wiki/Parts-List)
+ MQTT enabled
+ Configurable topics to publish
+ Publishes various information over MQTT:
  + Current kWh
  + Current Watt
  + Accum kWh (pulses)
  + IP address
  + MAC address
+ Retains the last accumulated consumption value even if the device is rebooted, reset or powered off
+ ESP8266 Webserver to create a web interface
+ Web interface uses Bootstrap framework
+ Information available on the web interface:
  + Current kWh
  + Current Watt
  + Accum kWh (pulses)
+ A button on the web interface allows to reset the device remotely
+ Debug info via serial interface
+ Information available on the serial interface:
  + IP address
  + MAC address
  + Current kWh
  + Current Watt
  + Accum kWh (pulses)
+ Telnet server
  + Set CPU speed for ESP8266 (80MHz/160MHz)
  + Remote reset of the device
+ Remote debug via Telnet server with configurable debug level (verbose, debug, info, warning and errors), profiler and filter
+ Information available on the telnet interface
  + Host name
  + IP address
  + MAC address
  + Free Heap RAM
  + Current kWh
  + Current Watt
  + Accum kWh (pulses)
+ Visual confirmation of operation using the device internal blue LED


### To-do
+ Use a non-invasive current sensor to get current (Amps) value and make reliable calculation for Apparent Power (kVA)
