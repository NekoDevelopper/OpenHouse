#include <Arduino.h>
#include <Ethernet.h>
#include <PubSubClient.h>
class OpenHouseMqtt{
    private:        
        void (*ReconnectFunc)();
        char *mqttClientId;
        int *mqttPort;
        char *mqttServer;
        unsigned int  mqttReconnectionTime;
        EthernetClient *MqttClient;                
        PubSubClient Mqtt;
        void Reconnect();
    public:
        void Begin(EthernetClient *ethClient,void (*reconnectFunc)(void), void (*callbackFunc)(char* topic, byte* payload, unsigned int length) , char *server, int *port, char *id, unsigned int retryTime=5);
        void Loop();
        void Publish(char *topic, char *message);
        void Subscribe(char *topic);

};


void OpenHouseMqtt::Reconnect(){
    static unsigned long int lastTry;
    if(millis()-lastTry>=mqttReconnectionTime*1000){
        if(OpenHouseMqtt::Mqtt.connected()==0){
            Serial.print("Connection to MQTT server:");
            Serial.println(OpenHouseMqtt::Mqtt.connect(mqttClientId));
            ReconnectFunc();
            lastTry=millis();
        }
    }
}

void OpenHouseMqtt::Begin(EthernetClient *ethClient,void (*reconnectFunc)(void), void (*callbackFunc)(char* topic, byte* payload, unsigned int length), char *server, int *port, char *id, unsigned int retryTime){
    OpenHouseMqtt::mqttPort=port;
    OpenHouseMqtt::mqttServer=server;
    OpenHouseMqtt::mqttClientId=id;
    OpenHouseMqtt::mqttReconnectionTime=retryTime;
    OpenHouseMqtt::ReconnectFunc=reconnectFunc;
    OpenHouseMqtt::Mqtt.setClient(*ethClient);
    OpenHouseMqtt::Mqtt.setServer(mqttServer,*mqttPort);
    OpenHouseMqtt::Mqtt.setCallback(callbackFunc);
}

void OpenHouseMqtt::Loop(){
    OpenHouseMqtt::Mqtt.loop();
    OpenHouseMqtt::Reconnect();
}
void OpenHouseMqtt::Publish(char *topic, char *message){
    OpenHouseMqtt::Mqtt.publish(topic,message);
}
void OpenHouseMqtt::Subscribe(char *topic){
    OpenHouseMqtt::Mqtt.subscribe(topic);    
}