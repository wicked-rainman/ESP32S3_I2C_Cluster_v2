#include "../../StampS3-Master-node/include/Cluster.h"
#include <AsyncTCP.h>          
#include <ESPAsyncWebServer.h> 
#include <SPI.h>
#include <SD.h>
#include <FS.h>
#include "WiFiConnect.h"
#include "ToggleLed.h"
#include "find_home_network.h"
#include "I2CService.h"
#include "I2CServeFile.h"
#include "I2CStoreLog.h"
uint8_t BootState = 0;
bool MasterReady=false;
char I2CSlaveReceived[MAX_I2C_BUFF_SIZE];
char I2CSlaveReply[MAX_I2C_BUFF_SIZE];
CRGB leds[NUM_LEDS];
AsyncWebServer server(80);