#include <RF24.h>
#include <SPI.h>
#include <VESC.h>

//#define DEBUG

typedef struct {
  uint8_t x;
  uint8_t y;
  bool c;
  bool z;
  uint8_t vBat;
} RemoteData;

RemoteData remotedata;
chuck_data chuckdata;
const byte addresses[][6] = {"PB001","PR001"};

RF24 radio(PA3, PA4);

void vescSetup() {
  VESC_init();
  chuckdata.js_x = 127;
  chuckdata.js_y = 127;
  chuckdata.bt_c = false;
  chuckdata.bt_z = false;
  chuckdata.acc_x = 0;
  chuckdata.acc_y = 0;
  chuckdata.acc_z = 0;
}

void setup() {
#ifdef DEBUG
  // Serial Setup (Debug only)
  Serial.begin(115200);
#endif

  // Radio Setup
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);
  radio.startListening();

  // Initialise Data
  remotedata.x = 127;
  remotedata.y = 127;
  remotedata.c = false;
  remotedata.z = false;
  remotedata.vBat = 0.0;

  vescSetup();
}

void loop() {
  if (radio.available()){
    radio.read(&remotedata, sizeof(RemoteData));
    chuckdata.js_x = remotedata.x;
    chuckdata.js_y = remotedata.y;
    chuckdata.bt_c - remotedata.c;
    chuckdata.bt_z = remotedata.z;
    VESC_setChuckData(&chuckdata);
  }

#ifdef DEBUG
  Serial.print("x: ");
  Serial.print(remotedata.x);
  Serial.print(" y: ");
  Serial.print(remotedata.y);
  Serial.print(" c: ");
  Serial.print(remotedata.c);
  Serial.print(" z: ");
  Serial.print(remotedata.z);
  Serial.print(" vBat: ");
  Serial.println(remotedata.vBat*5.0/255.0);
#endif
}
