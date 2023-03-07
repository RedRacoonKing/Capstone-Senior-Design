#ifndef SPI_CONFIG
#define SPI_CONFIG 

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <Arduino.h>
#include <SD_MMC.h>

#define SCK 36    // (CLK)
#define MISO 42   // (DO)
#define MOSI 40   // (DI)
#define SPI_CS 38 // (CS)

SPIClass spi = SPIClass(HSPI);
int counter = 0;
int file_counter = 0;
char name_buffer[50];


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

void spi_setup(){
    delay(1000);
    Serial.println("spi_setup(): Running...");
    spi.begin(SCK, MISO, MOSI, SPI_CS);

    if (!SD.begin(SPI_CS,spi,80000000)) {
        Serial.println("Error: Card Mount Failed");
        return;
    }
  
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("Error: No SD Card Attached");
        return;
    } 
}

void spi_write(float temp, float current, float voltage){

  char buffer[50];
  sprintf(buffer, "%f,%f,%f,\n", temp, current, voltage);

  if (counter == 0){
    Serial.println("spi_write(): Running");
    file_counter += 1;
    sprintf(name_buffer, "/log%d.csv", file_counter);
    writeFile(SD, name_buffer, "Temp,I_o,V_bat,Bat%\n");
    counter += 1;
  }
  
  // TODO: add timing 
  appendFile(SD, name_buffer, buffer);
  Serial.println(buffer);
  readFile(SD, name_buffer);
  Serial.println();
    
}

void spi_write_test(){
  Serial.println("spi_write(): Running");
  writeFile(SD, "/dummy2.csv", "Temp,I_o,V_bat,Bat%\n");
  appendFile(SD, "/dummy2.csv", "0,0,0,0\n");
  appendFile(SD, "/dummy2.csv", "1,1,1,1\n");
  readFile(SD, "/dummy2.csv");
}

#endif 
