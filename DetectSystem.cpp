/***
 * Implementation of detect system.
 */

#include <Arduino.h>
#include "DetectSystem.h"
#include "Util.h"

int DetectSystem::STRAIGHT_LINE_MARKUP = 30;
int DetectSystem::RIGHT_TURN_MARKUP = 20;
int DetectSystem::LEFT_TURN_MARKUP = 25;
int DetectSystem::RED_LIGHT_MARKUP = 5; // set red light markup time period to 5 second.

DetectSystem::DetectSystem() {
  init();
}

DetectSystem::~DetectSystem() {
}

DetectionResult* DetectSystem::processOutflowSensor(unsigned int outflowSensorPort,
                                                   DataQueue<unsigned long>& inflowQueue, //The & makes it pass by reference, hold the reference of what the caller gives instead of the value  
                                                   DrivingBehavior detectBehavior,
                                                   String intersectId, 
                                                   String exitStreetId) {
  Util::debugMsg("Detection Response Time Begin: ", millis());
  DetectionResult* ret = new DetectionResult();
  if (detectBehavior == AggressiveRedLightViolation) {
    ret->intersectionId = intersectId;
    ret->exitStreetId = exitStreetId;
    ret->drivingBehavior = detectBehavior;
    Util::debugMsg("Detection Response Time End: ", millis());
    return ret;
  }
  int markup;
  float distance = Util::STRAIGHT_LINE_DISTANCE;
  float velocity = millis() - inflowQueue.dequeue();
  velocity /= 3600000;
  
  if (detectBehavior == AggressiveStraightLineSpeeding) {
    markup = STRAIGHT_LINE_MARKUP;
    distance = Util::STRAIGHT_LINE_DISTANCE;
  } else if (detectBehavior == AggressiveRightTurnSpeeding) {
    markup = RIGHT_TURN_MARKUP;
    distance = Util::RIGHT_TURN_DISTANCE;
  } else if (detectBehavior == AggressiveLeftTurnSpeeding) {
    markup = LEFT_TURN_MARKUP;
    distance = Util::LEFT_TURN_DISTANCE;
  }
  velocity = distance / velocity;
  velocity /= 5280;
  Util::debugMsg("Speed: ", velocity);
  while (digitalRead(_o2_25) == 0) {
    ;
  }
  Util::debugMsg("Markup Value: ", markup);
  if (isAggressiveSpeeding(velocity, markup)) {
    printLcd(detectBehavior);
    ret->intersectionId = intersectId;
    ret->exitStreetId = exitStreetId;
    ret->drivingBehavior = detectBehavior;
  }
  Util::debugMsg("Detection Response Time End: ", millis());
  return ret;
}


LinkedList<DetectionResult*>* DetectSystem::detectAggressiveDriving() {
  LinkedList<DetectionResult*>* ret = new LinkedList<DetectionResult*>();

  if(digitalRead(_i2_n2) == 0) { //use if instead of while to allow system to multi-task (i.e. detect at multiple intersections)
    if (_i2_red_light && millis() - _startingRedTime >= RED_LIGHT_MARKUP) {
      // red light violation detected.
      _i2_n2_red_light_queue.enqueue(millis());
    } else {
      _i2_n2_queue.enqueue(millis());
    }
    Util::debugMsg("Detected a Car at Intersection 2");
    while (digitalRead(_i2_n2) == 0) { //accounts for when vehicles are too slow, makes sure that the vehicle timestamps are not enqueued more than once.
      ;
    }
    delay(100);
  } 

  if(digitalRead(_i7_57) == 0) {
    _i7_57_queue.enqueue(millis());
    Util::debugMsg("Detected a Car at Intersection 7");
    while (digitalRead(_i7_57) == 0) {
      ;
    }
    delay(100);
  }

  if(digitalRead(_o2_25) == 0 && !_i2_n2_queue.isEmpty()) {
    DetectionResult* res;
    if (_i2_red_light && !_i2_n2_red_light_queue.isEmpty()) {
      // red light violation for straightline
      res = processOutflowSensor(_o2_25, _i2_n2_red_light_queue, AggressiveRedLightViolation, "I2", "A25");
    } else {
      res = processOutflowSensor(_o2_25, _i2_n2_queue, AggressiveStraightLineSpeeding, "I2", "A25");
    }
    if (res->drivingBehavior != NonAggressive) {
      ret->add(res);
    }
    delay(100);
  }

  if(digitalRead(_o2_12) == 0 && !_i2_n2_queue.isEmpty()) {
    DetectionResult* res;
    if (_i2_red_light && !_i2_n2_red_light_queue.isEmpty()) {
      // red light violation for left turn
      res = processOutflowSensor(_o2_12, _i2_n2_red_light_queue, AggressiveRedLightViolation, "I2", "S12");
    } else {
      res = processOutflowSensor(_o2_12, _i2_n2_queue, AggressiveRightTurnSpeeding, "I2", "S12");
    }
    if (res->drivingBehavior != NonAggressive) {
      ret->add(res);
    }
    delay(100);
  }

  if(digitalRead(_o2_23) == 0 && !_i2_n2_queue.isEmpty()) {
    DetectionResult* res = processOutflowSensor(_o2_23, _i2_n2_queue, AggressiveLeftTurnSpeeding, "I2", "S23");
    if (res->drivingBehavior != NonAggressive) {
      ret->add(res);
    }
    delay(100);
  }
  
  if(digitalRead(_o7_78) == 0 && !_i7_57_queue.isEmpty()) {
    DetectionResult* res = processOutflowSensor(_o7_78, _i7_57_queue, AggressiveLeftTurnSpeeding, "I7", "S78");
    if (res->drivingBehavior != NonAggressive) {
      ret->add(res);
    }
    delay(100);
  }
  
  return ret;
} 

void DetectSystem::init() {
  _i2_n2 = 50;
  _o2_25 = 51;
  _o2_12 = 53;
  _o2_23 = 52;
  _i7_57 = 48;
  _o7_78 = 49;

  pinMode(_i2_n2, INPUT);
  pinMode(_o2_25, INPUT);
  pinMode(_o2_12, INPUT);
  pinMode(_o2_23, INPUT);
  pinMode(_i7_57, INPUT);
  pinMode(_o7_78, INPUT);

  _i2_red_light = false;
}

boolean DetectSystem::isAggressiveSpeeding(float drivingSpeed, int markupPercent) {
  return (drivingSpeed > (100.0 + (float)markupPercent)/(100.0) * Util::SPEED_LIMIT) ? true : false;
}

void DetectSystem::printLcd(DrivingBehavior detectedBehavior) {
  if (detectedBehavior == AggressiveStraightLineSpeeding) {
    Util::printLcd("Straight Line Speeding");
  } else if (detectedBehavior == AggressiveRightTurnSpeeding) {
    Util::printLcd("Right Turn Speeding"); 
  } else if (detectedBehavior == AggressiveLeftTurnSpeeding) {
    Util::printLcd("Left Turn Speeding");
  } else if (detectedBehavior == AggressiveRedLightViolation) {
    Util::printLcd("Red Light Violation");
  } else if (detectedBehavior == NonAggressive) {
    Util::printLcd("Not Aggressive");
  }
}
