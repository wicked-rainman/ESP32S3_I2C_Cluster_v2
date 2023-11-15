#include "../../StampS3-Master-node/include/Cluster.h"
#include <Wire.h>
void I2CBeaconSlaveRequest();
extern uint8_t RingBuffPos;
extern uint8_t BeaconStore[][BEACON_REC_SIZE];
extern SemaphoreHandle_t xShmem;
