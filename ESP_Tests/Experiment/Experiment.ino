#include <WiFi.h>
#include <cstdlib> // For malloc() and free()

const char* ssid = "José's Galaxy S21 5G";
const char* password = "lemc0316";
const char* host = "193.137.172.20"; // Servidor

WiFiClient client; // WiFiClient, permite enviar ou receber dados por TCP/IP

const int httpPort = 55965; //55965


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

 
  // ------------------- TENTA LIGAR POR TCP/IP ao cliente (neste caso o computador do MES) ---------------------- //
  // O objetivo é conectar ao host - ims.mec.ua.pt/193.137.172.20 - e enviar pedidos à porta 55965, associada ao MES
  
   client.connect(host, httpPort);

   while(!client.connected()) {
      Serial.print("Connecting to ");
      Serial.println(host);
      delay(2000);
      client.connect(host, httpPort);
   }

   // Conexão estabelecida
   Serial.println("\n ESP conectou-se com sucesso ao cliente \n");
       
 
  // --------------------- SE LIGACAO TCP ESTABELECIDA --------------------------------------------- //
  // Se já se conseguir ter a ligação devidamente estabelecida - ou seja, se client.connected() == TRUE
  
  if (client.connected()) {

    // Determinar o tamanho do xml a enviar para o telegram
    int telegram_size_og = telegram.length(); // tamanho original
    int telegram_size = telegram.length() + 4 ; // tamanho com mais 4 bytes. Isso porque é sempre necessário enviar um header com estes 4 bytes iniciais

    Serial.println("\n Telegram Size: ");
    Serial.print(telegram_size);
      
    // Geração dos primeiros 4 bytes - baseados numa fórmula definida pelo próprio MES...
    byte lolo = (telegram_size & 0xff);
    byte hilo = ((telegram_size >> 8) & 0xff);
    byte lohi = ((telegram_size >> 16) & 0xff);
    byte hihi = (telegram_size >> 24);

    // Definição do header da mensagem
    int telegramHeader[4] = {hihi, lohi, hilo, lolo};
  
    // Número de elementos no header??
    int numel_telegramBytes = sizeof(telegramHeader)/sizeof(telegramHeader[0]);
    Serial.println("\n\n");
    Serial.println("numel");
    Serial.println(numel_telegramBytes);
  
    // Fazer um print dos primeiros 4 bytes, só para confirmar que foram devidamente criados
    for(int i = 0; i < numel_telegramBytes ; i++){
      Serial.print(telegramHeader[i]);
      Serial.print(" ");
    }
    Serial.println("");
    
    // Converter a string para um char array
    int telegram_buf_len = telegram_size_og + 1; // O comprimento final da string será +1 devido à necessidade de se colocar um caracter de terminação
    Serial.print("Telegram_buf_len:");
    Serial.println(telegram_buf_len);
    
    char char_array[telegram_buf_len]; // Preparar então o array onde vão ser colocados todos os bytes
   
    telegram.toCharArray(char_array,telegram_buf_len); // Converter a string telegram para o array criado. 
    // É de salientar que faltam ainda alocar o header e o caracter de terminação


    // Guardar os bytes a enviar na memória dinâmica do ESP
    byte* telegramBytes = (byte*)malloc(telegram_size);

    if (telegramBytes != NULL) {
      
      // Copiamos agora o header para o array telegramBytes
      for (int i = 0; i < numel_telegramBytes; i++) {
        telegramBytes[i] = telegramHeader[i];
      }

      // De seguida copiamos todos os caracteres do XML (agora no charArray telegram) para dentro deste array
      for (int i = 0; i < telegram_size_og; i++) {
        int telegram_to_int = telegram[i] - '0' + 48; // Conversão para decimal
        telegramBytes[i + 4] = (byte)telegram_to_int;
      }

      // Enviamos os bytes para o cliente
      for (int i = 0; i < telegram_size; i++) {
        client.write(telegramBytes[i]);
      }

      // Eliminamos da memória dinâmica o array com os bytes enviados, de forma a libertar espaço
      free(telegramBytes);
    } 
    
    else {
      // Else apenas serve para quando não é possível criar o array telegramBytes
      Serial.println("Failed to allocate memory for telegramBytes.");
    }

  }   // IF (client.connected())

  Serial.print("Espera 10 segundos");
  delay(10000);
  // ...
}
