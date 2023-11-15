  #include "I2CBusCheck.h"
  void I2CBusCheck() {
    uint8_t k;
    bool toggle=true;
    for(k=85;k<90;k++) {
        Wire.beginTransmission(k);
        if(!Wire.endTransmission()) {
            #ifdef DEBUG
                USBSerial.printf("#Found I2c device: 0x%02x\n",k);
            #endif
        }
        else {
            #ifdef DEBUG
                USBSerial.printf("#Missing I2c device: 0x%02x\n",k);
            #endif
            while(true) {
                toggle = !toggle;
                toggle ? indicator(15,0,0) : indicator(0,0,0);
                vTaskDelay(200);
            }
        }
    }
    Wire.flush();
  }