
#include <LiquidCrystal.h>
#include <Wire.h>

// LiquidCrystal lcd(RS, R, D4, D5, D6, D7);
LiquidCrystal lcd(21, 33, 38, 39, 43, 44);  // 43 = TX, 44 = RX

void setup()
{
  lcd.begin(16,2);
  lcd.print("Hello, World!");
  lcd.setCursor(0,1);
  lcd.print("It Works!");
}

void loop()
{}
