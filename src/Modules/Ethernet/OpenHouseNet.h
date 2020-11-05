#include <Arduino.h>
#include <Ethernet.h>

class OpenHouseNet{
    private:
        //Ethernet config
        byte *deviceMacAdress;
        IPAddress deviceIpArdess;
        IPAddress deviceSubnetIpAdress;
        IPAddress deviceDnsIpAdress;
        IPAddress deviceGatewayIpAdress;
        //
        bool isIpStatic; //if ip sets static
        bool isIpGet; //unused if ip adress is static
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
        OpenHouseNet(unsigned short int ethernetCsPin=10);
        void Loop();

};

OpenHouseNet::OpenHouseNet(unsigned short int ethernetCsPin){
    Ethernet.init(ethernetCsPin);
}

void OpenHouseNet::Loop(){
    if(OpenHouseNet::ethernetShieldChip!=0){
        switch (Ethernet.maintain()){
            case 1:
                Serial.println("Ethernet:DHCP filed");
            case 2:
                Serial.println("Ethernet:DHCP lease error");
                OpenHouseNet::PrintIpStatus();
            case 3:
                Serial.println("Ethernet:Rebind DHCP filed");
            case 4:
                Serial.println("Ethernet:Ip status updated");
                OpenHouseNet::PrintIpStatus();
            default:
                break;
        }
    }
}

void OpenHouseNet::PrintHardwareStatus(){
    OpenHouseNet::GetHardWareSetting();
    Serial.println("---Hardware status:");
    if(OpenHouseNet::ethernetShieldChip!=0){
        Serial.print("------Chip:W");
        Serial.println(OpenHouseNet::ethernetShieldChip);
        
        Serial.print("------MAC");
        for(int i=0;i<6;i++){
            Serial.print(":");
            Serial.print(OpenHouseNet::deviceMacAdress[i],HEX);
        }
        Serial.println();
    }else
        Serial.println("------No Ethernet chip found");
}
void OpenHouseNet::PrintIpStatus(){
    OpenHouseNet::GetIpSetting();
    Serial.println("---IP status");
    Serial.print("------Device IP Adress:");
    Serial.println(OpenHouseNet::deviceIpArdess);
    Serial.print("------Gateway IP Adress:");
    Serial.println(OpenHouseNet::deviceGatewayIpAdress);
    Serial.print("------DNS IP Adress:");
    Serial.println(OpenHouseNet::deviceDnsIpAdress);
    Serial.print("------Subnet Mask:");
    Serial.println(OpenHouseNet::deviceSubnetIpAdress);
}
void OpenHouseNet::PrintStatus(){
    OpenHouseNet::PrintHardwareStatus();
    OpenHouseNet::PrintIpStatus();
}

void OpenHouseNet::Begin(byte *mac){
    OpenHouseNet::deviceMacAdress=mac;
    Serial.println("Network");
    Serial.println("Start configuring ethernet using DHCP");
    OpenHouseNet::isIpGet=Ethernet.begin(mac);
    OpenHouseNet::PrintStatus();
}

void OpenHouseNet::Begin(byte *mac, byte *ip, byte *subnet, byte *dns, byte *gateway){
    OpenHouseNet::deviceMacAdress=mac;
    Serial.println("Network");
    Serial.println("Start configuring ethernet using static IP settings");
    Ethernet.begin(mac,ip,dns,gateway,subnet);
    OpenHouseNet::PrintStatus();
}

void OpenHouseNet::GetHardWareSetting(){
    if(Ethernet.hardwareStatus()==EthernetW5100)           OpenHouseNet::ethernetShieldChip=5100;    
    else if(Ethernet.hardwareStatus()==EthernetW5200)      OpenHouseNet::ethernetShieldChip=5200;             
    else if(Ethernet.hardwareStatus()==EthernetW5500)      OpenHouseNet::ethernetShieldChip=5500;
    else if(Ethernet.hardwareStatus()==EthernetNoHardware) OpenHouseNet::ethernetShieldChip=0;                
    
    if(Ethernet.linkStatus()==LinkON) OpenHouseNet::ethernetLinkStatus=1; //check link
    else if(Ethernet.linkStatus()==LinkOFF) OpenHouseNet::ethernetLinkStatus=0; //check link
    else if(Ethernet.linkStatus()==Unknown) OpenHouseNet::ethernetLinkStatus=2; //check link
    
}
void OpenHouseNet::GetIpSetting(){
    OpenHouseNet::deviceIpArdess=Ethernet.localIP();          //Ip Adress
    OpenHouseNet::deviceDnsIpAdress=Ethernet.dnsServerIP();   //DNS IP Adress
    OpenHouseNet::deviceSubnetIpAdress=Ethernet.subnetMask(); //Subnet mask
    OpenHouseNet::deviceGatewayIpAdress=Ethernet.gatewayIP(); //GateWay IP Adress
}