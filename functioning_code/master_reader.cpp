/**
 * @file master_reader.cpp
 * @author Tyler, Joey
 * @brief Uses I2C communication to read from specific BMS IC registers and print the output in the serial monitor.
 *        ex. In this code, it will read from BMS IC's register (0x09), and return the output 172 (0xAC), because 
 *            the register is set to 0x10101100, by default.
 */

#include <Wire.h>
#include <Arduino.h>

#define I2C_SDA 7
#define I2C_SCL 9
#define STOP    1

#define SLAVE_ADDRESS 0x18
#define REG_ADDRESS 0x09
 
void setup() {
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.begin(115200);
}

void loop() {
  //Read from REG_ADDRESS (0x09), expected value = 172 
  Serial.println("Reading init value...");
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(REG_ADDRESS);
  Wire.endTransmission();

  Wire.requestFrom(SLAVE_ADDRESS, 1);
  byte value = Wire.read();
  Serial.println(value);

  // TODO: Writing 0
  // Serial.println("Writing value of 0 to register 0x00... ");
  // Wire.beginTransmission(SLAVE_ADDRESS);
  // Wire.write(REG_ADDRESS);
  // int test_value = 0b00100000;
  // Wire.write(test_value);
  // Wire.endTransmission();

  // TODO: Reading 0 from Register
  // Serial.println("Reading new value... ");
  // Wire.beginTransmission(SLAVE_ADDRESS);
  // Wire.write(REG_ADDRESS);
  // Wire.endTransmission();

  // Wire.requestFrom(SLAVE_ADDRESS, 1);
  // char final_value = Wire.read();
  // Serial.println((byte)final_value);

  delay(2000);
}
