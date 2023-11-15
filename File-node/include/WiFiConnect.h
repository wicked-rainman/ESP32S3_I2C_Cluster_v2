#include "../../StampS3-Master-node/include/Cluster.h"
#include <WiFi.h>
#include "ToggleLed.h"
void WiFiConnect();
extern char I2CSlaveReply[MAX_I2C_BUFF_SIZE];
extern bool find_home_network(char *);
//extern CRGB leds[NUM_LEDS];
