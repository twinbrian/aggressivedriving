#include "RgbLED.h"
#include "Util.h"

RgbLED::RgbLED(String ledName, unsigned int redPin, unsigned int greenPin, unsigned int bluePin) 
  : _name(ledName), _red_pin(redPin), _green_pin(greenPin), _blue_pin(bluePin), _color(None) {
  /* Set all LED light pins as output pins. */
  pinMode(_red_pin, OUTPUT);
  pinMode(_green_pin, OUTPUT);
  pinMode(_blue_pin, OUTPUT);
  /* Lights are off by default */
  turnOff();
}

/*** LED control functions ***/
void RgbLED::turnOnRed() {
  digitalWrite(_red_pin, LOW);
  digitalWrite(_green_pin, HIGH);
  digitalWrite(_blue_pin, HIGH);
  _color = Red;
}
void RgbLED::turnOnGreen() {
    digitalWrite(_red_pin, HIGH);
    digitalWrite(_green_pin, LOW);
    digitalWrite(_blue_pin, HIGH);
    _color = Green;
}
void RgbLED::turnOnBlue() {
  digitalWrite(_red_pin, HIGH);
  digitalWrite(_green_pin, HIGH);
  digitalWrite(_blue_pin, LOW);
  _color = Blue;
}
void RgbLED::turnOff() {
  digitalWrite(_red_pin, HIGH);
  digitalWrite(_green_pin, HIGH);
  digitalWrite(_blue_pin, HIGH);
  _color = None;
}

void RgbLED::blinkLED() {
  switch(_color) {
    case Red:
    if (digitalRead(_red_pin) == LOW) {
      digitalWrite(_red_pin, HIGH);
    } else {
      digitalWrite(_red_pin, LOW);
    }
    break;
    case Green:
    if (digitalRead(_green_pin) == LOW) {
      digitalWrite(_green_pin, HIGH);
    } else {
      digitalWrite(_green_pin, LOW);
    }
    break;
    case Blue:
    if (digitalRead(_blue_pin) == LOW) {
      digitalWrite(_blue_pin, HIGH);
    } else {
      digitalWrite(_blue_pin, LOW);
    }
    break;
  }
}

void RgbLED::turnOffBlink() {
  switch(_color) {
    case Red:
    turnOnRed();
    break;
    case Green:
    turnOnGreen();
    break;
    case Blue:
    turnOnBlue();
    break;
  }  
}

void RgbLED::setStartBlinkingTime(unsigned long startTime) {
  _startBlinkingTime = startTime;
}

unsigned long RgbLED::getStartBlinkingTime() {
  return _startBlinkingTime;
}
