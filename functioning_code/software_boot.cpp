/**
 * @file software_boot.txt
 * @author Tyler, Joey
 * @brief Uses software to boot up the BMS IC. Uses Pin #3 (any GPIO pin) and sets it to HIGH (will turn on BMS IC).
 *        No need to use physical boot button anymore. Uses i2c validation to check if successfully turned on
 *        (if i2c bus = created, BMS = ON, if i2c bus != created, BMS = OFF).
 */

#include <Wire.h>
#include <Arduino.h>

#define I2C_SDA 7
#define I2C_SCL 9
#define BOOT 3
#define SLAVE_ADDRESS 0x18

bool boot_flag = false;

void setup()
{
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.begin(115200);

}

void loop()
{
  // Boot up BMS w/ Software
  while (!boot_flag) {
    delay(1000);
    pinMode(BOOT, OUTPUT);
    // Serial.begin(115200);
    Serial.println("Booting Up from Setup...");
    delay(1000);

    digitalWrite(BOOT, HIGH);  // switch diode on
    Serial.println("LED turned on...");
    delay(3); // 3 ms = 3000 us 
    digitalWrite(BOOT, LOW);

    Wire.beginTransmission(SLAVE_ADDRESS);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.println("BMS IC has successfully booted up.");
      boot_flag = true;
    }
    else
      Serial.println("BMS IC: OFF.\n");

    delay(2000);

  }
  

}
