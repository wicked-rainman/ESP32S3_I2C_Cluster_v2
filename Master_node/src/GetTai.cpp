#include "GetTai.h"
void GetTai() {
    extern char TaiString[41];
    uint8_t BytesAvailable;
    BytesAvailable=Wire.requestFrom(TAI_NODE,40);
    Wire.readBytes(TaiString,BytesAvailable);
    TaiString[33]='\0';
}