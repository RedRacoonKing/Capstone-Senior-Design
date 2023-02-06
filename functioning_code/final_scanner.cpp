
/**
 * @file i2c_scanner.cpp
 * @author Tyler, Joey
 * @brief In order to use I2C protocol between two devices, the master device needs to locate the slave address.
 *        This will traverse through all possible addresses to find the slave address.
 */

#include <Wire.h>
#include <Arduino.h>

#define I2C_SDA 7 // goes to A4 (data)
#define I2C_SCL 9 // goes to A5 (clock)
 
void setup() {
  Wire.begin(I2C_SDA, I2C_SCL); // creates communciation link via code
  Serial.begin(115200); // baud rate/transmission rate
  Serial.println("\nI2C Scanner");
}

void loop() {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;

  // scans through 128 different addresses to locate the address of the slave (in HEX)
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    // error = 0: success
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }

    // error = 4: other error 
    else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }

  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  delay(5000);          
}
