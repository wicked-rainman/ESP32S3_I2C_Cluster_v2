#include "I2CServeFile.h"
//----------------------------------------------
// I2CServeFile
//
// Deliver a file over I2C as a slave device.
// This program is reentrant:
// Filestate 0, File to be transmitted is not opened
// Filestate 1, File has been open and is being read
// Filestate back to zero after an EOF.
//-------------------------------------------------
void I2CServeFile(char *reqstring) {
    static File f;
    static int Filestate=0;
    static char fname[30];
    char ch='\0';
    int i=0;
    leds[0]=0x0000ff;
    FastLED.show();
    if(Filestate == 0) {
        strncpy(fname,reqstring+5,30);
        if(SD.exists(fname)) {
            f=SD.open(fname,"r",false);
            strncpy(I2CSlaveReply,"File:ok\n",30);
            Filestate=1;   
        }
        else {
            strncpy(I2CSlaveReply,"File:NotOkay\n",30);
            Filestate=0;
        }
    }
    else {
        if(f.available()) {
            memset(I2CSlaveReply,0,MAX_I2C_BUFF_SIZE);
            while(f.available() && (ch!='\n') && (i<I2C_BUFFER_LENGTH-1)) {
                ch=f.read();
                I2CSlaveReply[i++]=ch;
            }
            I2CSlaveReply[i] = '\0';
        }
        else {
            strncpy(I2CSlaveReply,"EOF\n\0",30);
            f.close();
            Filestate=0; //Ready for another file
            MasterReady=true;
        }
    }
    leds[0]=0x000000;
    FastLED.show();
}

