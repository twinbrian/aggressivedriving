#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include <Arduino.h>
#include "Util.h"

//I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 
boolean initLCD = false;
boolean initSerial = false;

float Util::SPEED_LIMIT = 1.0;
float Util::INTERSECTION_DISTANCE = 1.2;
float Util::STRAIGHT_LINE_DISTANCE = INTERSECTION_DISTANCE;
float Util::LEFT_TURN_DISTANCE = INTERSECTION_DISTANCE * 1.5;
float Util::RIGHT_TURN_DISTANCE = INTERSECTION_DISTANCE * 1;
unsigned long Util::BLINK_INTERVAL = 500;
unsigned long Util::BLINK_DURATION = 7000;

/*** Serial monitor output functions ***/
void Util::serialSetup() {
  Serial.begin(9600);
}

void Util::debugMsg(String msg) {
  if (!initSerial) {
    serialSetup();
    initSerial = true;
  } 
  Serial.println(msg);
}

void Util::debugMsg(String msg, float v) {
  if (!initSerial) {
    serialSetup();
    initSerial = true;
  } 
  Serial.print(msg);
  Serial.println(v);
}

void Util::debugMsg(String msg, int v) {
  if (!initSerial) {
    serialSetup();
    initSerial = true;
  } 
  Serial.print(msg);
  Serial.println(v);
}

void Util::debugMsg(String msg, unsigned long v) {
  if (!initSerial) {
    serialSetup();
    initSerial = true;
  } 
  Serial.print(msg);
  Serial.println(v);
}

/*** LCD output functions ***/
void Util::lcdSetup() {
  lcd.begin(16,4);//Defining 16 columns and 4 rows of lcd display
  lcd.backlight();//To Power ON the back light
  lcd.setCursor(0,0);
  lcd.print("System is ready...");
}
void Util::printLcd(String msg) {
  if (!initLCD) {
    lcdSetup();
    initLCD = true;
  }
  lcd.clear();
  lcd.print(msg);
}
void Util::printLcd(String msg, float v) {
  if (!initLCD) {
    lcdSetup();
    initLCD = true;
  }
  char fvBuffer[16];
  dtostrf(v, 2, 2, fvBuffer);
  msg.concat(fvBuffer);
  printLcd(msg);  


}
