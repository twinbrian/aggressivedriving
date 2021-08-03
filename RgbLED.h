#ifndef Rgb_LED_h
#define Rgb_LED_h
#include <Arduino.h>

class RgbLED {
  
enum LEDColor {
  None,
  Red, 
  Green,
  Blue
};

public: 
  //allow caller to have multiple led instances with different names and pin numbers
  RgbLED(String ledName, unsigned int redPin, unsigned int greenPin, unsigned int bluePin); 
  
  void turnOff();
  void turnOnRed();
  void turnOnGreen();
  void turnOnBlue();
  void blinkLED();
  void turnOffBlink();
  void setStartBlinkingTime(unsigned long startTime);
  unsigned long getStartBlinkingTime();

private: 
  String _name;
  unsigned int _red_pin;
  unsigned int _green_pin;
  unsigned int _blue_pin;
  LEDColor _color;
  unsigned long _startBlinkingTime;
};
#endif
