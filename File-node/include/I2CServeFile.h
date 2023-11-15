#include "../../StampS3-Master-node/include/Cluster.h"
#include <SD.h>
#include <FS.h>
void I2CServeFile(char *);
extern bool MasterReady;
extern char I2CSlaveReply[MAX_I2C_BUFF_SIZE];
extern CRGB leds[NUM_LEDS];