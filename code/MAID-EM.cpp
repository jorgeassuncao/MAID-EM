/**
 *        _____  ____    ____  _______          _
 *       |_   _||_   \  /   _||_   __ \        / \
 *         | |    |   \/   |    | |__) |      / _ \
 *     _   | |    | |\  /| |    |  __ /      / ___ \
 *    | |__' |   _| |_\/_| |_  _| |  \ \_  _/ /   \ \_
 *    `.____.'  |_____||_____||____| |___||____| |____|
 *
 * ESP Energy Monitor by Jorge Assunção
 *
 * Based on a project by timseebeck @ https://community.home-assistant.io/t/power-monitoring-with-an-xtm18s-and-mqtt/16316
 * Remote Debug over Telnet by JoaoLopesF @ https://github.com/JoaoLopesF/ESP8266-RemoteDebug-Telnet
 *
 * See Github for instructions on how to use this code: https://github.com/jorgeassuncao/ESP8266-Energy-Monitor
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License version 2 as published by the Free Software Foundation.
 *
 * You can change your personal user data (wifi access, MQTT server, etc) in the "config/userdata.h" file
 */

//************* INCLUDE LIBRARIES ************************************************************************
//********************************************************************************************************
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "RemoteDebug.h"
#include "userdata.h"

//************* PROJECT AND VERSION **********************************************************************
//********************************************************************************************************
const char* proj_ver = "MAID - Energy Monitor v0.6.6 (15/10/2017)";             // Project name and version

//************* CONFIG DEBUG *****************************************************************************
//********************************************************************************************************
RemoteDebug Debug;                                                              // Remote debug type

//************* CONFIG WEBSERVER *************************************************************************
//********************************************************************************************************
ESP8266WebServer server(80);                                                    // Webserver port

//************* CONFIG PINS *****************************************************************************
//********************************************************************************************************
#define DIGITAL_INPUT_SENSOR 12                                                 // Digital input D6

int LED_pin = 2;                                                                // Internal LED in NodeMCU
#define BRIGHT 150                                                              // Maximum LED intensity (1-500)
#define INHALE 1500                                                             // Breathe-in time in milliseconds
#define PULSE INHALE*1000/BRIGHT                                                // Pulse
#define REST 1000                                                               // Pause between breathes

//************* CONFIG PULSES PER KWH ********************************************************************
//********************************************************************************************************
#define PULSE_FACTOR 1000                                                       // Number of pulses of the meter per kWh

//************* CONFIG OTHER GLOBALS *********************************************************************
//********************************************************************************************************
unsigned long SEND_FREQUENCY = 15000;                                           // Minimum time between send in milliseconds
volatile unsigned long pulseCount = 0;                                          // Variable -
volatile unsigned long lastBlink = 0;                                           // Variable -
double kwh;                                                                     // Variable -
unsigned long lastSend;                                                         // Variable -

WiFiClient espClient;                                                           // Initiate wifi
PubSubClient client(espClient);                                                 // Initiate MQTT

long lastMsg = 0;                                                               // Variable -
char msg[50];                                                                   // Variable -
char wattString[7];                                                             // Variable -
char kwhString[7];                                                              // Variable -
double kwhaccum;                                                                // Variable -
char kwhaccumString[7];                                                         // Variable -
float kwhReading;                                                               // Variable -

uint32_t mmLastTime = 0;                                                        // Variable -
uint32_t mmTimeSeconds = 0;                                                     // Variable -

int N = 1;                                                                      // Variable -  Number of times to loop
int runXTimes = 0;                                                              // Variable -  Count times looped

byte mac[6];                                                                    // Variable - MAC address
char myBuffer[15];                                                              // Variable - MAC string buffer

uint8_t MAC_array[6];                                                           // Variable -
char MAC_char[18];                                                              // Variable -

//************* CONFIG WEBSERVER ROOT PAGE ***************************************************************
//********************************************************************************************************
String getPage(){                                                               // Create webpage content
  String page = "<!DOCTYPE html>";
  page += "<html lang='en'>";
  page +=   "<head>";
  page +=     "<meta charset='utf-8'>";
  page +=     "<meta http-equiv='X-UA-Compatible' content='IE=edge'>";
  page +=     "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  page +=     "<meta http-equiv='refresh' content='15'/>";
  page +=     "<link rel='shortcut icon' href='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAADAAAAAwCAYAAABXAvmHAAAABGdBTUEAALGPC/xhBQAAAAFzUkdCAK7OHOkAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAZiS0dEAAAAAAAA+UO7fwAAAAlwSFlzAAAASAAAAEgARslrPgAAAyRJREFUaN7tmM1vDWEUxn/6QUhK4itxW6ErCQsSafEHSDStWAmxKhJKRCokysbdoHtCb22JpBo2SiyEv6FpoldYqK9ElbaKCnItZiamz7y9fWfuO23UfZJZnDnnPOc5M++8HwNllFHGf4Hqf7lmNdAHdM6i+CxwH6hyQZYDCv6VnSXxQb1rpZIdCJEF15EUxbcZ6u1LSrYS+Chkg/79tJAB3krNYWB5ErIuIZoANqYoPkAD8E1qX41LUgf8EJJ2Q1yjA8EmjtNSexKojUPaKQTPiM4Iu4HflPZhZ32OFrlfBeRFwyVb0krgjSS3SkwtME5ps1M2lD+GN/7DOCgahoAKG+IdkjgKLJaYG0Rf8foY4uuJDtGcxCyRh1TAcsiel6Sb4l8L/JKYtgRv4Lhw/CQ6zm9LTIeSmF7JVrGfir0Hb5gF6Ae6DU32+k9wHLgHbJCYHDAQsqt87jCezKDNiAHpukH8D8V/0iB+RGIKwCffF0a7xPSJf5v4+20a0OK6cOnsoGtDr0F8cPVI7CbxD4p/tfiHbRrQj2uh+PXDqpnBH77GJLZG/OPiXyT+SRVrNS05RME1oamBCbGXiv1ebB3Xj4vUeyT2OrHfib1M7C82DajAerFfir1T7A7gs4F3BG+LUCz3xQy1VZuxgbzYm8V+IPZh4cn7OXf4O432AFvwVvgAlX5uMW6trdqMOMfUD+eW+OvwFp1wzDEbYsEJ4bBZyM7aEG8nOnPoVqKb6OxQb0Puw7SV6JIY01aiwYa8Angtia0Sk/Ebc7WZGwXWiP+QaHhFjFnzMtEFRrfTLbjbTjfL/WrguWi4GIfYdKA5ZYgr9kp1EZsOJo4zRIdohpi4LiRf8ZZ+W9g2oGgEvkvulbjiAVYQPdTngVUpNlBL9FD/gYSHeoD9BiFHU2zA9Ftlb1LxAcJDKRsjL+kQyoZyYv+NMKESuAtciJmXtAHwFixnvxYh2Y/WUhqwrrnAVYfTNJB6LZfngWam/o4xNVTAW+Wb0mimVOj2o9g15Kqo6bU6PzU5xhTNs32kdI5yA/MJc/IRu0STZRNDwK65FltGGfMFfwB1P4cc2cai3gAAACV0RVh0ZGF0ZTpjcmVhdGUAMjAxNy0xMC0xM1QyMDo1OTo1MiswMDowMGf6eRAAAAAldEVYdGRhdGU6bW9kaWZ5ADIwMTctMTAtMTNUMjA6NTk6NTIrMDA6MDAWp8GsAAAAKHRFWHRzdmc6YmFzZS11cmkAZmlsZTovLy90bXAvbWFnaWNrLWZDN1pLU3MxVs1E8gAAAABJRU5ErkJggg=='>";
  page +=     "<title>";
  page +=       DEVICE_HOSTNAME;
  page +=     "</title>";
  page +=     "<link rel='stylesheet' href='//maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css' integrity='sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u' crossorigin='anonymous'>";
  page +=     "<link rel='stylesheet' href='//maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css' integrity='sha384-rHyoN1iRsVXV4nD0JutlnGaslCJuC7uwjduW9SVrLvRYooPp2bWYgmgJQIXwl/Sp' crossorigin='anonymous'>";
  page +=     "<script src='//maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js' integrity='sha384-Tc5IQib027qvyjSMfHjOMaLkfuWVxZxUPnCJA7l2mCWNIpG9mGCD8wGNIcPD7Txa' crossorigin='anonymous'></script>";
  page +=     "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.0/jquery.min.js'></script>";
  page +=   "</head>";
  page +=   "<body>";
  page +=   "<p></p>";
  page +=     "<div class='container-fluid'>";
  page +=     "<div class='row'>";
  page +=       "<div class='col-md-12'>";
  page +=         "<div class='jumbotron'>";
  page +=           "<h2>";
  page +=             proj_ver;
  page +=           "</h2>";
  page +=           "<p>This project uses a MQTT topic to store the accumulated kWh value, so that when there is a reboot, reset or power off the value won't be lost. The subscribed topic to get the accumulated kWh value is <small><em style='color: #ababab;'>";
  page +=             mqtt_topic_sub_1;
  page +=           "</em></small></p>";
  page +=         "</div>";
  page +=         "<div class='alert alert-dismissable alert-info'>";
  page +=           "<button type='button' class='close' data-dismiss='alert' aria-hidden='true'>×</button>";
  page +=           "<strong>Attention!</strong> This page auto-refreshes every 15 seconds.";
  page +=         "</div>";
  page +=       "</div>";
  page +=     "</div>";
  page +=     "<div class='row'>";
  page +=       "<div class='col-md-4'>";
  page +=         "<h3 class='text-primary'>Current Consumption</h3>";
  page +=           "<p class='lead'>";
  page +=             kwhString;
  page +=           " kWh</p>";
  page +=         "</div>";
  page +=       "<div class='col-md-4'>";
  page +=         "<h3 class='text-primary'>Current Power</h3>";
  page +=           "<p class='lead'>";
  page +=             wattString;
  page +=           " Watt</p>";
  page +=         "</div>";
  page +=       "<div class='col-md-4'>";
  page +=         "<h3 class='text-primary'>Accum Consumption</h3>";
  page +=           "<p class='lead'>";
  page +=             kwhaccumString;
  page +=           " kWh (total)</p>";
  page +=         "</div>";
  page +=       "</div>";
  page +=       "<div class='row'>";
  page +=         "<div class='col-md-12'>";
  page +=           "<div class='alert alert-dismissable alert-danger'>";
  page +=             "<button type='button' class='close' data-dismiss='alert' aria-hidden='true'>×</button>";
  page +=           "<h4>Warning!</h4>Sometimes the board does not restart correctly. If you can't get back to this page after one minute, you need to reset on the board directly on the fisical reset button.&nbsp;&nbsp;";
  page +=           "<button type='button' class='btn btn-warning btn-default'>RESET</button>";
  page +=         "</div>";
  page +=     "</div>";
  page +=   "</div>";
  page +=     "<div class='col-md-12'>";
  page +=       "copyright <br/><br/>";
  page +=     "</div>";
  page += "</div>";
  page +=   "</body>";
  page += "</html>";
  return page;
}

//************* SETUP WIFI SERVER ************************************************************************
//********************************************************************************************************
void handleRoot() {                                                             // Handle "root" page
  server.send ( 200, "text/html", getPage() );
}

void handleNotFound(){                                                          // Handle "not found" page
  String message = "Page or File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

//************* SETUP WIFI *******************************************************************************
//********************************************************************************************************
void setup_wifi() {
  delay(20);

/** TELNET **/
  Debug.begin(DEVICE_HOSTNAME);                                                       // Initiaze the telnet server
  Debug.setResetCmdEnabled(true);                                               // Enable/disable (true/false) the reset command (true/false)
  Debug.showTime(false);                                                        // Enable/disable (true/false) timestamps
  Debug.showProfiler(false);                                                    // Enable/disable (true/false) Profiler - time between messages of Debug
  Debug.showDebugLevel(false);                                                  // Enable/disable (true/false) debug levels
  Debug.showColors(true);                                                       // Enable/disable (true/false) colors

  Serial.println("- - - - - - - - - - - - - - - - - - - - - - - - - - -");      // Block separator to serial interface
  Debug.println("- - - - - - - - - - - - - - - - - - - - - - - - - - -");       // Block separator to telnet debug interface
  Serial.println(proj_ver);                                                     // Send project name and version to serial interface
  Debug.println(proj_ver);                                                      // Send project name and version to telnet debug interface
  Serial.println("- - - - - - - - - - - - - - - - - - - - - - - - - - -");      // Block separator to serial interface
  Debug.println("- - - - - - - - - - - - - - - - - - - - - - - - - - -");       // Block separator to telnet debug interface
  Serial.println();                                                             // Send space to serial interface
  Debug.println();                                                              // Send space to telnet debug interface

  Serial.println();                                                             // Connecting to wifi network
  Serial.print("Connecting to "); Serial.println(ssid);                         // Send network name to serial interface
  Debug.printf("Connecting to "); Debug.println(ssid);                          // Send network name to telnet debug interface

  WiFi.config(ip, dns, gateway, subnet);                                        // Configure connection with IP, DNS, Gateway and subnet
  WiFi.mode(WIFI_STA);                                                          // Switch to STA mode
  WiFi.begin(ssid, password);                                                   // Start wifi connection with SSID and Password
  WiFi.macAddress(mac);                                                         // Get MAC address of the node

  while (WiFi.status() != WL_CONNECTED) {                                       // Wait until connected to wifi
    delay(500);
    Serial.print(".");
  }

  Serial.println();                                                             // Block space to serial interface
  Debug.println();                                                              // Block space to telnet debug interface
  Serial.println("WiFi connected");                                             // Send successful connection to serial interface
  Debug.println("WiFi connected");                                              // Send successful connection to telnet debug interface

  Serial.print("IP address is "); Serial.println(WiFi.localIP());               // Send IP address to serial interface
  Debug.printf("IP address is "); Debug.println(WiFi.localIP());                // Send IP address to telnet debug interface

  sprintf(myBuffer,"%02X:%02X:%02X:%02X:%02X:%02X",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);    // Get MAC address
  Serial.print("MAC address is "); Serial.println(myBuffer);                                      // Send MAC address to serial interface
  Debug.printf("MAC address is "); Debug.println(myBuffer);                                       // Send MAC address to telnet debug interface

}

//************* READ MQTT TOPIC **************************************************************************
//********************************************************************************************************
void callback(char* topic, byte* payload, unsigned int length) {
  if (runXTimes < N) {                                                          // Read the topic only N times

  Serial.println();                                                             // Block space to serial interface
  Debug.println();                                                              // Block space to telnet debug interface
  Serial.print("Message arrived for topic [");                                  // Send text to serial interface
  Debug.printf("Message arrived for topic [");                                  // Send text to telnet debug interface
  Serial.print(topic); Serial.print("] ");                                      // Send MQTT topic to serial interface
  Debug.printf(topic); Debug.printf("] ");                                      // Send MQTT topic to telnet debug interface

  String value = "";                                                            // Store MQTT topic inside string
  for (int i=0;i<length;i++) {
    value += (char)payload[i];
  }

  kwhReading = value.toFloat();                                                 // Convert kwhReading to float

  Serial.println();                                                             // Block space to serial interface
  Debug.println();                                                              // Block space to telnet debug interface
  Serial.print("Payload: "); Serial.println(kwhReading);                        // Send MQTT payload to serial interface
  Debug.printf("Payload: "); Debug.println(kwhReading);                         // Send MQTT payload to telnet debug interface
  Serial.println();                                                             // Block space to serial interface
  Debug.println();                                                              // Block space to telnet debug interface

  runXTimes++;                                                                  // Increase loop count
  }
}

//************* RECONNECT MQTT ***************************************************************************
//********************************************************************************************************
void reconnect() {

  while (!client.connected()) {                                                 // Loop until reconnected
    delay(5000);                                                                // Wait 5 seconds before retrying
    Serial.print("Attempting connection to MQTT server... ");                   // Send text to serial interface
    Debug.printf("Attempting connection to MQTT server... ");                   // Send text to telnet debug interface
    if (client.connect(
			DEVICE_HOSTNAME, MQTT_USERNAME, MQTT_PASSWORD,
			MQTT_WILL_TOPIC, MQTT_WILL_QOS, MQTT_WILL_RETAIN, MQTT_WILL_MESSAGE)) {		// Connect to MQTT broker
      Serial.println(" connected!");                                            // Send text to serial interface
      Debug.println(" connected!");                                             // Send text to telnet debug interface
      client.subscribe(mqtt_topic_sub_1);                                       // MQTT topic to subscribe
    } else {
      Serial.print("failed, rc=");                                              // Send text to serial interface
      Debug.printf("failed, rc=");                                              // Send text to telnet debug interface
      Serial.print(client.state());                                             // Send failure state to serial interface
      //Debug.printf(client.state());                                           // Send failure state to telnet debug interface
      Serial.println(" try again in 10 seconds");                               // Send text to serial interface
      Debug.println(" try again in 10 seconds!");                               // Send text to telnet debug interface
      delay(10000);                                                             // Wait 5 seconds before retrying
    }
  }
}

//************* ON PULSE *********************************************************************************
//********************************************************************************************************
void onPulse()
{
    unsigned long newBlink = micros();
    unsigned long interval = newBlink-lastBlink;

    if (interval<10000L) {                                                      // Sometimes we get interrupt on RISING
            return;
    }

    kwh += 1.0 / (double)PULSE_FACTOR;                                          // Every time there is a pulse, the energy consumption is 1 [pulse] / PULSE_FACTOR [pulses/kWh]
    lastBlink = newBlink;
    pulseCount++;                                                               // Accumulate the energy (it will be initialized again once MQTT message is sent)
}

//************* SETUP ************************************************************************************
//********************************************************************************************************
void setup()
{
  pinMode(LED_pin, OUTPUT);                                                     // Configure internal LED pin as output.

  WiFi.macAddress(MAC_array);                                                   // NEWNEWNEW
  for (int iii = 0; iii < sizeof(MAC_array); ++iii){                            // NEWNEWNEW
    sprintf(MAC_char,"%s%02X:",MAC_char,MAC_array[iii]);                        // NEWNEWNEW
  }

  if (MDNS.begin("esp8266")) {                                                  //
    Serial.println("MDNS responder started");                                   //
  }                                                                             //

  server.on("/", handleRoot);                                                   // Serve root page

  server.onNotFound(handleNotFound);                                            // Serve page not found

  server.begin();                                                               // Start Webserver
  Serial.println("HTTP server started");                                        // Send text to serial interface
  Debug.println("HTTP server started");                                         // Send text to telnet debug interface

  Serial.begin(115200);                                                         // Start serial interface
  setup_wifi();                                                                 // Start wifi
  client.setServer(MQTT_SERVER, MQTT_PORT);                                     //
  client.setCallback(callback);                                                 //

  // Use the internal pullup to be able to hook up this sketch directly to an energy meter with S0 output
  // If no pullup is used, the reported usage will be too high because of the floating pin
  pinMode(DIGITAL_INPUT_SENSOR,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DIGITAL_INPUT_SENSOR), onPulse, RISING);

  Debug.begin(DEVICE_HOSTNAME);                                                 // Start Telnet server

  kwh = 0;
  lastSend=millis();
}

//************* LOOP *************************************************************************************
//********************************************************************************************************
void loop()
{
  server.handleClient();                                                        // Handle http requests

    for (int ii=1;ii<BRIGHT;ii++){                                              // Breath in
      digitalWrite(LED_pin, LOW);                                               // LED on
      delayMicroseconds(ii*10);                                                 // Wait
      digitalWrite(LED_pin, HIGH);                                              // LED off
      delayMicroseconds(PULSE-ii*10);                                           // Wait
      delay(0);                                                                 // Prevent watchdog firing
    }
    for (int ii=BRIGHT-1;ii>0;ii--){                                            // Breath out
      digitalWrite(LED_pin, LOW);                                               // LED on
      delayMicroseconds(ii*10);                                                 // Wait
      digitalWrite(LED_pin, HIGH);                                              // LED off
      delayMicroseconds(PULSE-ii*10);                                           // Wait
      ii--;
      delay(0);                                                                 // Prevent watchdog firing
    }
      delay(REST);                                                              // Pause before repeat

    if (!client.connected()) {                                                  // If client disconnects...
        reconnect();                                                            // ...connect again
    }

    client.loop();

    unsigned long now = millis();
    bool sendTime = now - lastSend > SEND_FREQUENCY;                            // Only send values at a maximum frequency

    if (sendTime) {
      Serial.println("- - - - - - - - - - - -");                                // Block separator to serial interface
      Debug.println("- - - - - - - - - - - -");                                 // Block separator to telnet debug interface

      dtostrf(kwh, 2, 4, kwhString);                                            // Convert Current kWh to string
      client.publish(mqtt_topic_kwh,kwhString);                                 // Publish Current kWh to MQTT topic
      Serial.print("- Current kWh: "); Serial.println(kwhString);               // Send Current kWh to serial interface
      Debug.printf("- Current kWh: "); Debug.println(kwhString);                // Send Current kWh to telnet debug interface
      lastSend = now;                                                           // Update the send time after publishing

      double watt = kwh * 1000.0 * 3600.0 / (double)SEND_FREQUENCY * 1000.0;    // Calculate the power using the energy
      dtostrf(watt, 4, 2, wattString);                                          // Convert Current Watt to string
      client.publish(mqtt_topic_watt,wattString);                               // Publish Current Watt to MQTT topic
      Serial.print("- Current Watt: "); Serial.println(wattString);             // Send Current Watt to serial interface
      Debug.printf("- Current Watt: "); Debug.println(wattString);              // Send Current Watt to telnet debug interface

      kwhaccum = kwhReading + ((double)pulseCount/((double)PULSE_FACTOR));      // Calculate the accumulated energy since the begining
      dtostrf(kwhaccum, 5, 2, kwhaccumString);                                  // Convert Accum kWh (pulses) to string
      client.publish(mqtt_topic_pulse,kwhaccumString, true);                    // Publish Accum kWh (pulses) to MQTT topic
      Serial.print("- Accum kWh: "); Serial.println(kwhaccumString);            // Send Accum kWh (pulses) to serial interface
      Debug.printf("- Accum kWh: "); Debug.println(kwhaccumString);             // Send Accum kWh (pulses) to telnet debug interface
      kwh = 0;                                                                  // Reset kWh count

      String ipaddress = WiFi.localIP().toString();                             // Create IP address string
      char ipchar[ipaddress.length()+1];
      ipaddress.toCharArray(ipchar,ipaddress.length()+1);
      client.publish(mqtt_topic_ip,ipchar);                                     // Publish IP address to MQTT topic

      Serial.println();                                                         // Block space to serial interface
      Debug.println();                                                          // Block space to telnet debug interface
    }

  Debug.handle();                                                               // Remote debug over telnet

  yield();                                                                      // Yielding

}

// END
//********************************************************************************************************
