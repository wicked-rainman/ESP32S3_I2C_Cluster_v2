#include "find_home_network.h"
bool find_home_network() {
    int16_t NetworkCount;
    int16_t ctr;
    NetworkCount = WiFi.scanNetworks(false,true,false);
    for (ctr = 0; ctr < NetworkCount; ctr++) {
        if(!memcmp(HOME_SSID,WiFi.SSID(ctr).c_str(),strlen(HOME_SSID))) return true;
    }
    return false;
}