
/*
  0x08
  0x18
*/

#include <Wire.h>
#include <Arduino.h>

#define I2C_SDA 7
#define I2C_SCL 9
#define I2C_Freq 100000

TwoWire I2CBME = TwoWire(0);  // create i2c bus 

void setup() {
  Serial.begin(115200);
  Serial.println("Attempting to ping BMS IC...");
  I2CBME.begin(I2C_SDA, I2C_SCL, I2C_Freq); // I2C_Freq = clock freq (100000)
  


}


void loop() {

}
