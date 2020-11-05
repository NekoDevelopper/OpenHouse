#include <Arduino.h>
#include "Modules/Ethernet/OpenHouseNet.h"
#include "Modules/Mqtt/Devices.h"
OpenHouseNet Network;
byte ethernetMac[6]={0x00,0x01,0x02,0x03,0x04,0x05};

EthernetClient MqttEthernetClient;
char MqttServerAdress[]="192.168.1.14";
char MqttClientId[]="Test";
int MqttServerPort=1883;

OpenHouseMqtt Mqtt;

char Sensor1Topic[]="/mdm/2";
OpenHouseMqttSensor Sensor1(&Mqtt,Sensor1Topic);

void setup() {
  Serial.begin(9600);
  Mqtt.Begin(&MqttEthernetClient,MqttServerAdress,&MqttServerPort,MqttClientId);
  Network.Begin(ethernetMac);
}
unsigned long int millissss=0;
char test[]="123";
void loop() {
  Network.Loop();
  Mqtt.Loop();
  if(millis()-millissss>=5000){
    Sensor1.Send(test);
    millissss=millis();
  }

}