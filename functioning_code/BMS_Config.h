#ifndef BMS_CONFIG
#define BMS_CONFIG

#include <Arduino.h>
#include <Wire.h>
#include <bq769x0.h>
#include <registers.h>

#define I2C_SDA 7
#define I2C_SCL 9
#define ALERT_PIN 17

const int SLAVE_ADDRESS = 0x18;
const int SHIFTED_SLAVE_ADDR = 0x30;
const int REG_ADDRESS = 0x00;
// need another global for LCD Screen

bq769x0 BMS;    // BMS Object 

float temp, current, voltage;   // used for BMS

// i2c_setup: sets up communication between BM IC (PCB) & ESP-32
void i2c_setup(){
    Wire.begin(I2C_SDA, I2C_SCL);
    Serial.begin(115200);
    delay(1000);
    Serial.println("i2c_setup(): Running...");
}

void bms_set_protection(){
    Serial.println("bms_set_protection(): Running...");
    BMS.setTemperatureLimits(0, 0, 0, 0);
    BMS.setShortCircuitProtection(0);
    BMS.setOvercurrentChargeProtection(0);
    BMS.setOvercurrentDischargeProtection(0);
    BMS.setCellUndervoltageProtection(0);
    BMS.setCellOvervoltageProtection(0);
}

// bms_setup: configures bms functionalities needed
void bms_setup(){
    Serial.println("bms_setup(): Running...");
    BMS.begin(ALERT_PIN, 3);   // (alert, boot)
    
    // BMS.enableCharging();
    // BMS.enableDischarging();
    // BMS.updateBalancingSwitches();
    
    // bms_set_protection();
    
    Serial.println("-----------------------");

}


// i2c_scanner: locates the slave address of slave device (i.e. BM IC)
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

// clear SYS_STAT register
void clear_sys_stat(){
    Serial.println("clear_sys_stat(): Running...");
    Serial.print("- [OLD] SYS_STAT: ");
    Serial.println(BMS.readRegister(SYS_STAT));

    BMS.writeRegister(SYS_STAT, 0xFF);  // clearing SYS_STAT by writing 1's to the bits
    delay(1000);

    Serial.print("- [NEW] SYS_STAT: ");
    Serial.println(BMS.readRegister(SYS_STAT));
     Serial.println("-----------------------");
}

// read SYS_CTRL1 (value = 24), write value = 0, reset back to 24
void test_sys_ctrl1(){
    Serial.println("test_sys_ctrl1(): Running...");

    // read (0x18) 24 from SYS_CTRL1
    Serial.print("- [OLD] SYS_CTRL1: ");
    Serial.println(BMS.readRegister(SYS_CTRL1));

    // write (0x00) 0 to SYS_CTRL1
    BMS.writeRegister(SYS_CTRL1, 0x00);
    delay(1000);

    // read (0x00) 0 from SYS_CTRL1
    Serial.print("- [NEW] SYS_CTRL1: ");
    Serial.println(BMS.readRegister(SYS_CTRL1));

    // write (0x18) 24 to SYS_CTRL1
    BMS.writeRegister(SYS_CTRL1, 24);

    // read (0x18) 24 from SYS_CTRl1
    Serial.print("- [NEWEST] SYS_CTRL1: ");
    Serial.println(BMS.readRegister(SYS_CTRL1));
    Serial.println("-----------------------");
}

void get_bms_values(){

    // BMS.setThermistorBetaValue(1);
    BMS.updateTemperatures();
    BMS.updateCurrent();
    BMS.updateVoltages();
    
    temp = BMS.getTemperatureDegC();
    current = BMS.getBatteryCurrent();
    voltage = BMS.getBatteryVoltage();
}

#endif

