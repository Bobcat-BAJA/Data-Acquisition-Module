#include <Wire.h>
#include <MPU6050_light.h>

// Create MPU object
MPU6050 mpu(Wire);

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Initialize MPU6050
  byte status = mpu.begin();
  Serial.print("MPU6050 status: ");
  Serial.println(status);  // 0 = success
  
  if (status != 0) {
    Serial.println("MPU6050 initialization failed. Check connections!");
    while (1);  // Stop the program if it can't connect
  }

  Serial.println("Calibrating MPU...");
  delay(1000);
  mpu.calcOffsets();  // Gyro and accel calibration
  Serial.println("Calibration complete.");
}

void loop() {
  mpu.update();  // Update sensor readings

  // Print raw accelerometer and gyro data
  Serial.print("Acc X: "); Serial.print(mpu.getAccX());
  Serial.print("\tY: "); Serial.print(mpu.getAccY());
  Serial.print("\tZ: "); Serial.println(mpu.getAccZ());

  Serial.print("Gyro X: "); Serial.print(mpu.getGyroX());
  Serial.print("\tY: "); Serial.print(mpu.getGyroY());
  Serial.print("\tZ: "); Serial.println(mpu.getGyroZ());

  Serial.print("Temperature: "); Serial.print(mpu.getTemp()); Serial.println(" °C");

  delay(1000);  // Adjust delay as needed
}
