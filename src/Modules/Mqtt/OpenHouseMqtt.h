#pragma once
#include <Arduino.h>
#include "../Ethernet/OpenHouseNet.h"
#include <PubSubClient.h>
#ifndef OHMqttHandlersCount
#define OHMqttHandlersCount 10
#endif

class OHMqttSender //Sender Class
{
    private:
        char* topic; // Topic
    public:
        OHMqttSender(char *Topic); // Class Constructor
        void send(char *message); //Send func

};
class OHMqttHandler //Handler Class
{
    public:
        OHMqttHandler(char *Topic, void (*Func)(char *message)); //Class Constructor
        void (*func)(char *mesage); // Function, called message recived from it's topic
        char *topic; //Topic to subscribe
};

class OHMqttClass{
    private:
        static void CallbackFunc(char* topic, byte* payload, unsigned int length); //Callback func
        char *mqttClientId;
        int *mqttPort;
        char *mqttServer;
        unsigned int mqttReconnectionTime;
        EthernetClient MqttClient;
        PubSubClient Mqtt;
        void Reconnect();

        
    public:
        void Begin(char *server, int *port, char *id, unsigned int retryTime=5);
        void Loop();
        void Publish(char *topic, char *message);
        void Subscribe(char *topic);
};
extern OHMqttClass OHMqtt;