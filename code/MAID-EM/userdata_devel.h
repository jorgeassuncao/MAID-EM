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
const char* ssid = "UNIVERSE";                                                  // Wifi SSID
const char* password = "2526F68597";                                            // Wifi password

IPAddress ip(192,168,1,222);                                                    // IP address
IPAddress dns(8,8,8,8);                                                         // DNS server
IPAddress gateway(192,168,1,1);                                                 // Gateway
IPAddress subnet(255,255,255,0);                                                // Subnet mask

#define host_name "Wi-Fi Energy Monitor 01"                                        // Hostname

//************* CONFIG MQTT ******************************************************************************
// Configurations of your MQTT server -
//********************************************************************************************************
const char* mqtt_server = "192.168.1.200";                                      // MQTT server IP ou URL
int mqtt_port = 1883;                                                           // MQTT port
const char* mqtt_username = "homeassistant";                                    // MQTT user
const char* mqtt_password = "JA2508ca72";                                       // MQTT password

//************ MQTT TOPICS *******************************************************************************
// Configurations of your MQTT topics - to match your MQTT broker
//********************************************************************************************************
const char* mqtt_topic_watt = "home/indoor/sensor/ESP_Energy_Meter_01/watt_t";  // MQTT topic - watt
const char* mqtt_topic_kwh = "home/indoor/sensor/ESP_Energy_Meter_01/kwh_t";    // MQTT topic - kwh
const char* mqtt_topic_pulse = "home/indoor/sensor/ESP_Energy_Meter_01/pulse_t";// MQTT topic - pulse
const char* mqtt_topic_ip = "home/indoor/sensor/ESP_Energy_Meter_01/ip_t";      // MQTT topic - ip
const char* mqtt_topic_mac = "home/indoor/sensor/ESP_Energy_Meter_01/mac_t";    // MQTT topic - mac
const char* mqtt_topic_sub_1 = "home/indoor/sensor/ESP_Energy_Meter_01/pulse";  // MQTT topic subscribe #1
