#include <SPI.h>
#include <LoRa.h>
#include <TinyGPSPlus.h>

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 27

const char* gpsStream =
  "$GPRMC,045103.000,A,3014.1984,N,09749.2872,W,0.67,161.46,030913,,,A*7C\r\n"
  "$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n"
  "$GPRMC,045200.000,A,3014.3820,N,09748.9514,W,36.88,65.02,030913,,,A*77\r\n"
  "$GPGGA,045201.000,3014.3864,N,09748.9411,W,1,10,1.2,200.8,M,-22.5,M,,0000*6C\r\n"
  "$GPRMC,045251.000,A,3014.4275,N,09749.0626,W,0.51,217.94,030913,,,A*7D\r\n"
  "$GPGGA,045252.000,3014.4273,N,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F\r\n";

const char* node = "01";

TinyGPSPlus gps;

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  Serial.println(node);
  Serial2.begin(9600);
  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);

  //replace the LoRa.begin(---E-) argument with your location's frequency
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  //Send LoRa packet to receiver
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
  }
  while (Serial2.available() > 0) {
    gps.encode(Serial2.read());
  }
  Serial.println("PUEN STUPID");
  gpsInfo();

  // while (*gpsStream)
  //   if (gps.encode(*gpsStream++))
  //     gpsInfo();
  // gpsTest();  // GPS Test
  delay(10000);
}

void gpsInfo() {
  LoRa.beginPacket();
  LoRa.print(node);
  LoRa.print(F("#"));
  if (gps.location.isValid()) {
    LoRa.print(gps.location.lat(), 6);
    LoRa.print(F("#"));
    LoRa.print(gps.location.lng(), 6);
    LoRa.print(F("#"));
  } else {
    Serial.print(F("INVALID"));
  }

  if (gps.date.isValid()) {
    LoRa.print(gps.date.day());  // Day (1-31) (u8)
    LoRa.print(F("/"));
    LoRa.print(gps.date.month());  // Month (1-12) (u8)
    LoRa.print(F("/"));
    LoRa.print(gps.date.year());  // Year (2000+) (u16)
    LoRa.print(F("#"));
  } else {
    Serial.print(F("INVALID"));
  }

  if (gps.time.isValid()) {
    LoRa.print(gps.time.hour());  // Hour (0-23) (u8)
    LoRa.print(F(":"));
    LoRa.print(gps.time.minute());  // Minute (0-59) (u8)
    LoRa.print(F(":"));
    LoRa.print(gps.time.second());  // Second (0-59) (u8)
    LoRa.println(F(""));            // New Line for Next Packet
  } else {
    Serial.print(F("INVALID"));
  }
  LoRa.endPacket();
  Serial.println("PACKET SEND!");
}

void gpsTest() {
  // Serial GPS TEST
  Serial.print(node);
  Serial.print(F("#"));
  if (gps.location.isValid()) {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F("#"));
    Serial.print(gps.location.lng(), 6);
    Serial.print(F("#"));
  } else {
    Serial.print(F("INVALID"));
  }

  if (gps.date.isValid()) {
    Serial.print(gps.date.day());  // Day (1-31) (u8)
    Serial.print(F("/"));
    Serial.print(gps.date.month());  // Month (1-12) (u8)
    Serial.print(F("/"));
    Serial.print(gps.date.year());  // Year (2000+) (u16)
    Serial.print(F("#"));
  } else {
    Serial.print(F("INVALID"));
  }

  if (gps.time.isValid()) {
    Serial.print(gps.time.hour());  // Hour (0-23) (u8)
    Serial.print(F(":"));
    Serial.print(gps.time.minute());  // Minute (0-59) (u8)
    Serial.print(F(":"));
    Serial.print(gps.time.second());  // Second (0-59) (u8)
    Serial.println(F(""));            // New Line for Next Packet
  } else {
    Serial.print(F("INVALID"));
  }
}