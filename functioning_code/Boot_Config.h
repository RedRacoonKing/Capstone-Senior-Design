#ifndef BOOT_CONFIG
#define BOOT_CONFIG

#include <Arduino.h>
#include <Wire.h>
#include <I2C_Config.h>
#include <LED_Config.h>

#define BOOT 3

bool boot_flag = false;

void boot_bms(){
    Serial.println("boot_bms: Running...");
    // Boot up BMS w/ Software
    while (!boot_flag) {
        delay(1000);
        pinMode(BOOT, OUTPUT);
        delay(1000);

        digitalWrite(BOOT, HIGH);  // switch diode on
        delay(3); // 3 ms = 3000 us 
        digitalWrite(BOOT, LOW);

        Wire.beginTransmission(SLAVE_ADDRESS);
        byte error = Wire.endTransmission();

        if (error == 0){
            boot_flag = true;
            Serial.println("- BM IC: [ON]");
            led_boot();
        }

        else
            Serial.println("- BM IC: [OFF]\n");

        delay(2000);
    }
}

#endif 
