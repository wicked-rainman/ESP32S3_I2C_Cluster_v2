#include "../../StampS3-Master-node/include/Cluster.h"
extern void  WiFiConnect();
void I2CSlaveListen(int);
void I2CSlaveRespond();
extern void I2CServeFile(char *);
extern void I2CStoreLog(char *);
extern char I2CSlaveReply[MAX_I2C_BUFF_SIZE];
extern char I2CSlaveReceived[MAX_I2C_BUFF_SIZE];
