/**
 * @file software_boot.txt
 * @author Tyler, Joey
 * @brief Uses software to boot up the BMS IC. Uses Pin #21 (any GPIO pin) and sets it to HIGH (will turn on BMS IC).
 *        No need to use physical boot button anymore.
 */

#include <Wire.h>
#include <Arduino.h>

void setup()
{
  delay(1000);
  pinMode(21, OUTPUT);
  Serial.begin(115200);
  Serial.print("Booting Up from Setup...");
  delay(1000);

  digitalWrite(21, HIGH);  // switch diode on
  Serial.println("LED turned on...");
  delay(3); // 3 ms = 3000 us 
  digitalWrite(21, LOW);
}

void loop()
{}
