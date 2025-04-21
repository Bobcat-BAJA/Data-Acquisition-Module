#include <FlexCAN_T4.h>

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can2;
CAN_message_t msg;

void setup(void) {
  Serial.begin(115200);  // Initialize Serial Monitor
  while (!Serial);  // Wait for Serial to be ready

  can2.begin();
  can2.setBaudRate(500000);
  Serial.println("CAN2 initialized at 500000 baud.");
}

void loop() {
  if (can2.read(msg)) {
    Serial.print("CAN2 ");
    Serial.print("MB: "); Serial.print(msg.mb);
    Serial.print("  ID: 0x"); Serial.print(msg.id, HEX);
    Serial.print("  EXT: "); Serial.print(msg.flags.extended);
    Serial.print("  LEN: "); Serial.print(msg.len);
    Serial.print(" DATA: ");
    for (uint8_t i = 0; i < 8; i++) {
      Serial.print(msg.buf[i]); Serial.print(" ");
    }
    Serial.print("  TS: "); Serial.println(msg.timestamp);
  }
  else {
    Serial.println("No message received on CAN2.");
    delay(10);  // Add a delay to avoid flooding the serial monitor
  }
}
