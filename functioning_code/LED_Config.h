#ifndef LED_CONFIG
#define LED_CONFIG

#include <Arduino.h>
#include <Wire.h>
#include <ezButton.h>   // supports button debounce
#include <LCD_CONFIG.h>
#include <SPI_CONFIG.h>
#include <BMS_Config.h>

#define LED_RED 11
#define LED_GREEN 13
#define LED_BLUE 15
#define MOMENTARY_SWITCH 1

ezButton button(MOMENTARY_SWITCH);
volatile int led_state = LOW;
volatile int disp_counter = 0;   

// led_setup(): configures required pins as OUTPUT
void led_setup(){
    Serial.println("led_setup(): Running...");
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
}

// led_boot(): turns RGB LED (blue) when BM IC is powered on/idle state
void led_boot(){
    // display blue light --> ON/IDLE
    if (disp_counter == 0) {
        analogWrite(LED_RED, 41);
        analogWrite(LED_GREEN, 177);
        analogWrite(LED_BLUE, 255);
        Serial.println("- BM IC State: [IDLE]");
        Serial.println("- LED Color: [Blue]");
    }

    else {
        analogWrite(LED_RED, 41);
        analogWrite(LED_GREEN, 177);
        analogWrite(LED_BLUE, 255);
    }
    
    // if (disp_counter != 0)
    //     Serial.println("-----------------------");
}

// TODO: convert to ISR using interrupts (guru meditation error: core 0 panic'ed)
// led_logging(): turns RGB LED (green) when MOMENTARY SWITCH is flipped to HIGH
void led_logging(){

    // bms_checking();
    get_bms_values();
    // BMS.checkStatus();
    // display on screen
    // update BMS_UI hex array
    // led_display();
 
    // momentary switch (HIGH) --> logging state
    if (led_state == HIGH && disp_counter == 1){
        Serial.println("- BM IC State: [LOGGING]");
        Serial.println("- LED Color: [Green]");
        Serial.println("-----------------------");
        analogWrite(LED_RED, 0);
        analogWrite(LED_GREEN, 255);
        analogWrite(LED_BLUE, 128);

        disp_counter += 1;

    }

    else if (led_state == HIGH && disp_counter > 1){
        analogWrite(LED_RED, 0);
        analogWrite(LED_GREEN, 255);
        analogWrite(LED_BLUE, 128);

        // get_bms_values();
        delay(1000);
        spi_write(temp, current, voltage);
        // spi_write_test();
    }

        // TODO: implement function that logs data

    if (disp_counter == 0)
    disp_counter++;


    // momentary switch (LOW) --> idle state
    if (led_state == LOW){
        led_boot();
        // lcd_display_ui();
        counter = 0;
    }

    // TODO: logging has finished, output something


}

// void bms_checking(){
//     get_bms_values();   // update --> set global variables
//     // check to see if charging or discharging


//     // if temp > max || temp < min
//         // shutdown();
    
// }

// led_fault(): turns RGB LED (red) when a circuit fault is detected 
int led_fault(){
    // TODO: implement circuit fault handling functions
    Serial.println("- BM IC State: [CIRCUIT FAULT]");
    Serial.println("- LED Color: [Red]");
    
    // TODO: turn LED Color --> RED


    return 0;
}

void switch_test(){
    led_state = !led_state;

    // momentary switch (HIGH) --> logging state
    if (led_state == HIGH){
        Serial.println("- BM IC State: [LOGGING]");
        Serial.println("- LED Color: [Green]");
        Serial.println("-----------------------");
    }

    else{
        Serial.println("- BM IC State: [IDLE]");
        Serial.println("- LED Color: [Blue]");
    }

}

// ISR
void IRAM_ATTR toggle_logging(){
    led_state = !led_state;
}

// switch_setup(): configures switch as an input_pullup, sets debounce time
void switch_setup(){
    Serial.println("switch_setup: Running...");
    Serial.println("-----------------------");
    pinMode(MOMENTARY_SWITCH, INPUT_PULLUP);
    button.setDebounceTime(50); // set debounce time to 50 milliseconds
    attachInterrupt(digitalPinToInterrupt(MOMENTARY_SWITCH), toggle_logging, RISING);
}

#endif

