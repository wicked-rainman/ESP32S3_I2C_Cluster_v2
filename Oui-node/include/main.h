#include "../../StampS3-Master-node/include/Cluster.h"
#include "I2CSlave.h"
#include "FoundMacDev.h"
char MasterSentMsg[MAX_I2C_BUFF_SIZE];
char MasterReceiveMsg[MAX_I2C_BUFF_SIZE];
bool MasterReady=false;
CRGB leds[NUM_LEDS];
char oui_array [MAX_OUI_TABLE_SIZE][50];
int oui_count=0;