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
#include <SPI_Config.h>
#include <LCD_Config.h>

void setup()
{
  i2c_setup();
  // i2c_scanner();
  led_setup();
  boot_bms();
  switch_setup();

  i2c_write();

  // spi_setup();
  // spi_write();
  lcd_setup();
  lcd_display_ui();
}

void loop()
{
  button.loop();

  if (button.isPressed()){
    led_logging();
  }
}

