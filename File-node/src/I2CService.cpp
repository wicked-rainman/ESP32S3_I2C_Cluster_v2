#include "I2CService.h"
void I2CSlaveListen(int fred) {
    uint8_t i=0;
    while(Wire.available()) I2CSlaveReceived[i++] = Wire.read();
    I2CSlaveReceived[i]='\0';
    #ifdef DEBUG
    USBSerial.printf("#Received:\"%s\"\n",I2CSlaveReceived);
    #endif
    if(!memcmp(I2CSlaveReceived,"FILE:",5)) I2CServeFile(I2CSlaveReceived);
    else if(!memcmp(I2CSlaveReceived,"STOR",4)) I2CStoreLog(I2CSlaveReceived);
    else USBSerial.printf("#Received: (Invalid request) \"%s\"\n", I2CSlaveReceived);
}


//-------------------------------------------------------------------------------
void I2CSlaveRespond() {
    Wire.printf("%s\n",I2CSlaveReply);
    #ifdef DEBUG
        USBSerial.printf("#Replied: %s",I2CSlaveReply);
    #endif
}