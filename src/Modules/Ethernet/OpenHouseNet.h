#pragma once
#include <Arduino.h>
#include <Ethernet.h>

class OHNetClass
{
private:
    //Ethernet config
    byte *deviceMacAdress;
    IPAddress deviceIpArdess;
    IPAddress deviceSubnetIpAdress;
    IPAddress deviceDnsIpAdress;
    IPAddress deviceGatewayIpAdress;
    //
    bool isIpStatic; //if ip sets static
    bool isIpGet;    //unused if ip adress is static
    //
    unsigned short int ethernetShieldChip;
    unsigned short int ethernetLinkStatus;
    //
    void GetIpSetting();
    void GetHardWareSetting();
    //
    void PrintHardwareStatus();
    void PrintIpStatus();
    void PrintStatus();

public:
    void Begin(byte *mac);
    void Begin(byte *mac, byte *ip, byte *subnet, byte *dns, byte *gateway);
    //
    void Loop();

};
extern OHNetClass OHNet;
