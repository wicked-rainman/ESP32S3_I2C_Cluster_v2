#include "main.h"

void setup() {
  uint8_t WaitCount=0;
  Wire.setPins(I2C_SLAVE_SDA,I2C_SLAVE_SCL);
  Wire.begin(0x57);
  USBSerial.begin(115200);
  vTaskDelay(2000);
  #ifdef DEBUG
    USBSerial.printf("\n#-----------------------\n#This is the OUI node\n#-----------------------\n#\n");
  #endif
  FastLED.addLeds<WS2812,LED, GRB>(leds, NUM_LEDS);
  leds[0]= 0x000000;
  FastLED.show();
  #ifdef DEBUG
    USBSerial.println("#Waiting for master node");
  #endif
  leds[0]= 0x000f00;
  FastLED.show();
  Wire.onRequest(I2CSlaveRequest);
  Wire.onReceive(I2CSlaveReceive);
}

void loop() { }