#include <Arduino.h>
#include "Modules/Ethernet/OpenHouseNet.h"

OpenHouseNet Network;
byte ethernetMac[6]={0x00,0x01,0x02,0x03,0x04,0x05};
void setup() {
  Serial.begin(9600);
  Network.Begin(ethernetMac);
}

void loop() {
  // put your main code here, to run repeatedly:
}