#include <Arduino.h>
#include "Modules/Ethernet/OpenHouseNet.h"
#include "Modules/Mqtt/OpenHouseMqtt.h"

OpenHouseNet Network;
byte ethernetMac[6]={0x00,0x01,0x02,0x03,0x04,0x05};

EthernetClient MqttEthernetClient;
char MqttServerAdress[]="192.168.1.14";
char MqttClientId[]="Test";
int MqttServerPort=1883;

OpenHouseMqtt Mqtt;
void setup() {
  Serial.begin(9600);
  Mqtt.Begin(&MqttEthernetClient,MqttServerAdress,&MqttServerPort,MqttClientId);
  Network.Begin(ethernetMac);
}

void loop() {
  Network.Loop();
  Mqtt.Loop();
}