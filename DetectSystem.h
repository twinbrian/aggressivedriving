#ifndef Detect_System_h
#define Detect_System_h

#include <Queue.h>
#include <LinkedList.h>
#include "RgbLED.h"

enum DrivingBehavior {
    NonAggressive,
    AggressiveStraightLineSpeeding,
    AggressiveRightTurnSpeeding,
    AggressiveLeftTurnSpeeding,
    AggressiveRedLightViolation
  };
  
class DetectionResult {
public:
  DetectionResult() : drivingBehavior(NonAggressive), intersectionId(""), exitStreetId("") {
  }
  DetectionResult(const DetectionResult& rhs) { //copy constructor, copies the same type of object in a member variable to itself. The & is a reference operator
    drivingBehavior = rhs.drivingBehavior;
    intersectionId = rhs.intersectionId;
    exitStreetId = rhs.exitStreetId;
  }
  DrivingBehavior drivingBehavior;
  String intersectionId;
  String exitStreetId; 
};

class DetectSystem { 
public:
  DetectSystem(); //constructor, same name as file name

  LinkedList<DetectionResult*>* detectAggressiveDriving();

  void lightTurnRed() {
    _i2_red_light = true;
    _startingRedTime = millis();
  }
  ~DetectSystem();

private:

  static int STRAIGHT_LINE_MARKUP;
  static int RIGHT_TURN_MARKUP;
  static int LEFT_TURN_MARKUP;
  static int RED_LIGHT_MARKUP;
  
  void init();
  boolean isAggressiveSpeeding(float drivingSpeed, int markupPercent);
  void printLcd(DrivingBehavior detectedBehavior); 
  DetectionResult* processOutflowSensor(unsigned int outflowSensorPort, DataQueue<unsigned long>& inflowQueue,
    DrivingBehavior behavior, String intersectId, String streetId);
    
  
  // All the sensor ports used for detection at intersection 2 and 7.
  unsigned int _i2_n2;
  unsigned int _o2_25;
  unsigned int _o2_12;
  unsigned int _o2_23;
  unsigned int _i7_57;
  unsigned int _o7_78;
 
  // LED to provide red light violation at intersection 2.
  boolean _i2_red_light;
  unsigned long _startingRedTime;
  
  //Queues for inflow sensors at intersections 2 and 7.
  DataQueue<unsigned long> _i2_n2_queue;
  DataQueue<unsigned long> _i2_n2_red_light_queue;
  DataQueue<unsigned long> _i7_57_queue;

  DrivingBehavior detectedBehavior;
};

#endif
