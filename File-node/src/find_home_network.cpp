#include "find_home_network.h"
bool find_home_network(char *home_ssid) {
    int16_t NetworkCount;
    int16_t ctr;
    NetworkCount = WiFi.scanNetworks(false,true,false);
    for (ctr = 0; ctr < NetworkCount; ctr++) {
        if(!memcmp(home_ssid,WiFi.SSID(ctr).c_str(),strlen(home_ssid))) return true;
        vTaskDelay(100);
    }
    return false;
}
