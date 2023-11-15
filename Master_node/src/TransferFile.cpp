#include "TransferFile.h"

bool TransferFile(int source, char* filename, int destination, char *reference) {
    int BytesAvailable;
    int i=0;
    uint8_t ch;
    char reply[MAX_I2C_BUFF_SIZE];

    //Send "FILE:"<filename> to the file node.
    //This instructs the file node to try and open the 
    //specified <filename>
    Wire.beginTransmission(source);
    Wire.printf("%s\n",filename);
    Wire.endTransmission();
    Wire.requestFrom(source,MAX_I2C_BUFF_SIZE);
    while(true) {
        //Block until a response is received from the file node
        while(Wire.available() == 0) vTaskDelay(50);
        while(Wire.available()) {
            ch = Wire.read();
            // A newline char is considered to be end of received record
            if(ch =='\n') {
                reply[i++]=0x0;
                #ifdef DEBUG
                    USBSerial.printf("#Xfer: \"%s\"\n",reply);
                #endif
                //If the record starts with File:Okay then throw it away
                if(!memcmp(reply,"File:ok",7)) {
                    memset(reply,0,MAX_I2C_BUFF_SIZE);
                    i=0;
                    break;
                }
                //If the received record starts with File:NotOkay, then file can't be transfered
                else if(!memcmp(reply,"File:NotOkay",12)) return false;
                //If the received record starts with the string EOF, then file transfer is complete
                 else if(!memcmp(reply,"EOF",3)) return true;
                //Else this is a record to pass to the destination node.
                else {
                    Wire.beginTransmission(destination); 
                    Wire.printf("%s:%s\n",reference,reply);        
                    Wire.endTransmission();
                    memset(reply,0,MAX_I2C_BUFF_SIZE);
                    i=0;
                    break;
                }
            }
            else {
                //This is just another character in the record
                reply[i++]=ch;
            }
        }
        //Repeat the file request to get the next record (if available)
        Wire.beginTransmission(source);
        Wire.printf("%s\n",filename);
        Wire.endTransmission();
        Wire.requestFrom(source,MAX_I2C_BUFF_SIZE);
    }
    //Never reach
    return false;
}