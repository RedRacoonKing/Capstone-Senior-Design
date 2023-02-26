// /**
//  * @file main.cpp
//  * @author Tyler, Joey
//  * @brief Firmware for Aztec Electric Racing GLV
//  */

#include <Wire.h>
#include <Arduino.h>
#include <LED_Config.h>
#include <I2C_Config.h>
#include <Boot_Config.h>

void setup()
{
  i2c_setup();
  i2c_scanner();
  // led_setup();
  // boot_bms();
  // switch_setup();
}

void loop()
{
  button.loop();

  if (button.isPressed()){
    led_logging();
  }
  // i2c read/write here
}

