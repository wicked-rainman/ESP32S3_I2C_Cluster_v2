#include "../../StampS3-Master-node/include/Cluster.h"
#include <Wire.h>
#include <WiFi.h>
void NetworkScan(void *summat_kinda_voidish);
void RollToFix(void *wideEmptySpace);
int SSID_storeUsed;
char ReturnVal[35];
static uint8_t BLUE_LIST[BLUE_LIST_SIZE][6]={
    {0xf4,0x12,0xfa,0x61,0x77,0x54},
    {0x08,0xb6,0x1f,0x87,0xf5,0x8c},
    {0xdc,0x54,0x75,0xd0,0x9d,0x50},
    {0xdc,0x54,0x75,0xd0,0x8f,0xB8},
    {0xf4,0x12,0xfa,0x9d,0x77,0x28},
    {0xf4,0x12,0xfa,0x9d,0x95,0x24},
    {0xf4,0x12,0xfa,0x9d,0x76,0x10},
    {0xf4,0x12,0xfa,0x9d,0x8c,0x1c}}; 
CRGB leds[NUM_LEDS];
uint8_t led_status = 0;

SemaphoreHandle_t xShmem;
SemaphoreHandle_t xUsb;
extern void I2CSlaveReceive(int);
extern void I2CSlaveRequest();
extern bool SsidStorePut(uint8_t *, char *, int32_t);
