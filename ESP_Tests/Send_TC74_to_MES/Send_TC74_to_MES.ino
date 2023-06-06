#include <WiFi.h>
#include <cstdlib> // For malloc() and free() functions
#include <Wire.h>

#define TC74_ADDRESS 0x4D

// Some router and server configurations
const char* ssid = "José's Galaxy S21 5G";  // replace with your hotspot ID
const char* password = "lemc0316";    // replace with the respective password
const char* host = "193.137.172.20"; // server IP address

// WiFiClient object, which allows us to send data to a client via TCP/IP
WiFiClient client; 

// MES port that will receive the requests
const int httpPort = 55965; 

// XML with the "partProcessed" correct structure
String telegram = R"(
<?xml version="1.0" encoding="utf-8" standalone="yes"?>
<root>
    <header eventId="1" eventName="partProcessed" version="1.0" eventSwitch="-1" contentType="3">
        <location lineNo="9999" statNo="30" statIdx="1" fuNo="1" workPos="1" toolPos="1" application="PLC" processName="Final Functional Test" processNo="1260" />
    </header>
    <event>
        <partProcessed identifier="99992730000004445556660" />
    </event>
    <body>
        <structArrays>
            <array name="processRealData">
                <structDef>
                    <item name="name" dataType="8" />
                    <item name="value" dataType="4" />
                    <item name="upLim" dataType="4" />
                    <item name="loLim" dataType="4" />
                    <item name="checkType" dataType="3" />
                    <item name="unit" dataType="8" />
                    <item name="resultState" dataType="3" />
                </structDef>
                <values>
                    <item name="Temperature" value="232.27" upLim="250" loLim="0" checkType="5" unit="C" resultState="1" />
                </values>
            </array>
        </structArrays>
        <structs>
            <resHead result="1" typeNo="4445556660" typeVar="" workingCode="0" nioBits="0" cycleTimePrev="24453" />
        </structs>
    </body>
</root>
)";



void setup() {
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  
  Serial.print("Connecting to "); 
  Serial.println(ssid);

  // Establish connection to our router (hotspot)
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("ESP is connected via WiFi to the router, which gave him the following IP address: ");
  
  // IP address
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());

  // Start I2C communication with the TC74
  Wire.begin();
} // setup

  
void loop() {

 
  // ------------------- TRYING TO CONNECT VIA TCP/IP TO THE CLIENT ---------------------- //
  // We want to connect to the MES pc (193.137.172.20) and send requests to port 55965, which accepst the XML telegrams
  
   client.connect(host, httpPort);

   // While the connection is not established
   while(!client.connected()) {
      Serial.print("Connecting to ");
      Serial.println(host);
      delay(2000);
      client.connect(host, httpPort);
   }

   // Connection established successfully
   Serial.println("\n ESP is successfully connected with the server \n");
       
 
  // --------------------- IF WE HAVE AN ESTABLISHED TCP/IP CONNECTION --------------------------------------------- //
  
  if (client.connected()) {

    // -------------------------- GET TEMPERATURE VALUE ----------------------------- //
  
    // Making a request to the address previously configured as the TC74 temperature address
    Wire.requestFrom(TC74_ADDRESS, 1);
    while (Wire.available() == 0);
  
    // Save the temperature value in a byte
    byte temperature = Wire.read();
    // Convert it to a string
    String temperature_converted = String(temperature, DEC);

    // Replace in the appropriate place the temperatura value, in the XML to send
    telegram.replace("<item name=\"Temperature\" value=\"232.27\"", "<item name=\"Temperature\" value=\"" + temperature_converted + "\"");


    // -------------------------- SEND XML TO MES ---------------------------------- //    

    // Getting the size of the telegram to send
    int telegram_size_og = telegram.length(); // original size
    int telegram_size = telegram.length() + 4 ; // 4 more bytes because of the header
    Serial.println("\n Telegram Size: ");
    Serial.print(telegram_size);
      
    // Generating those 1st 4 bytes - formula from MES
    byte lolo = (telegram_size & 0xff);
    byte hilo = ((telegram_size >> 8) & 0xff);
    byte lohi = ((telegram_size >> 16) & 0xff);
    byte hihi = (telegram_size >> 24);

    // Defining the message header
    int telegramHeader[4] = {hihi, lohi, hilo, lolo};
  
    // Number of elements in the header
    int numel_telegramBytes = sizeof(telegramHeader)/sizeof(telegramHeader[0]);
  
    // Print out those 4 header bytes, just to make sure they were successfully created
    for(int i = 0; i < numel_telegramBytes ; i++){
      Serial.print(telegramHeader[i]);
      Serial.print(" ");
    }
    Serial.println("");
    
    // Convert string to char array
    int telegram_buf_len = telegram_size_og + 1; // Adding 1 to the size because of a termination byte
    Serial.print("Telegram_buf_len:");
    Serial.println(telegram_buf_len);
    
    char char_array[telegram_buf_len]; // Prepare an array where we'll put the entire telegram
   
    telegram.toCharArray(char_array,telegram_buf_len); // Convert the string to a char array
   
    // Store those bytes in the dynamicmemory of the ESP
    byte* telegramBytes = (byte*)malloc(telegram_size);

    if (telegramBytes != NULL) {
      
      // Copying the header to the bytes array
      for (int i = 0; i < numel_telegramBytes; i++) {
        telegramBytes[i] = telegramHeader[i];
      }

      // Copying every byte from the telegram char array to this new array to send
      for (int i = 0; i < telegram_size_og; i++) {
        int telegram_to_int = telegram[i] - '0' + 48; // Converting the bytes to decimal values
        telegramBytes[i + 4] = (byte)telegram_to_int;
      }

      // Sending those bytes to MES
      for (int i = 0; i < telegram_size; i++) {
        client.write(telegramBytes[i]);
      }

      // Eliminating the byte array sent from the ESP's dynamic memory. Just to free some space
      free(telegramBytes);
    } 
    
    else {
      Serial.println("Failed to allocate memory for telegramBytes.");
    }

  }   // IF (client.connected())

  Serial.print("Eait 10 secs for the next data injection");
  delay(10000);
}
