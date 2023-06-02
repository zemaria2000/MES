#include <Wire.h>

// defining the address of the TC74 (we can see it from the other sketch - i2c scanner
#define TC74_ADDRESS 0x4D

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
                    <item name="Humidity" value="232.27" upLim="250.000" loLim="220.000" checkType="5" unit="C" resultState="1" />
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
  // put your setup code here, to run once:

  Serial.begin(115200);
  // Initializing the I2C communication
  Wire.begin();


}

void loop() {
  // put your main code here, to run repeatedly:

  // ---------------------------- GETTING THE VALUE ----------------------------- //

  // Will do a request from the TC74 address to read the temperature
  Wire.requestFrom(TC74_ADDRESS, 1);
  while (Wire.available() == 0);

  byte temperature = Wire.read();

  String temperature_converted = String(temperature, DEC);

  telegram.replace("<item name=\"Humidity\" value=\"232.27\"", "<item name=\"Humidity\" value=\"" + temperature_converted + "\"");

  Serial.println(telegram);


  delay(5000);



  

}
