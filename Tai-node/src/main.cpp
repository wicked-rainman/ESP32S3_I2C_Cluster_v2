#include "main.h"

void setup() {
  M5.begin(false,true,true,true);
  Wire.setPins(13,15);
  Wire.begin(0x58);
  delay(1200);  
  #ifdef DEBUG
    USBSerial.printf("\n#-----------------------\n#This is the TAI node\n#-----------------------\n#\n");
  #endif
  Serial.begin(9600);
  Serial.setPins(43,44);
  #ifdef USE_LEDS
    FastLED.addLeds<WS2812,LED, GRB>(leds, NUM_LEDS);
    leds[0]= 0x000000;
    FastLED.show();
  #endif
  Wire.onRequest(I2CBeaconSlaveRequest);
  xShmem = xSemaphoreCreateMutex();
  xSemaphoreGive(xShmem);
  xTaskCreatePinnedToCore(GpsProc,"GpsProc",5000,NULL,1,NULL,0);
}

void loop() {
  vTaskDelay(100);
}
