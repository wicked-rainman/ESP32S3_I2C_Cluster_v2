#include "main.h"

void setup() {
  uint64_t cardSize;
  uint8_t cardType;
  uint8_t WaitCount=0;
  M5.begin(false,true,false,true);

  delay(1200);
  #ifdef DEBUG
    USBSerial.printf("\n#-----------------------\n#This is the FILE node\n#-----------------------\n#\n");
  #endif
  //
  //Set up node as I2C slave
  //
  Wire.setPins(I2C_SLAVE_SDA,I2C_SLAVE_SCL);
  Wire.begin(0x59);
  //
  //Set up SPI interface for the SD card
  //SPI.begin(Clock, MISO, MOSI, ChipSelect);
  //SPI clk,9 miso,7 mosi,5 cs, 3
  //
  SPI.begin(9,7,5,3);
  //
  //Set up the status LED
  //
  FastLED.addLeds<WS2812,LED, GRB>(leds, NUM_LEDS);
  leds[0]= 0x000000;
  FastLED.show();
  #ifdef DEBUG
    USBSerial.printf("#Total heap: %d\n", ESP.getHeapSize());
    USBSerial.printf("#Free heap: %d\n", ESP.getFreeHeap());
    USBSerial.printf("#Total SPIFLASH: %d\n", spi_flash_get_chip_size());
  #endif
  //
  //Start up the SD
  //
  if(!SD.begin(3)) {
    USBSerial.println("#SD begin failed");
    while(1);
  }
  #ifdef DEBUG
    else USBSerial.println("#SD mount ok");
  #endif
  cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("#No SD card attached");
    while(1);
  }
  #ifdef DEBUG
    USBSerial.print("#SD Card Type: ");
    if(cardType == CARD_MMC) USBSerial.println("MMC");
    else if(cardType == CARD_SD) USBSerial.println("SDSC");
    else if(cardType == CARD_SDHC) USBSerial.println("SDHC");
    else USBSerial.println("UNKNOWN");
    cardSize = SD.cardSize() / (1024 * 1024);
    USBSerial.printf("#SD Card Size: %lluMB\n", cardSize);
  #endif
  //
  //Wait for the Master node to choose or create a WiFi network to use,
  //then deliver startup functios
  //
  Wire.onReceive(I2CSlaveListen);
  Wire.onRequest(I2CSlaveRespond);
  while(!MasterReady) vTaskDelay(100);
  WiFiConnect();
  USBSerial.println("#Boot init completed");
  //
  //Start up the async web server
  //
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Yes, this is a web server");
  });
  server.on("/cute", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD,"/cute.log", "text/html");
  });
  server.begin();
  //
  //Set the status LED to green. Startup is finished.
  //
  leds[0]= 0x000f00;
  FastLED.show();
}

void loop() {
  vTaskDelay(100);
}

