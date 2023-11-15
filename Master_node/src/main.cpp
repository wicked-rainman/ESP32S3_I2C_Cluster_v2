#include "main.h"

void setup() {

  pinMode(PIN_BUTTON, INPUT);
  USBSerial.begin(115200);
  vTaskDelay(1200); //This is how long Espressif seem to think the USB serial port takes to init.

  //
  // This is the I2C Master.                                                      
  // ------------------------                                                     
  // It's function is as follows:                                                 
  //                                                                              
  // 1. During startup:
  //    a. Setup an ISR for the single button. This will be used to instruct the
  //       cluster to start processing or stop.
  //    b. Set this controller to be the I2C Master.
  //    c. If available, connect to the WiFi HOME_SSID network, else create an AWAY_SSID network. 
  //    d. Scan the I2C buss to make sure all 5 node controllers can be seen.
  //    f. Get the file node to list the contents of the OUI file and transfer this to the 
  //       OUI node for storage.
  //    g. Start up a WiFi UDPBEACON_PORT that will be used to broadcast cluster results.                
  //    h. Read the oui csv stored on the FILE_NODE and populate the OUI_NODE lookup table with it's contents.
  //    i. Initialise two ring buffers that the concentrator uses to dedup output records.
  //    j. Turn the LED on red and wait for PIN_BUTTON to change state.
  //                                                                              
  // 2.Request the latest beacon packet via I2c from the Beacon node.             
  //  There is no guarantee that every beacon packet will be processed, but
  //  they are extracted from a circular buffer on the beacon node end and in 
  //  practice, very little(if any) seem to get lost.
  //
  // 3.Extract the three Mac addresses from the beacon packet
  //  and see if all three combined (18 bytes) have been seen before.
  //  We are only intrested in TA, so if it's a duplicate just ignore it.
  //
  // 4.For each of the first three Mac addresses in the beacon, check to
  // see if the address is rolling. If it is, mark it as such.
  // If it's not a rolling mac, ask the OUI node to look up the vendor,
  // then ask the network scanning node if there is an associated SSID.
  //
  // 5. Get a time and lat lon stamp from the TAI_NODE
  //
  // 6. Output all that gleamed information via the serial port and as a broadcast on UDP port 5128. 
  // The FILE_NODE will listen to this broadcast and save it in a log file.
  //
  #ifdef DEBUG
  USBSerial.printf("\n#-----------------------\n#This is the master node\n#-----------------------\n#\n");
  #endif
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  attachInterrupt(PIN_BUTTON,SetClusterState, RISING);
  FastLED.addLeds<WS2812,LED, GRB>(leds, NUM_LEDS);

  Wire.setPins(I2C_MASTER_SDA,I2C_MASTER_SCL);
  Wire.begin();
  WiFiConnect();
  I2CBusCheck();
  TransferFile(FILE_NODE,"FILE:/oui.txt",OUI_NODE,"OUI");
  beaconPort.begin(UDPBEACON_PORT);
  //
  // Initialise the two ring buffers used by the concentrator function (Dedupes records based   
  // on full or rolling mac addresses). For rolling macs, only the central 4 bytes are stored   
  // (They get put in the short buffer). Full macs get stuffed into the long buffer.            
  // If either get's found, then the beacon records are dropped as "seen before"                
  //
  memset(DedupeBufLong,0,DEDUPE_LONG_BUF_SIZE);
  memset(DedupeBufShort,0,DEDUPE_SHORT_BUF_SIZE);

  #ifdef DEBUG
    USBSerial.println("#Init complete. Waiting for button press");
  #endif
//
// Wait for a button press before running. This gives a chance for a new OUI table   
// to be NC'd up to the OUI node and allows for the Network scanner to learn for      
// a while. The longer the time delay before pressing the button the better. This    
// is because the scanner performs on-the-fly editing of SSIDs to resolve            
// rolling AP mac addresses. In practice, one or two mins before activation          
// seems a good time to chill.                                                       
//
  indicator(15,0,0);
  while (ClusterState=='S') vTaskDelay(60);
  indicator(0,0,0);
}
//-----------------------------------------------------------------------------

void loop() {
  static uint8_t Addr1[6];
  static uint8_t Addr2[6];
  static uint8_t Addr3[6];
  static char Addr1Oui[40];
  static char Addr2Oui[40];
  static char Addr3Oui[40];
  static char SsidStr1[200];
  static char SsidStr2[200];
  static char SsidStr3[200];
  static char AsocSsidStr1[200];
  static char AsocSsidStr2[200];
  static char AsocSsidStr3[200];

  static uint8_t ChannelNo;
  static uint8_t SequenceNo;
  static uint8_t BroadcastOui[3]={255,255,255};
  char beaconStr1[500];
  char beaconStr2[260];
  char beaconStr3[110];
  //-------------------------------------------------------------
  //Check to see if the controller button has been pressed.
  //If it has, the global variable ClusterState will contain
  //the single character 'S' for stop. In normal circumstances
  //after Setup has run, it will contain 'R' (for run)
  //If it contains 'S', send a UDP broadcast to tell
  //any WiFi connected clusters to stop processing.
  //-----------------------------------------------------------
  if(ClusterState=='S') {
    Wire.beginTransmission(FILE_NODE);
    Wire.printf("STOR:STOP\n");
    Wire.endTransmission();
    indicator(15,0,0);
    vTaskDelay(1000);    
  }
  else {
    //Cluster is in run mode.
    //If a new beacon packet is available
    if(FetchNextBeacon()) {
      // If we've not seen the three combined Mac addresses before,
      // then use this record.
      if(Concentrate(buf)) {
        indicator(0,15,0);
        memcpy(Addr1,buf,6);
        memcpy(Addr2,buf+6,6);
        memcpy(Addr3,buf+12,6);
        ChannelNo=buf[18];
        SequenceNo=buf[19];

        //
        //  Deal with Addr1 (Often broadcast)                               
        //
      
        if (!memcmp(Addr1,BroadcastOui,3)) {  //If it's a broadcast mac
          strcpy(SsidStr1,"NET_Unk");         //Don't look up the SSID
          strcpy(Addr1Oui,"Bcast");           //Don't look up the OUI
          strcpy(AsocSsidStr1,"NET_Unk");
        }
        else if(RollingMac(Addr1[0])) {       //If it's a rolling mac  
          strcpy(Addr1Oui,"Roll");            //Don't look up the OUI, set it to Roll
          ResolveSSID(SSID_MODE_SSID,Addr1,SsidStr1);        //Look for previous assoc with an SSID
          ResolveSSID(SSID_MODE_ASSOC,Addr1,AsocSsidStr1);        //Look up the SSID
        }
        else {                                //It's a normal mac
          ResolveDevice(Addr1,Addr1Oui);      //Look up the OUI
          ResolveSSID(SSID_MODE_SSID,Addr1,SsidStr1);        //Look up the SSID
          ResolveSSID(SSID_MODE_ASSOC,Addr1,AsocSsidStr1);        //Look up the SSID
        }

        //
        //  Deal with Addr2                                                
        //

        if(RollingMac(Addr2[0])) {            //If it's a rolling mac (no broadcasts in Addr2)
          strcpy(Addr2Oui,"Roll");            //Set OUI to Roll
          ResolveSSID(SSID_MODE_SSID,Addr2,SsidStr2);        //Look for an SSID match
          ResolveSSID(SSID_MODE_ASSOC,Addr2,AsocSsidStr2);        //Look up the SSID
        }
        else {                                // It's a normal mac.
          ResolveDevice(Addr2,Addr2Oui);      //Look up the OUI
          ResolveSSID(SSID_MODE_SSID,Addr2,SsidStr2);        //Look up the SSID
          ResolveSSID(SSID_MODE_ASSOC,Addr2,AsocSsidStr2);        //Look up the SSID

        }

        //
        //  Deal with Addr3 if it's not the same as Addr2                   
        //

        //if(memcmp(Addr2,Addr3,6)) {             //If Addr2 is different from Addr3
          if (!memcmp(Addr3,BroadcastOui,3)) {  //If it's a broadcast mac
            strcpy(Addr3Oui,"Bcast");           //Set OUI to roll
            strcpy(SsidStr3,"NET_Unk");         //Set SSID to Unknown
            strcpy(AsocSsidStr3,"NET_Unk");
          }
          else if(RollingMac(Addr3[0])) {       //If it's a rolling mac  
            strcpy(Addr3Oui,"Roll");            //Set OUI to roll
            ResolveSSID(SSID_MODE_SSID,Addr3,SsidStr3);        //Look up SSID
            ResolveSSID(SSID_MODE_ASSOC,Addr3,AsocSsidStr3);   //Look up the associated SSID
          }
          else {                                //It's a normal mac
            ResolveDevice(Addr3,Addr3Oui);      //Look up OUI
            ResolveSSID(SSID_MODE_SSID,Addr3,SsidStr3);        //Look up SSID
            ResolveSSID(SSID_MODE_ASSOC,Addr3,AsocSsidStr3);   //Look up the associated SSID
          }

        // Get a time stamp and Lat Lon
        GetTai();
  
        // Produce the output.
        // By default, the output is UDP broadcasted on port 5128.
        // Serial output is optional
  

        //Pass the record to the file node for storage
        //This needs to be broken down into chunks less than 128 chars as long records get cut off (Looks like an I2C bug).
        //A,15:55:29,5154.1566N,00205.3461W,ffffffffffff,9e126d630bc0,ffffffffffff,NET_Unk,Bcast,NET_Unk,Roll,NET_Unk,Bcast,NET_Unk,NET_Unk,VM8404955
        //A,15:56:36,5154.1589N,00205.3443W,ffffffffffff,ae54e1015b6f,ffffffffffff,NET_Unk,Bcast,NET_Unk,Roll,NET_Unk,Bcast,NET_Unk,NET_Unk,TALKTALK90DB97
        snprintf(beaconStr1,100,"%s,%02x%02x%02x%02x%02x%02x,%02x%02x%02x%02x%02x%02x,%02x%02x%02x%02x%02x%02x,\0",
          TaiString,
          Addr1[0],Addr1[1],Addr1[2],Addr1[3],Addr1[4],Addr1[5],
          Addr2[0],Addr2[1],Addr2[2],Addr2[3],Addr2[4],Addr2[5],
          Addr3[0],Addr3[1],Addr3[2],Addr3[3],Addr3[4],Addr3[5]);
        Wire.beginTransmission(FILE_NODE);
        Wire.printf("STOR:%s",beaconStr1);
        Wire.endTransmission();

        snprintf(beaconStr2,260,"%s,%s,%s,%s,%s,%s,\0",SsidStr1,Addr1Oui,SsidStr2,Addr2Oui,SsidStr3,Addr3Oui);
        Wire.beginTransmission(FILE_NODE);
        Wire.printf("%s",beaconStr2);
        Wire.endTransmission();

        snprintf(beaconStr3,110,"%s,%s,%s\n\0",AsocSsidStr1,AsocSsidStr2,AsocSsidStr3);
        Wire.beginTransmission(FILE_NODE);
        Wire.printf("%s",beaconStr3);
        Wire.endTransmission();


        //Broadcast the record over UDP
        strcat(beaconStr1,beaconStr2);
        strcat(beaconStr1,beaconStr3);
        UdpWrite(&beaconPort,SendIP,UDPBEACON_PORT,beaconStr1);
        #ifdef GENERATE_SERIAL_OUTPUT
          USBSerial.printf("%s",beaconStr1);
        #endif
      }
      indicator(0,0,0);
    }
  }
}