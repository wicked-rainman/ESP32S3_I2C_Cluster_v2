  #include "WiFiConnect.h"
  void WiFiConnect() {
    IPAddress local_IP(192,168,1,1); //For AP mode
    IPAddress gateway(192,168,1,1);  //for AP mode
    IPAddress subnet(255,255,255,0); //for AP mode
    
    if(find_home_network(HOME_SSID)) {
        #ifdef DEBUG
            USBSerial.printf("#Home network %s is available. Connecting\n#",HOME_SSID);
        #endif
        WiFi.begin(HOME_SSID,HOME_WIFI_PASSWORD);
        while(WiFi.status() != WL_CONNECTED) {
            #ifdef DEBUG
                USBSerial.print(".");
            #endif
            ToggleLed(0x0000ff,0x000000,300);
        }
        #ifdef DEBUG
            USBSerial.printf("\n#Connected to %s as %s",HOME_SSID,I2CSlaveReply);
        #endif
    }
    else { //Home network couldn't be found
        #ifdef DEBUG
            USBSerial.printf("#Home network is unavailable. %s Connecting to %s\n#",AWAY_SSID);
        #endif
        WiFi.begin(AWAY_SSID,AWAY_WIFI_PASSWORD);
        while(WiFi.status() != WL_CONNECTED) {
            #ifdef DEBUG
                USBSerial.print(".");
            #endif
            ToggleLed(0x0000ff,0x000000,300);
        }
        #ifdef DEBUG
            USBSerial.printf("\n#Connected to %s as %s",AWAY_SSID,I2CSlaveReply);
        #endif
    }
  }