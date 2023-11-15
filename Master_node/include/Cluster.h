#include <M5AtomS3.h>
#include <FastLED.h>

// Wifi defines
#define HOME_SSID "SomeSSID"
#define HOME_WIFI_PASSWORD "SSIDPassword"
#define AWAY_SSID "Blue"
#define AWAY_WIFI_PASSWORD NULL
#define SSID_MODE_RSSI 2 
#define SSID_MODE_SSID 0
#define SSID_MODE_ASSOC 1

//GPS defines
#define GOOD_GPS_FIX 'A'
#define BAD_GPS_FIX 'V'

//Delay defines
#define SCAN_INTERVAL 2000
#define ROLL_FIX_DELAY 5000
#define FILE_INITIAL_READ_DELAY 2000
#define WIFI_CHANNEL_SWITCH_INTERVAL  (300)

//Variable dimension defines
#define BLUE_LIST_SIZE 8
#define MAX_SSID_STORE_SIZE 200
#define MAX_SSID_LENGTH 35
#define NODE_WIFI_JOIN_WAIT 8000
#define BEACON_CACHE_SIZE 256
#define BEACON_REC_SIZE 20
#define CHANNEL_COUNT 20
#define DEDUPE_SHORT_BUF_SIZE 1000
#define DEDUPE_LONG_BUF_SIZE 5000
#define BEACON_RECORD_SIZE 200
#define TAIDATA_SIZE 40
#define MAX_GPS_SENTENCE_SIZE 100
#define MAX_I2C_BUFF_SIZE 200
#define MAX_OUI_TABLE_SIZE 5000

//Input/Output control
#define I2C_SLAVE_SDA 13
#define I2C_SLAVE_SCL 15
#define I2C_MASTER_SDA 13
#define I2C_MASTER_SCL 15
#define BEACON_NODE 0x55
#define NETWORK_SCAN_NODE 0x56
#define OUI_NODE 0x57
#define TAI_NODE 0x58
#define FILE_NODE 0x59
#define LED 21
#define NUM_LEDS 1
#define PIN_BUTTON 0
#define CLUSTER_STOP 'S'
#define CLUSTER_START 'R'
#define USE_LEDS
#define UDPBEACON_PORT 5128
#define GENERATE_SERIAL_OUTPUT
#define DEBUG
