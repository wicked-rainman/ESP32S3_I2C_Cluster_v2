#include "../../StampS3-Master-node/include/Cluster.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include "GpsProc.h"
#include "I2CBeaconSlave.h"
char TAIData[40];
SemaphoreHandle_t xShmem;
CRGB leds[NUM_LEDS];