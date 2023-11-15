#include "I2CStoreLog.h"
void I2CStoreLog(char *rec) {
    static char LogRec[MAX_I2C_BUFF_SIZE];
    static uint8_t state=0;
    static File f;
    strncpy(LogRec,rec+5,MAX_I2C_BUFF_SIZE);
    #ifdef USE_LEDS
    leds[0]=0x0000ff;
    #endif
    if(state==0) {
        if(!memcmp(LogRec,"STOP",4)) {
            #ifdef USE_LEDS
            leds[0]=0x0f0000;
            FastLED.show();
            #endif
            return;
        }
        #ifdef DEBUG
            USBSerial.println("#I2CStoreLog: Creating new log file.");
        #endif
        f=SD.open("/log.txt",FILE_WRITE,true);
        if(!f) {
             USBSerial.println("#I2CStoreLog: Could not open the log file");
            #ifdef USE_LEDS
            leds[0]=0x0f0000;
            FastLED.show();
            #endif
            return;
        }
        f.printf("%s\n",LogRec);
        state=1;
        return;
    }
    else {        

        if(!memcmp(LogRec,"STOP",4)) {
            #ifdef DEBUG
            USBSerial.println("#I2CStoreLog: Closing log file");
            #endif
            f.close();
            #ifdef USE_LEDS
            leds[0]=0x0f0000;
            FastLED.show();
            #endif
            state=0;
            return;
        }
        f.printf("%s\n",LogRec);
        #ifdef USE_LEDS
        leds[0]=0x000000;
        #endif
        return;
    }
}