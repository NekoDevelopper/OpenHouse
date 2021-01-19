#include "OpenHouseMqtt.h"
OHMqttClass OHMqtt=OHMqttClass();
OHMqttHandler *Handlers[OHMqttHandlersCount];
int HandlersCount = 0;

void OHMqttClass::Reconnect(){
    static unsigned long int lastTry=millis();
    if(millis()-lastTry>=mqttReconnectionTime*1000){
        if(OHMqttClass::Mqtt.connected()==0){
            Serial.print("Connection to MQTT server:");
            Serial.println(OHMqttClass::Mqtt.connect(mqttClientId));
            lastTry=millis();
            for (uint8_t i=0; i < HandlersCount; i++){
                Serial.print("Subscribing to topic ");
                Serial.print(Handlers[i]->topic);
                Subscribe(Handlers[i]->topic);
                Serial.println(" Ok");
            }
        }
    }
}
void OHMqttClass::CallbackFunc(char* topic, byte* payload, unsigned int length){
    char buffer[200]="";
    for (uint8_t i=0; i < length; i++){
        buffer[i]=(char)payload[i];
    }
    Serial.print(topic);
    Serial.print("->");
    Serial.println(buffer);
    for (uint8_t i=0; i < HandlersCount;i++){
        if(strcmp(topic,Handlers[i]->topic)==0){
            Handlers[i]->func(buffer);
        }
    }
}
void OHMqttClass::Begin(char *server, int *port, char *id, unsigned int retryTime){
    OHMqttClass::mqttPort=port;
    OHMqttClass::mqttServer=server;
    OHMqttClass::mqttClientId=id;
    OHMqttClass::mqttReconnectionTime=retryTime;
    OHMqttClass::Mqtt.setClient(MqttClient);
    OHMqttClass::Mqtt.setServer(mqttServer,*mqttPort);
    OHMqttClass::Mqtt.setCallback(&CallbackFunc);
}

void OHMqttClass::Loop(){
    OHMqttClass::Mqtt.loop();
    OHMqttClass::Reconnect();
}
void OHMqttClass::Publish(char *topic, char *message){
    OHMqttClass::Mqtt.publish(topic,message);
}
void OHMqttClass::Subscribe(char *topic){
    OHMqttClass::Mqtt.subscribe(topic);    
}

OHMqttSender::OHMqttSender(char *Topic){
    OHMqttSender::topic=Topic;
}
void OHMqttSender::send(char *message){
    Serial.print(topic);
    Serial.print("<-");
    Serial.println(message);
    OHMqtt.Publish(topic,message);
}

OHMqttHandler::OHMqttHandler(char *Topic, void (*Func)(char* message)){
    Handlers[HandlersCount]=this;
    HandlersCount++;
    topic=Topic;
    func=Func;
}