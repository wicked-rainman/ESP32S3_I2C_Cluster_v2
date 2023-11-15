#include "indicator.h"
void indicator(uint8_t red,uint8_t green,uint8_t blue) {
    #ifdef USE_LEDS
        leds[0].r=red;
        leds[0].g=green;
        leds[0].b=blue;
        FastLED.show();
    #endif
}