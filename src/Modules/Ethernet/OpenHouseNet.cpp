#include "OpenHouseNet.h"
OHNetClass OHNet = OHNetClass();

void OHNetClass::Loop()
{
    if (OHNetClass::ethernetShieldChip != 0)
    {
        switch (Ethernet.maintain())
        {
        case 1:
            Serial.println("Ethernet:DHCP filed");
        case 2:
            Serial.println("Ethernet:DHCP lease error");
            OHNetClass::PrintIpStatus();
        case 3:
            Serial.println("Ethernet:Rebind DHCP filed");
        case 4:
            Serial.println("Ethernet:Ip status updated");
            OHNetClass::PrintIpStatus();
        default:
            break;
        }
    }
}

void OHNetClass::PrintHardwareStatus()
{
    OHNetClass::GetHardWareSetting();
    Serial.println("---Hardware status:");
    if (OHNetClass::ethernetShieldChip != 0)
    {
        Serial.print("------Chip:W");
        Serial.println(OHNetClass::ethernetShieldChip);

        Serial.print("------MAC");
        for (int i = 0; i < 6; i++)
        {
            Serial.print(":");
            Serial.print(OHNetClass::deviceMacAdress[i], HEX);
        }
        Serial.println();
    }
    else
        Serial.println("------No Ethernet chip found");
}
void OHNetClass::PrintIpStatus()
{
    OHNetClass::GetIpSetting();
    Serial.println("---IP status");
    Serial.print("------Device IP Adress:");
    Serial.println(OHNetClass::deviceIpArdess);
    Serial.print("------Gateway IP Adress:");
    Serial.println(OHNetClass::deviceGatewayIpAdress);
    Serial.print("------DNS IP Adress:");
    Serial.println(OHNetClass::deviceDnsIpAdress);
    Serial.print("------Subnet Mask:");
    Serial.println(OHNetClass::deviceSubnetIpAdress);
}
void OHNetClass::PrintStatus()
{
    OHNetClass::PrintHardwareStatus();
    OHNetClass::PrintIpStatus();
}

void OHNetClass::Begin(byte *mac)
{
    Ethernet.init(8);
    OHNetClass::deviceMacAdress = mac;
    Serial.println("Network");
    Serial.println("Start configuring ethernet using DHCP");
    OHNetClass::isIpGet = Ethernet.begin(mac);
    OHNetClass::PrintStatus();
}

void OHNetClass::Begin(byte *mac, byte *ip, byte *subnet, byte *dns, byte *gateway)
{
    OHNetClass::deviceMacAdress = mac;
    Serial.println("Network");
    Serial.println("Start configuring ethernet using static IP settings");
    Ethernet.begin(mac, ip, dns, gateway, subnet);
    OHNetClass::PrintStatus();
}

void OHNetClass::GetHardWareSetting()
{
    if (Ethernet.hardwareStatus() == EthernetW5100)
        OHNetClass::ethernetShieldChip = 5100;
    else if (Ethernet.hardwareStatus() == EthernetW5200)
        OHNetClass::ethernetShieldChip = 5200;
    else if (Ethernet.hardwareStatus() == EthernetW5500)
        OHNetClass::ethernetShieldChip = 5500;
    else if (Ethernet.hardwareStatus() == EthernetNoHardware)
        OHNetClass::ethernetShieldChip = 0;

    if (Ethernet.linkStatus() == LinkON)
        OHNetClass::ethernetLinkStatus = 1; //check link
    else if (Ethernet.linkStatus() == LinkOFF)
        OHNetClass::ethernetLinkStatus = 0; //check link
    else if (Ethernet.linkStatus() == Unknown)
        OHNetClass::ethernetLinkStatus = 2; //check link
}
void OHNetClass::GetIpSetting()
{
    OHNetClass::deviceIpArdess = Ethernet.localIP();          //Ip Adress
    OHNetClass::deviceDnsIpAdress = Ethernet.dnsServerIP();   //DNS IP Adress
    OHNetClass::deviceSubnetIpAdress = Ethernet.subnetMask(); //Subnet mask
    OHNetClass::deviceGatewayIpAdress = Ethernet.gatewayIP(); //GateWay IP Adress
}