#ifndef I2C_CONFIG
#define I2C_CONFIG

#include <Arduino.h>
#include <Wire.h>

#define I2C_SDA 7
#define I2C_SCL 9

const int SLAVE_ADDRESS = 0x18;

void i2c_setup(){
    Wire.begin(I2C_SDA, I2C_SCL);
    Serial.begin(115200);
    delay(1000);
    Serial.println("i2c_setup(): Running...");
}

void i2c_scanner(){

    bool address_flag = false;

    while (!address_flag){
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

            address_flag = true;
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

}

    
#endif

