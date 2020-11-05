#include <Arduino.h>
#include <Ethernet.h>
#include <PubSubClient.h>

class OpenHouseMqtt{
    private:
        char *mqttClientId;
        int *mqttPort;
        char *mqttServer;
        unsigned int  mqttReconnectionTime;
        EthernetClient *MqttClient;                
        PubSubClient Mqtt;
        void Reconnect();
        void Callback();
    public:
        void Begin(EthernetClient *ethClient, char *server, int *port, char *id, unsigned int retryTime=5);
        void Loop();
        void Publish(char *topic, char *message);
        

};


void OpenHouseMqtt::Reconnect(){
    static unsigned long int lastTry;
    if(millis()-lastTry>=mqttReconnectionTime*1000){
        if(Mqtt.connected()==0){
            Serial.print("Connection to MQTT server:");
            Serial.println(Mqtt.connect(mqttClientId));
            lastTry=millis();
        }
    }
}

void OpenHouseMqtt::Begin(EthernetClient *ethClient, char *server, int *port, char *id, unsigned int retryTime){
    OpenHouseMqtt::mqttPort=port;
    OpenHouseMqtt::mqttServer=server;
    OpenHouseMqtt::mqttClientId=id;
    OpenHouseMqtt::mqttReconnectionTime=retryTime;
    OpenHouseMqtt::Mqtt.setClient(*ethClient);
    OpenHouseMqtt::Mqtt.setServer(mqttServer,*mqttPort);
}

void OpenHouseMqtt::Loop(){
    OpenHouseMqtt::Mqtt.loop();
    OpenHouseMqtt::Reconnect();
}
void OpenHouseMqtt::Publish(char *topic, char *message){
    OpenHouseMqtt::Mqtt.publish(topic,message);
}