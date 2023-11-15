#include "I2CSlave.h"

//-------------------------------------------------------------------------------
void I2CSlaveReceive(int fred) {
    int ctr;
    while(Wire.available()) MasterReceiveMsg[ctr++] = Wire.read();
    MasterReceiveMsg[ctr]='\0';
    if(!memcmp(MasterReceiveMsg,"OUI:",4)) {
        strncpy(oui_array[oui_count++],MasterReceiveMsg+4,MAX_I2C_BUFF_SIZE);
        #ifdef DEBUG
            USBSerial.printf("#OUIRec: \"%s\"\n",MasterReceiveMsg);
        #endif
    }
    else {
        #ifdef DEBUG
            USBSerial.printf("#Received: %02x:%02x:%02x\n",MasterReceiveMsg[0],MasterReceiveMsg[1],MasterReceiveMsg[2]);
        #endif    
        FindMacDev(MasterReceiveMsg);
    }
    #ifdef USE_LEDS
        leds[0] = 0x000f00;
        FastLED.show();
    #endif
    
}
//-------------------------------------------------------------------------------
void I2CSlaveRequest() {
    Wire.write(MasterSentMsg);
    #ifdef DEBUG
        USBSerial.printf("#Replied: %s",MasterSentMsg);
    #endif
    #ifdef USE_LEDS
    leds[0] = 0x000000;
    FastLED.show();
    #endif
}