#ifndef LED_CONFIG
#define LED_CONFIG

#include <Arduino.h>
#include <Wire.h>
#include <ezButton.h>   // supports button debounce
#include <LCD_CONFIG.h>

#define LED_RED 11
#define LED_GREEN 13
#define LED_BLUE 15
#define MOMENTARY_SWITCH 1

ezButton button(MOMENTARY_SWITCH);
int led_state = LOW;
int disp_counter = 0;

void led_setup(){
    Serial.println("led_setup(): Running...");
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
}

void led_boot(){
    // display blue light --> ON/IDLE
    analogWrite(LED_RED, 41);
    analogWrite(LED_GREEN, 177);
    analogWrite(LED_BLUE, 255);
    Serial.println("- BM IC State: [IDLE]");
    Serial.println("- LED Color: [Blue]");
    
    if (disp_counter != 0)
        Serial.println("-----------------------");
}

// TODO: convert to ISR using interrupts (guru meditation error: core 0 panic'ed)
// executes when MOMENTARY SWITCH is flipped to HIGH
void led_logging(){
    // display green light --> logging = ON

    if (disp_counter == 0)
        disp_counter++;

    led_state = !led_state;

    if (led_state == HIGH){
        Serial.println("- BM IC State: [LOGGING]");
        Serial.println("- LED Color: [Green]");
        Serial.println("-----------------------");
        analogWrite(LED_RED, 0);
        analogWrite(LED_GREEN, 255);
        analogWrite(LED_BLUE, 128);

        // TODO: implement function that logs data

    }

    if (led_state == LOW){
        led_boot();
    }
}

int led_fault(){
    // TODO:
    return 0;
}

void switch_setup(){
    Serial.println("switch_setup: Running...");
    Serial.println("-----------------------");
    pinMode(MOMENTARY_SWITCH, INPUT_PULLUP);
    button.setDebounceTime(50); // set debounce time to 50 milliseconds
    // attachInterrupt(digitalPinToInterrupt(MOMENTARY_SWITCH), led_logging, RISING);
}


#endif

