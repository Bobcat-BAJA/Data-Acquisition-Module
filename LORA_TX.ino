
uint8_t incomingByte = 0;

void setup() {
  Serial.begin(9600); //init communication over USB
  Serial2.begin(115200); //communication over RX/TX pins
  Serial.println("Ready");
  Serial2.println("AT+RESET");
  
  delay(100);
  while (Serial2.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial2.read();
    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte);
  }
  Serial.println("Sent");
}
int i = 0;

void loop() {

 delay(2000);
 
 Serial2.println("AT+SEND=0,8,G_is_GAY");
  while (Serial2.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial2.read();
    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte);
  }
}
