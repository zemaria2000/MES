#include  <WiFi.h>

extern "C" {
#include "user_interface.h"
#include "wpa2_enterprise.h"
#include "c_types.h"
}



uint8_t target_esp_mac[6] = {0x24, 0x0a, 0xc4, 0x9a, 0x58, 0x28};

// Configure the network settings
IPAddress ip(193, 137, 172, 45); // set your desired IP address
IPAddress gateway(193, 137, 172, 254); // set your router's IP address
IPAddress subnet(255, 255, 255, 0); // set your subnet mask
IPAddress dns(193, 136, 172, 20); // set your DNS server

// SSID to connect to
char ssid[] = "josemaria@ua.pt";
char username[] = "josemaria@ua.pt";
char identity[] = "josemaria@ua.pt";
char password[] = "OMSpeuh_2604";


void setup() {
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
  delay(1000);
  Serial.setDebugOutput(true);
  Serial.printf("SDK version: %s\n", system_get_sdk_version());
  Serial.printf("Free Heap: %4d\n",ESP.getFreeHeap());
  
  // Setting ESP into STATION mode only (no AP mode or dual mode)
  wifi_set_opmode(STATION_MODE);

  struct station_config wifi_config;

  memset(&wifi_config, 0, sizeof(wifi_config));
  strcpy((char*)wifi_config.ssid, ssid);
  strcpy((char*)wifi_config.password, password);

  wifi_station_set_config(&wifi_config);
  wifi_set_macaddr(STATION_IF,target_esp_mac);
  

  wifi_station_set_wpa2_enterprise_auth(1);

  // Clean up to be sure no old data is still inside
  wifi_station_clear_cert_key();
  wifi_station_clear_enterprise_ca_cert();
  wifi_station_clear_enterprise_identity();
  wifi_station_clear_enterprise_username();
  wifi_station_clear_enterprise_password();
  wifi_station_clear_enterprise_new_password();
  
  wifi_station_set_enterprise_identity((uint8*)identity, strlen(identity));
  wifi_station_set_enterprise_username((uint8*)username, strlen(username));
  wifi_station_set_enterprise_password((uint8*)password, strlen((char*)password));

  WiFi.config(ip, gateway, subnet, dns);
    
  wifi_station_connect();
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Serial.println("########################################################");
  delay(10000);
}
