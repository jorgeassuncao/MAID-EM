/**
 * File contents of "userdata_devel.h" for MAID - Energy Monitor
 *
 * This file is used to change the userdata to meet your needs
 * For more information visit https://github.com/jorgeassuncao/MAID-EM
 */

//************ CONFIG WIFI *******************************************************************************
// Configurations of your wifi network - Fixed IP is used because it's quicker than DHCP. IP address and
// and Hostname must be unique inside your network
//********************************************************************************************************
const char* ssid = "IOT";                                                       // Wifi SSID
const char* password = "Bl29pvJWSeSXVirffoDD0i9JmZ7z6vouzks2Eed2B";             // Wifi password

IPAddress ip(10,10,20,100);                                                     // IP address
IPAddress dns(8,8,8,8);                                                         // DNS server
IPAddress gateway(10,10,20,1);                                                  // Gateway
IPAddress subnet(255,255,255,0);                                                // Subnet mask

#define DEVICE_HOSTNAME "AQS"                                                         // Hostname

//************* CONFIG MQTT ******************************************************************************
// Configurations of your MQTT server -
//********************************************************************************************************
const char* MQTT_SERVER = "10.10.10.7";                                         // MQTT server IP ou URL
int MQTT_PORT = 1883;                                                           // MQTT port
const char* MQTT_USERNAME = "hassio";                                           // MQTT user
const char* MQTT_PASSWORD = "root";                                             // MQTT password

//************ MQTT LWT **********************************************************************************
// Configurations of your MQTT LWT - Define here the Last Will and Testment of your device. the MQTT broker and the
// payload for door open and closed
//********************************************************************************************************
  const char* MQTT_WILL_TOPIC = "aqs/status";// MQTT last will topic
  const char* MQTT_WILL_MESSAGE = "Online";                                     // MQTT last will message
  int MQTT_WILL_QOS = 1;                                                        // MQTT last will QoS (0,1 or 2)
  int MQTT_WILL_RETAIN = 0;                                                     // MQTT last will retain (0 or 1)

//************ MQTT TOPICS *******************************************************************************
// Configurations of your MQTT topics - to match your MQTT broker
//********************************************************************************************************
const char* mqtt_topic_watt = "aqs/watt";                                       // MQTT topic - watt
const char* mqtt_topic_kwh = "aqs/kwh";                                         // MQTT topic - kwh
const char* mqtt_topic_pulse = "aqs/pulse";                                     // MQTT topic - pulse
const char* mqtt_topic_ip = "aqs/ip";                                           // MQTT topic - ip
const char* mqtt_topic_mac = "aqs/mac";                                         // MQTT topic - mac
const char* mqtt_topic_sub_1 = mqtt_topic_pulse;                                // MQTT topic subscribe #1
