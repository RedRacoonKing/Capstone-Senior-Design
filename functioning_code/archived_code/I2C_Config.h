#ifndef I2C_CONFIG
#define I2C_CONFIG

#include <Arduino.h>
#include <Wire.h>

#define I2C_SDA 7
#define I2C_SCL 9

// #define I2C_SDA 37
// #define I2C_SCL 35
// need two more preprocessor constants for LCD Screen

const int SLAVE_ADDRESS = 0x18;
const int SHIFTED_SLAVE_ADDR = 0x30;
const int REG_ADDRESS = 0x00;
// need another global for LCD Screen

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


// CRC-8 calculation function
uint8_t calculateCRC(uint8_t *data, uint16_t len)
{
  uint8_t crc = 0x00;
  byte generator = 0b00000111;

  for (int i = 0; i < len; i++)
  {
    crc ^= data[i];

    for (int j = 0; j < 8; j++)
    {
      if (crc & 0x80)
      {
        crc = (crc << 1) ^ generator;
      }
      else
      {
        crc = crc << 1;
      }
    }
  }

  return crc;
}

void i2c_write(){
    uint8_t data[] = {0xFF};
    uint16_t dataLen = sizeof(data);
    uint8_t slave[] = {0x18};
    uint16_t slaveLen = sizeof(slave);
    uint8_t reg[] = {0x00};
    uint16_t regLen = sizeof(reg);

    // Calculate CRC-8 checksum
    uint8_t crc = calculateCRC(data, dataLen);
    uint8_t crcSA = calculateCRC(slave, slaveLen);
    uint8_t crcREG = calculateCRC(reg, regLen);

    Serial.println("Reading init value...");
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(REG_ADDRESS);
    Wire.endTransmission();

    Wire.requestFrom(SLAVE_ADDRESS, 1);
    byte value = Wire.read();
    Serial.println(value);

    // Prepare data to be sent over I2C

    // data
    uint8_t sendData[dataLen + 1];
    memcpy(sendData, data, dataLen);
    sendData[dataLen] = crc;
    // slave address
    uint8_t sendSlave[slaveLen + 1];
    memcpy(sendSlave, slave, slaveLen);
    sendSlave[slaveLen] = crcSA;
    // register address
    uint8_t sendReg[regLen + 1];
    memcpy(sendReg, reg, regLen);
    sendReg[regLen] = crcREG;

    // one array for write transaction
    uint8_t write_buffer[4] = {0x30, 0x00, 0xFF, 0xF3};

    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(write_buffer, 4);
    Wire.endTransmission();

    // Wait for a response
    delay(1000);

    // Read response from registers
    uint8_t response[1];
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(REG_ADDRESS);
    Wire.endTransmission();
    Wire.requestFrom(SLAVE_ADDRESS, sizeof(response));
    for (int i = 0; i < sizeof(response); i++) {
        response[i] = Wire.read();
    }

    // Print response
    Serial.print("Response: ");
    for (int i = 0; i < sizeof(response); i++) {
        Serial.print(response[i]);
        Serial.print(" ");
    }
    Serial.println();

    Serial.println("Reading new value... ");
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(REG_ADDRESS);
    Wire.endTransmission();

    Wire.requestFrom(SLAVE_ADDRESS, 1);
    byte final_value = Wire.read();
    Serial.println(final_value);

    delay(5000);
}
    
#endif

