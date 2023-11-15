  #include "WiFiConnect.h"
  void WiFiConnect() {
    IPAddress local_IP(192,168,1,1); //For AP mode
    IPAddress gateway(192,168,1,1);  //for AP mode
    IPAddress subnet(255,255,255,0); //for AP mode
    
    if(find_home_network()) {
        #ifdef GENERATE_SERIAL_OUTPUT
            USBSerial.printf("#Home network %s is available. Connecting\n#",HOME_SSID);
        #endif
    WiFi.begin(HOME_SSID,HOME_WIFI_PASSWORD);
    while(WiFi.status() != WL_CONNECTED) {
        #ifdef DEBUG
          USBSerial.print(".");
        #endif
        indicator(0,0,255);
        vTaskDelay(200);
        indicator(0,0,0);
        vTaskDelay(200);
    }
        #ifdef GENERATE_SERIAL_OUTPUT
            USBSerial.printf("\n#Connected to %s as ",HOME_SSID);
            USBSerial.println(WiFi.localIP());
        #endif
    }
    else { //Home network couldn't be found so create an AP
        indicator(16,16,16);
        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(local_IP, gateway, subnet);
        WiFi.softAP(AWAY_SSID,AWAY_WIFI_PASSWORD);
        #ifdef GENERATE_SERIAL_OUTPUT
            USBSerial.printf("#Home network %s not found. Created \"%s\" access point\n",HOME_SSID,AWAY_SSID);
            USBSerial.printf("#Gateway is ");
            USBSerial.println(WiFi.localIP());
        #endif
        indicator(0,0,0);
    }
  }