#include <WiFi.h>
#include <cstdlib> // For malloc() and free()

const char* ssid = "José's Galaxy S21 5G";
const char* password = "lemc0316";
const char* host = "193.137.172.20"; // Servidor

WiFiClient client; // WiFiClient, permite enviar ou receber dados por TCP/IP

const int httpPort = 55965; //55965

//String telegram = "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?><root><header
//eventId=\"1\" eventName=\"partProcessed\" version=\"1.0\" eventSwitch=\"-1\"
//contentType=\"3\"><location lineNo=\"9999\" statNo=\"30\" statIdx=\"1\" fuNo=\"1\" workPos=\"1\"
//toolPos=\"1\" application=\"PLC\" processName=\"Final Functional Test\" processNo=\"1260\"
///></header><event><partProcessed identifier=\"99992730000004445556660\"
///></event><body><structArrays><array name=\"processRealData\"><structDef><item name=\"name\"
//dataType=\"8\" /><item name=\"value\" dataType=\"4\" /><item name=\"upLim\" dataType=\"4\"
///><item name=\"loLim\" dataType=\"4\" /><item name=\"checkType\" dataType=\"3\" /><item
//name=\"unit\" dataType=\"8\" /><item name=\"resultState\" dataType=\"3\"
///></structDef><values><item name=\"Temperature\" value=\"232.27\" upLim=\"250.000\"
//loLim=\"220.000\" checkType=\"5\" unit=\"C\" resultState=\"1\" /><item name=\"Humidity\"
//value=\"232.27\" upLim=\"250.000\" loLim=\"220.000\" checkType=\"5\" unit=\"%\"
//resultState=\"1\" /></values></array></structArrays><structs><resHead result=\"1\"
//typeNo=\"4445556660\" typeVar=\"\" workingCode=\"0\" nioBits=\"0\" cycleTimePrev=\"24453\"
///></structs></body></root>";


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
                    <item name="Blr_Gas_Pres_Max" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
                    <item name="Blr_Gas_Pres_Min" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
                    <item name="Blr_Supply" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
                    <item name="Bnr_Gas_Pres_Max" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
                    <item name="Bnr_Gas_Pres_Min" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
                    <item name="CH_Flow_NTC_Temp_Rise" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
                    <item name="CH_Flow_Press" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
                    <item name="CH_Flow_Rate" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
                    <item name="CO_Max" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
                    <item name="CO_Min" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
                    <item name="CO2_Max" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
                    <item name="CO2_Min" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
                    <item name="Code_Plug" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
                    <item name="Delta_CH_Flow_Temp_Cold" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
                    <item name="Delta_CH_Flow_Temp_Hot" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
                    <item name="Gas_Flow_Max" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
                    <item name="Gas_Flow_Min" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
                    <item name="Humidity" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
                    <item name="Pressure" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
                    <item name="Temperature" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
                </values>
            </array>
        </structArrays>
        <structs>
            <resHead result="1" typeNo="4445556660" typeVar="" workingCode="0" nioBits="0" cycleTimePrev="24453" />
        </structs>
    </body>
</root>
)";

//
//String telegram = "aaaa";


ICACHE_RAM_ATTR int telegramBytes[3940];



void setup() {
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  
  Serial.print("Connecting to "); 
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // O ESP envia por porta série, para o PC local esta informaçao:
  Serial.println("");
  Serial.println("O ESP está ligado por WiFi ao Router, o Router deu-lhe o IP ");
  
  // IP address
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP()); // Imprimir IP do ESP

} // setup

  
void loop() {

 
  // ------------------- TENTA LIGAR POR TCP/IP ao APACHE
  // Se o status da ligação TCP/IP entre o ESP e o computador remoto for igual a 0 (client.status= 0 - desligado)
  // Neste caso, o ESP tenta estabelecer ligacao TCP/IP com o Apache

   Serial.println("Olá 1");
   client.connect(host, httpPort);
   Serial.println("Olá 2");

   while(!client.connected()) {
      Serial.print("Connecting to ");
      Serial.println(host);
      delay(2000);
      client.connect(host, httpPort);
   }
       
 
  // ----------------- SE LIGACAO TCP ESTABELECIDA ...
  // Se o status da ligação for 4, significa que a ligação TCP/IP está estabelecida entre o ESP e o Apache

  if (client.connected()) {
    int telegram_size_og = telegram.length(); //original size
    int telegram_size = telegram.length() + 4 ; // size + 4 bytes

    Serial.println("Telegram Size: ");
    Serial.print(telegram_size);
      
    /*_________ creating 4 first bytes __________*/
    byte lolo = (telegram_size & 0xff);
    byte hilo = ((telegram_size >> 8) & 0xff);
    byte lohi = ((telegram_size >> 16) & 0xff);
    byte hihi = (telegram_size >> 24);
  
    int telegramHeader[4] = {hihi, lohi, hilo, lolo};
  
    // number of elements in array telegramBytes
    int numel_telegramBytes = sizeof(telegramHeader)/sizeof(telegramHeader[0]);
    Serial.println("\n\n");
    Serial.println("numel");
    Serial.println(numel_telegramBytes);
  
    // Print first 4 elements of telegramBytes
    for(int i = 0; i < numel_telegramBytes ; i++){
      Serial.print(telegramHeader[i]);
      Serial.print(" ");
    }
    Serial.println("");
    
    /* ---------- Converting string to char array -----------*/
    int telegram_buf_len = telegram_size_og + 1; // Length (with one extra character for the null terminator)
    Serial.print("Telegram_buf_len:");
    Serial.println(telegram_buf_len);
    
    char char_array[telegram_buf_len]; // Prepare the character array (the buffer)
//
//    Serial.print("char_array");
//    Serial.println(char_array[telegram_buf_len]);
    
    telegram.toCharArray(char_array,telegram_buf_len); // Copy it over

/* ---------- Dynamic memory allocation for telegramBytes -----------*/
    byte* telegramBytes = (byte*)malloc(telegram_size);

    if (telegramBytes != NULL) {
      // Copy the telegramHeader to the telegramBytes array
      for (int i = 0; i < numel_telegramBytes; i++) {
        telegramBytes[i] = telegramHeader[i];
        Serial.println(telegramBytes[i]);
      }

      // Copy the XML string bytes to the telegramBytes array
      for (int i = 0; i < telegram_size_og; i++) {
        int telegram_to_int = telegram[i] - '0' + 48; // Convert to decimal
        telegramBytes[i + 4] = (byte)telegram_to_int;
      }

      // Write the bytes to the client
      for (int i = 0; i < telegram_size; i++) {
        client.write(telegramBytes[i]);
      }

      // Free the allocated memory
      free(telegramBytes);
    } else {
      // Memory allocation failed
      Serial.println("Failed to allocate memory for telegramBytes.");
    }

    // ...
  }

  Serial.print("Espera 10 segundos");
  delay(10000);
  // ...
}



    
//    int telegram_size = telegram.length() + 4 ; // size + 4 bytes
//
//    Serial.println("Telegram Size: ");
//    Serial.print(telegram_size);
//      
//    /*_________ creating 4 first bytes __________*/
//    byte lolo = (telegram_size & 0xff);
//    byte hilo = ((telegram_size >> 8) & 0xff);
//    byte lohi = ((telegram_size >> 16) & 0xff);
//    byte hihi = (telegram_size >> 24);
//  
//    int telegramHeader[4] = {hihi, lohi, hilo, lolo};
//  
//    // number of elements in array telegramBytes
//    int numel_telegramBytes = sizeof(telegramHeader)/sizeof(telegramHeader[0]);
//    Serial.println("\n\n");
//    Serial.println("numel");
//    Serial.println(numel_telegramBytes);
//  
//    // Print first 4 elements of telegramBytes
//    for(int i = 0; i < numel_telegramBytes ; i++){
//      Serial.print(telegramHeader[i]);
//      Serial.print(" ");
//    }
//    Serial.println("");
//    
//    /* ---------- Converting string to char array -----------*/
//    int telegram_buf_len = telegram_size_og + 1; // Length (with one extra character for the null terminator)
//    Serial.print("Telegram_buf_len:");
//    Serial.println(telegram_buf_len);
//    
//    char char_array[telegram_buf_len]; // Prepare the character array (the buffer)
////
////    Serial.print("char_array");
////    Serial.println(char_array[telegram_buf_len]);
//    
//    telegram.toCharArray(char_array,telegram_buf_len); // Copy it over
//    
//    /* ---------- Add telegramHeader to telegramBytes -----------*/
//    // int telegramBytes[telegram_size];
//    // int telegramBytes[1000];
//    // Add telegram bytes after first 4 bytes
//    for(int i = 0; i < numel_telegramBytes; i++){
//      telegramBytes[i] = telegramHeader[i];
//      Serial.println(telegramBytes[i]);
//    }
//
//    /* ---------- Add xml chars to telegramBytes -----------*/
//    // Add telegram bytes after first 4 bytes
//    for(int i = 0; i < telegram_size_og; i++){
//      int telegram_to_int = telegram[i] - '0' + 48; //convert to decimal
//      telegramBytes[i+4] = telegram_to_int;
//    }
//    
////    for(int i = 0; i < telegram_size ; i++){
////      Serial.print(telegramBytes[i]);
////      Serial.print(" ");
////    }
//    
//    Serial.println(" ");
//    
//  for (int i = 0; i < telegram_size; i++) {
//    client.write((byte)telegramBytes[i]);
//  }
//      
//    Serial.println("Enviou XML");
//    // Serial.println(telegram);
//  } // IF (client.connected())
//
//  // client.stop();


// End of Loop
