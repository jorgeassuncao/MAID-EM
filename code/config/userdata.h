/**
 * File contents of "userdata.h" for MAID - Energy Monitor
 *
 * This file is used to change your personal user data to meet your needs
 * For more information visit https://github.com/jorgeassuncao/ESP8266-Energy-Monitor
 */

//************ CONFIG WIFI *******************************************************************************
// Configurations of your wifi network - Fixed IP is used because it's quicker to connect than DHCP
//********************************************************************************************************

// Wifi SSID (the name of your wifi network)
const char* ssid = "wifi_ssid";
// Wifi password (the password to access your network)
const char* password = "wifi_password";
// The fixed IP address you want for this node (must be unique inside your network - see above note)
IPAddress ip(192,168,1,222);
// DNS server IP address (usually the same as your router/gateway but can be another DNS server)
IPAddress dns(8,8,8,8);
// Gateway IP address (the address you use to access your router/gateway)
IPAddress gateway(192,168,1,1);
// Subnet mask IP address ()
IPAddress subnet(255,255,255,0);
// Hostname (the name you want to give to this node, must be unique inside your network)
#define host_name "your_hostname"

//************* CONFIG MQTT ******************************************************************************
// Configurations of your MQTT server -
//********************************************************************************************************

// MQTT server IP ou URL (the ip address or URL of your MQTT broker)
const char* mqtt_server = "mqtt_server_address";
// MQTT port (usually 1883)
int mqtt_port = 1883;
// MQTT user (the username of your MQTT broker)
const char* mqtt_username = "mqtt_user";
// MQTT password (the password of your MQTT broker)
const char* mqtt_password = "mqtt_password";

//************ MQTT TOPICS *******************************************************************************
// Configurations of your MQTT topics - to match your MQTT broker
//********************************************************************************************************

// MQTT topic to publish "watt" values
const char* mqtt_topic_watt = "home/indoor/sensor/wem_01/watt";
// MQTT topic to publish "kwh" values
const char* mqtt_topic_kwh = "home/indoor/sensor/wem_01/kwh";
// MQTT topic to publish "pulse/total kwh" values
const char* mqtt_topic_pulse = "home/indoor/sensor/wem_01/pulse";
// MQTT topic to publish IP address
const char* mqtt_topic_ip = "home/indoor/sensor/wem_01/ip";
// MQTT topic to publish MAC address
const char* mqtt_topic_mac = "home/indoor/sensor/wem_01/mac";
// MQTT topic to subscribe and get values when node starts
const char* mqtt_topic_sub_1 = mqtt_topic_pulse;

// END
