#include "GpsProc.h"
//------------------------------------------------------------
// GpsProc. Only really interested in GNRMC sentences
//
//  1.  $GNRMC
//  2.  UTC at position
//  3.  Position status (A = data valid, V = data invalid)
//  4.  Latitude (DDmm.mm)
//  5.  Latitude direction: (N = North, S = South)
//  6.  Longitude (DDDmm.mm)
//  7.  Longitude direction: (E = East, W = West)
//  8.  Speed over ground, knots
//  9.  Track made good, degrees True
//  10. Date: dd/mm/yy
//  11. Magnetic variation, degrees
//  12. Magnetic variation direction E/W
//  13. Positioning system mode indicator
//  14. Check sum
//  15. [CR][LF]
//
// EG. "$GNRMC,080536.000,A,5154.1639,N,00205.3397,W,0.54,62.77,021023,,,A*5C"
//-----------------------------------------------------------

void GpsProc(void *tosh) {
  static char ch='\0';
  static bool toggle = false;
  static uint8_t SentenceSize=0;
  static char GpsSentence[MAX_GPS_SENTENCE_SIZE];
  extern char TAIData[40];
  static char hours[3]={'0','0',0x00};
  static char mins[3]={'0','0',0x00};
  static char secs[3]={'0','0',0x00};
  static char Latitude[11];
  static char Longitude[12];
  extern SemaphoreHandle_t xShmem;
  while(true) {
    while (ch != '$') {
      if (Serial.available()) ch = Serial.read();
      else vTaskDelay(100);
    }
    SentenceSize = 1;
    GpsSentence[0] = ch;
    while ((ch != '\n') && (SentenceSize < MAX_GPS_SENTENCE_SIZE)) {
        if (Serial.available()) {
            ch = Serial.read();
            GpsSentence[SentenceSize++] = ch;
        }
        else vTaskDelay(100);
    }

    GpsSentence[strlen(GpsSentence)-2] = '\0';
    if(!memcmp("$GNRMC",GpsSentence,6)) {
        memcpy(hours,GpsSentence+7,2);
        memcpy(mins,GpsSentence+9,2);
        memcpy(secs,GpsSentence+11,2);
        if(GpsSentence[18] == GOOD_GPS_FIX) {
            memcpy(Latitude,GpsSentence+20,9);
            Latitude[9]=GpsSentence[30];
            Latitude[10] = '\0';
            memcpy(Longitude,GpsSentence+32,10);
            Longitude[10]=GpsSentence[43];
            Longitude[11]='\0';
        }
        else {
            memcpy(Latitude,"0000.0000X\0",11);
            memcpy(Longitude,"00000.0000X\0",12);
        }
        #ifdef USE_LEDS
          toggle = !toggle;
          if(toggle) {
            if(GpsSentence[18] == GOOD_GPS_FIX) {
              leds[0]=0x000f00;
              FastLED.show();
            }
            else {
              leds[0]=0x0f0000;
              FastLED.show();
            }
          }
          else {
            leds[0] = 0x000000;
            FastLED.show();
          }
        #endif
        xSemaphoreTake(xShmem,portMAX_DELAY);
        snprintf(TAIData,40,"%c,%s:%s:%s,%s,%s\n",
          GpsSentence[18],hours,mins,secs,Latitude,Longitude);
        xSemaphoreGive(xShmem);
        USBSerial.printf("%s",TAIData);
    }
  }
}
