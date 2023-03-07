// /**
//  * @file main.cpp
//  * @author Tyler, Joey
//  * @brief Firmware for Aztec Electric Racing GLV
//  */

#include <Wire.h>
#include <Arduino.h>
#include <LED_Config.h>
#include <BMS_Config.h>
#include <Boot_Config.h>
#include <SD_Config.h>
#include <LCD_Config.h>

void setup()
{
  delay(3000);
  i2c_setup();
  // Serial.begin(115200);
  // delay(1000);
  // i2c_scanner();
  led_setup();
  boot_bms();
  bms_setup();
  switch_setup();

  // i2c_write();

  spi_setup();
  // spi_write_test();
  // spi_write();
  // lcd_setup();
  // lcd_display_ui();
  // clear_sys_stat();
  // test_sys_ctrl1();
}

void loop()
{
  led_logging();
  // button.loop();

  // if (button.isPressed()){
  //   led_logging();
  // }
}

