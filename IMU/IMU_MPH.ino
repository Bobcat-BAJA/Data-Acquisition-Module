#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

unsigned long lastTime = 0;
float velocity_mph = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.begin();
  Serial.println("Calibrating MPU...");
  delay(1000);
  mpu.calcOffsets();
  Serial.println("Calibration done.");
  lastTime = millis();
}

void loop() {
  mpu.update();

  unsigned long currentTime = millis();
  float deltaTime = (currentTime - lastTime) / 1000.0;  // seconds
  lastTime = currentTime;

  float accelX = mpu.getAccX();
  float accelY = mpu.getAccY();
  float accelZ = mpu.getAccZ();

  // Check if total acceleration magnitude is near 1g (means it's stationary)
  float totalAccel = sqrt(accelX * accelX + accelY * accelY + accelZ * accelZ);

  // Gravity magnitude is 1g, allow ±0.05g tolerance
  if (abs(totalAccel - 1.0) < 0.05) {
    velocity_mph = 0;  // Zero velocity if "not moving"
  } else {
    // Remove noise on X
    if (abs(accelX) < 0.02) {
      accelX = 0;
    }
    velocity_mph += (accelX * 9.80665) * deltaTime * 2.23694;
    if (velocity_mph < 0) velocity_mph = 0;
  }

  Serial.print("Velocity: ");
  Serial.print(velocity_mph);
  Serial.println(" mph");

  delay(1000);
}
