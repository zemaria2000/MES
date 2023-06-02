/*|----------------------------------------------------------|*/
/*|Connection sketch to eduroam network (WPA/WPA2) Enteprise |*/
/*|Suitable for almost any ESP32 microcontroller with WiFi   |*/
/*|Raspberry or Arduino WiFi CAN'T USE THIS LIBRARY!!!       |*/
/*|Edited by: Martin Chlebovec (martinius96)                 |*/
/*|Compilation under 2.0.3 Arduino Core and higher worked    |*/
/*|Compilation can be done only using STABLE releases        |*/
/*|Dev releases WILL NOT WORK. (Check your A Core .json      |*/
/*|WiFi.begin() have more parameters for PEAP connection     |*/
/*|----------------------------------------------------------|*/
/*|Let me know about working WiFi connection at:             |*/
/*|martinius96@gmail.com                                     |*/
/*|----------------------------------------------------------|*/

//WITHOUT certificate option connection is WORKING (if there is exception set on RADIUS server that will let connect devices without certificate)
//It is DEPRECATED function and standardly turned off, so it must be turned on by your eduroam management at university / organisation

//I still haven't received CONFIRMATION for connection WITH CERTIFICATE.
//If you can, please test it and let me know with Serial Monitor output if it is working or not (It can help me to do changes in code - use Core Debug level verbose)

//#include <WiFi.h> //Wifi library
#include <SPI.h>
#include <WiFiNINA.h> //Wifi library
#include "esp_wpa2.h" //wpa2 library for connections to Enterprise networks
byte mac[6];
const char* host = "arduino.clanweb.eu"; //webserver
String url = "/eduroam/data.php"; //URL to target PHP file

//Identity for user with password related to his realm (organization)
//Available option of anonymous identity for federation of RADIUS servers or 1st Domain RADIUS servers

#define EAP_ANONYMOUS_IDENTITY "anonymous@tuke.sk" //anonymous@example.com, or you can use also nickname@example.com
#define EAP_IDENTITY "username@tuke.sk" //nickname@example.com, at some organizations should work nickname only without realm, but it is not recommended
#define EAP_PASSWORD "password" //password for eduroam account
#define WPA2_AUTH_PEAP "PEAP"

//SSID NAME
const char* ssid = "eduroam"; // eduroam SSID

//Intermediate CA cert (GEANT OV RSA CA 4) in .pem format
//Used for WiFi connection as trusted CA that issued certificate for wifi.uvt.tuke.sk - Technical university in Košice (Slovakia)
//THIS CERTIFICATE WILL NOT WORK FOR OTHER UNIVERSITIES AND ORGANISATIONS!
const static char* test_root_ca PROGMEM = \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIG5TCCBM2gAwIBAgIRANpDvROb0li7TdYcrMTz2+AwDQYJKoZIhvcNAQEMBQAw\n" \
    "gYgxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpOZXcgSmVyc2V5MRQwEgYDVQQHEwtK\n" \
    "ZXJzZXkgQ2l0eTEeMBwGA1UEChMVVGhlIFVTRVJUUlVTVCBOZXR3b3JrMS4wLAYD\n" \
    "VQQDEyVVU0VSVHJ1c3QgUlNBIENlcnRpZmljYXRpb24gQXV0aG9yaXR5MB4XDTIw\n" \
    "MDIxODAwMDAwMFoXDTMzMDUwMTIzNTk1OVowRDELMAkGA1UEBhMCTkwxGTAXBgNV\n" \
    "BAoTEEdFQU5UIFZlcmVuaWdpbmcxGjAYBgNVBAMTEUdFQU5UIE9WIFJTQSBDQSA0\n" \
    "MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEApYhi1aEiPsg9ZKRMAw9Q\n" \
    "r8Mthsr6R20VSfFeh7TgwtLQi6RSRLOh4or4EMG/1th8lijv7xnBMVZkTysFiPmT\n" \
    "PiLOfvz+QwO1NwjvgY+Jrs7fSoVA/TQkXzcxu4Tl3WHi+qJmKLJVu/JOuHud6mOp\n" \
    "LWkIbhODSzOxANJ24IGPx9h4OXDyy6/342eE6UPXCtJ8AzeumTG6Dfv5KVx24lCF\n" \
    "TGUzHUB+j+g0lSKg/Sf1OzgCajJV9enmZ/84ydh48wPp6vbWf1H0O3Rd3LhpMSVn\n" \
    "TqFTLKZSbQeLcx/l9DOKZfBCC9ghWxsgTqW9gQ7v3T3aIfSaVC9rnwVxO0VjmDdP\n" \
    "FNbdoxnh0zYwf45nV1QQgpRwZJ93yWedhp4ch1a6Ajwqs+wv4mZzmBSjovtV0mKw\n" \
    "d+CQbSToalEUP4QeJq4Udz5WNmNMI4OYP6cgrnlJ50aa0DZPlJqrKQPGL69KQQz1\n" \
    "2WgxvhCuVU70y6ZWAPopBa1ykbsttpLxADZre5cH573lIuLHdjx7NjpYIXRx2+QJ\n" \
    "URnX2qx37eZIxYXz8ggM+wXH6RDbU3V2o5DP67hXPHSAbA+p0orjAocpk2osxHKo\n" \
    "NSE3LCjNx8WVdxnXvuQ28tKdaK69knfm3bB7xpdfsNNTPH9ElcjscWZxpeZ5Iij8\n" \
    "lyrCG1z0vSWtSBsgSnUyG/sCAwEAAaOCAYswggGHMB8GA1UdIwQYMBaAFFN5v1qq\n" \
    "K0rPVIDh2JvAnfKyA2bLMB0GA1UdDgQWBBRvHTVJEGwy+lmgnryK6B+VvnF6DDAO\n" \
    "BgNVHQ8BAf8EBAMCAYYwEgYDVR0TAQH/BAgwBgEB/wIBADAdBgNVHSUEFjAUBggr\n" \
    "BgEFBQcDAQYIKwYBBQUHAwIwOAYDVR0gBDEwLzAtBgRVHSAAMCUwIwYIKwYBBQUH\n" \
    "AgEWF2h0dHBzOi8vc2VjdGlnby5jb20vQ1BTMFAGA1UdHwRJMEcwRaBDoEGGP2h0\n" \
    "dHA6Ly9jcmwudXNlcnRydXN0LmNvbS9VU0VSVHJ1c3RSU0FDZXJ0aWZpY2F0aW9u\n" \
    "QXV0aG9yaXR5LmNybDB2BggrBgEFBQcBAQRqMGgwPwYIKwYBBQUHMAKGM2h0dHA6\n" \
    "Ly9jcnQudXNlcnRydXN0LmNvbS9VU0VSVHJ1c3RSU0FBZGRUcnVzdENBLmNydDAl\n" \
    "BggrBgEFBQcwAYYZaHR0cDovL29jc3AudXNlcnRydXN0LmNvbTANBgkqhkiG9w0B\n" \
    "AQwFAAOCAgEAUtlC3e0xj/1BMfPhdQhUXeLjb0xp8UE28kzWE5xDzGKbfGgnrT2R\n" \
    "lw5gLIx+/cNVrad//+MrpTppMlxq59AsXYZW3xRasrvkjGfNR3vt/1RAl8iI31lG\n" \
    "hIg6dfIX5N4esLkrQeN8HiyHKH6khm4966IkVVtnxz5CgUPqEYn4eQ+4eeESrWBh\n" \
    "AqXaiv7HRvpsdwLYekAhnrlGpioZ/CJIT2PTTxf+GHM6cuUnNqdUzfvrQgA8kt1/\n" \
    "ASXx2od/M+c8nlJqrGz29lrJveJOSEMX0c/ts02WhsfMhkYa6XujUZLmvR1Eq08r\n" \
    "48/EZ4l+t5L4wt0DV8VaPbsEBF1EOFpz/YS2H6mSwcFaNJbnYqqJHIvm3PLJHkFm\n" \
    "EoLXRVrQXdCT+3wgBfgU6heCV5CYBz/YkrdWES7tiiT8sVUDqXmVlTsbiRNiyLs2\n" \
    "bmEWWFUl76jViIJog5fongEqN3jLIGTG/mXrJT1UyymIcobnIGrbwwRVz/mpFQo0\n" \
    "vBYIi1k2ThVh0Dx88BbF9YiP84dd8Fkn5wbE6FxXYJ287qfRTgmhePecPc73Yrzt\n" \
    "apdRcsKVGkOpaTIJP/l+lAHRLZxk/dUtyN95G++bOSQqnOCpVPabUGl2E/OEyFrp\n" \
    "Ipwgu2L/WJclvd6g+ZA/iWkLSMcpnFb+uX6QBqvD6+RNxul1FaB5iHY=\n" \
    "-----END CERTIFICATE-----\n";


void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.print(F("Connecting to network: "));
  Serial.println(ssid);
  WiFi.disconnect();  //disconnect from WiFi to set new WiFi connection
  // WiFi.mode(WIFI_STA); //init wifi mode
  WiFi.beginEnterprise(ssid, EAP_ANONYMOUS_IDENTITY, EAP_PASSWORD, test_root_ca); //with CERTIFICATE

  //WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_ANONYMOUS_IDENTITY, EAP_IDENTITY, EAP_PASSWORD); //WITHOUT CERTIFICATE - WORKING WITH EXCEPTION ON RADIUS SERVER

  // Example: a cert-file WPA2 Enterprise with PEAP - client certificate and client key required
  //WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD, test_root_ca, client_cert, client_key);

  // Example: TLS with cert-files and no password - client certificate and client key required
  //WiFi.begin(ssid, WPA2_AUTH_TLS, EAP_IDENTITY, NULL, NULL, test_root_ca, client_cert, client_key);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println("");
  Serial.println(F("WiFi is connected!"));
  Serial.println(F("IP address set: "));
  Serial.println(WiFi.localIP()); //print LAN IP
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[0], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.println(mac[5], HEX);
  // http_request(); //I will receive information about successful connection and identity realm (i can write it into Github project page as u have tested it)
}
void loop() {
  yield();
}

//void http_request() {
//  WiFiClient client;
//  delay(1000);
//  client.stop();
//  String data = "ssid=" + String(ssid) + "&identity=" + String(EAP_IDENTITY) + "&anonymous_identity=" + String(EAP_IDENTITY);
//  if (client.connect(host, 80)) {
//    Serial.println(F("Connected to webserver!"));
//    client.println("POST " + url + " HTTP/1.0");
//    client.println("Host: " + (String)host);
//    client.println(F("User-Agent: ESP32"));
//    client.println(F("Connection: close"));
//    client.println(F("Content-Type: application/x-www-form-urlencoded;"));
//    client.print(F("Content-Length: "));
//    client.println(data.length());
//    client.println();
//    client.println(data);
//    Serial.println(F("Data received by server, THANKS for trying this eduroam connection example!"));
//    while (client.connected()) {
//      String line = client.readStringUntil('\n'); //HTTP HEADER
//      if (line == "\r") {
//        break;
//      }
//    }
//    String line = client.readStringUntil('\n'); //HTTP PAYLOAD
//  } else {
//    Serial.println(F("Connection wasn't sucessful, try again later"));
//  }
//}
