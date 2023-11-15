#include "FoundMacDev.h"
void FindMacDev(char *MacOui) {
    static char MacStr[8]; 
    int i=0;

    snprintf(MacStr,7,"%02x%02x%02x",MacOui[0],MacOui[1],MacOui[2]);
    for(i=0;i < oui_count;i++) {
        if(!strncmp(MacStr, oui_array[i],6)) {
            strncpy(MasterSentMsg,oui_array[i]+7,30);
            strcat(MasterSentMsg,"\n");
            return;
        }
    }
    strncpy(MasterSentMsg,"OUI_Unk\n",(size_t) 9);
}
