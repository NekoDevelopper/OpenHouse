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
void TestVoid(char *message){
  if(message[0]=='1'){
    digitalWrite(2,HIGH);
  }else if(message[0]=='0'){
    digitalWrite(2,LOW);
  }
}

char Relay1Topic[]="/mdm/1";
OpenHouseMqttRelay Relay1(&Mqtt,Relay1Topic,&TestVoid);

void subscribe(){
  Relay1.Subscribe();
}

char bufer[200];
void HandleMeeesgae(char* topic, byte* payload, unsigned int length){
  for(unsigned int i=0;i<length;i++){
    bufer[i]=(char)payload[i];
  }
  Relay1.Recive(bufer,topic);
}
void setup() {
  Serial.begin(9600);
  pinMode(2,OUTPUT);
  Mqtt.Begin(&MqttEthernetClient,&subscribe,&HandleMeeesgae,MqttServerAdress,&MqttServerPort,MqttClientId);
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