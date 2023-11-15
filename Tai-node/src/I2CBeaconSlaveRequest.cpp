#include "I2CBeaconSlave.h"
void I2CBeaconSlaveRequest() {
    Wire.printf("%s",TAIData);
    xSemaphoreGiveFromISR(xShmem,NULL);
}