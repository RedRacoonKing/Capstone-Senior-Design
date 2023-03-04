#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <Arduino.h>
#include <Wire.h>
#include <I2C_Config.h>
#include "bq769x0.h"
#include "registers.h"


#define SCK   36
#define MISO  42
#define MOSI  40
#define CS    38

bq769x0 BMS(bq76930, 0x18);

SPIClass spi = SPIClass(HSPI);

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void createDir(fs::FS &fs, const char * path){
  Serial.printf("Creating Dir: %s\n", path);
  if(fs.mkdir(path)){
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void removeDir(fs::FS &fs, const char * path){
  Serial.printf("Removing Dir: %s\n", path);
  if(fs.rmdir(path)){
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

void readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)){
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

void deleteFile(fs::FS &fs, const char * path){
  Serial.printf("Deleting file: %s\n", path);
  if(fs.remove(path)){
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void testFileIO(fs::FS &fs, const char * path){
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if(file){
    len = file.size();
    size_t flen = len;
    start = millis();
    while(len){
      size_t toRead = len;
      if(toRead > 512){
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  } else {
    Serial.println("Failed to open file for reading");
  }


  file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for(i=0; i<2048; i++){
    file.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}

void setup(){
  delay(1000);
  int val;
  long bat_v;

  // setup i2c bus
  i2c_setup();

  // setup bms
  BMS.begin(17, 3);

  // read value from SYS_STAT (0x00) - should be 34
  Serial.println("Reading SYS_STAT: ");
  Serial.println(BMS.readRegister(0x00));

  // write 0x00 to SYS_STAT (0x00) - should clear register
  BMS.writeRegister(0x00, 0xFF);

  // read new value from SYS_STAT (0x00) - should be 0x00
  Serial.println("Reading SYS_STAT: ");
  Serial.println(BMS.readRegister(0x00));

  // read CC_CFG (should be set to 0x19 during BMS.begin() call)
  Serial.println("Reading CC_CFG: ");
  Serial.println(BMS.readRegister(0x0B));

  // reading from SYS_CTRL1 (0x04) (24 == ADC_EN + TEMP_SEL)
  Serial.println("Reading SYS_CTRL1: ");
  Serial.println(BMS.readRegister(0x04));

  // reading from BAT_HI (0x2A) and BAT_LO (0x2B) 
  // Serial.println("Reading BAT_HIGH: ");
  // Serial.println(BMS.readRegister(BAT_HI_BYTE));
  // Serial.println("Reading BAT_LOW: ");
  // Serial.println(BMS.readRegister(BAT_LO_BYTE));


  // BMS.update();
  BMS.updateVoltages();

  bat_v = BMS.getBatteryVoltage();
  Serial.print("Battery voltage: ");
  Serial.println(bat_v);

  // delay(1000);
  // Serial.begin(115200);
  // spi.begin(SCK, MISO, MOSI, CS);

  // if (!SD.begin(CS,spi,80000000)) {
  //   Serial.println("Card Mount Failed");
  //   return;
  // }
  // uint8_t cardType = SD.cardType();

  // if(cardType == CARD_NONE){
  //   Serial.println("No SD card attached");
  //   return;
  // }

  // Serial.println("Writing dummy CSV file...\n");

  // writeFile(SD, "/dummy.csv", "Temp,I_o,V_bat,Bat%\n");
  // appendFile(SD, "/dummy.csv", "0,1,2,3\n");
  // readFile(SD, "/dummy.csv");
  // Serial.println("Done....");

  


  // listDir(SD, "/", 0);
  // createDir(SD, "/mydir");
  // listDir(SD, "/", 0);
  // removeDir(SD, "/mydir");
  // listDir(SD, "/", 2);
  // writeFile(SD, "/hello.txt", "Hello ");
  // appendFile(SD, "/hello.txt", "World!\n");
  // readFile(SD, "/hello.txt");
  // deleteFile(SD, "/foo.txt");
  // renameFile(SD, "/hello.txt", "/foo.txt");
  // readFile(SD, "/foo.txt");
  // testFileIO(SD, "/test.txt");
  // Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  // Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

void loop(){

}
