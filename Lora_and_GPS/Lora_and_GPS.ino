#include <Adafruit_GPS.h>

// —— Pin assignments ——
#define LORA_RX_PIN    0    // pin wired to LoRa TX
#define LORA_TX_PIN    1    // pin wired to LoRa RX

// —— UART definitions ——
#define LoRaSerial   Serial1
#define GPSSerial    Serial2

Adafruit_GPS GPS(&GPSSerial);
uint32_t lastGPSsend = 0;

void setup() {
  // bring up LoRa
  LoRaSerial.begin(115200);
  delay(100);
  LoRaSerial.println("AT+RESET");
  delay(200);
  flushLoRa();

  // bring up GPS
  GPSSerial.begin(9600);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  GPSSerial.println(PMTK_Q_RELEASE);
}

void loop() {
  // feed GPS parser
  while (GPSSerial.available()) {
    GPS.read();
  }

  // on each complete sentence, clear buffer so next is fresh
  if (GPS.newNMEAreceived() && GPS.parse(GPS.lastNMEA())) {
    while (GPSSerial.available()) {
      GPSSerial.read();
    }
  }

  // every 250 ms, if we have a fix, convert & send
  if (millis() - lastGPSsend > 250) {
    lastGPSsend = millis();

    if (GPS.fix) {
      // raw ddmm.mmmm → decimal degrees
      float rawLat = GPS.latitude;
      float rawLon = GPS.longitude;

      float latDeg = floor(rawLat/100.0);
      float latMin = rawLat - latDeg*100.0;
      float lonDeg = floor(rawLon/100.0);
      float lonMin = rawLon - lonDeg*100.0;

      float lat = latDeg + latMin/60.0;
      float lon = lonDeg + lonMin/60.0;

      if (GPS.lat == 'S') lat = -lat;
      if (GPS.lon == 'W') lon = -lon;

      // build payload "lat,lon" with max float precision
      char payload[32];
      int len = snprintf(payload, sizeof(payload),
                         "%.7f,%.7f", lat, lon);

      // send via LoRa AT command
      LoRaSerial.print("AT+SEND=0,");
      LoRaSerial.print(len);
      LoRaSerial.print(",");
      LoRaSerial.println(payload);

      // silently clear any LoRa replies
      delay(100);
      flushLoRa();
    }
  }
}

// discard any incoming LoRa bytes
void flushLoRa() {
  while (LoRaSerial.available()) {
    LoRaSerial.read();
  }
}
