#include "Cluster.h"
#include <WiFi.h>
void WiFiConnect();
extern bool find_home_network();
extern CRGB leds[NUM_LEDS];
extern uint8_t led_status;
extern void indicator(uint8_t,uint8_t,uint8_t);