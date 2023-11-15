#include "../../StampS3-Master-node/include/Cluster.h"
#include <Wire.h>
extern int oui_count;
extern char oui_array[][50];
extern char MasterReceiveMsg[MAX_I2C_BUFF_SIZE];
extern char MasterSentMsg[MAX_I2C_BUFF_SIZE];
extern CRGB leds[NUM_LEDS];
void I2CSlaveRequest();
void I2CSlaveReceive(int);
void FindMacDev(char *);