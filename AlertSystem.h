#ifndef Alert_System_h
#define Alert_System_h

#include "AlertDecisionGraph.h"
#include "DetectSystem.h"

class AlertSystem {
public: 

  AlertSystem();
  void alertTrafficLights(LinkedList<DetectionResult*>* detectionResult);
  void blinkAlertedLights();
  void resetAlertedLights();
  void turnI2Red();
  
private:  
  AlertDecisionGraph _graph;
  LinkedList<RgbLED*> _alertedLights;
  RgbLED* _i2_light;
};

#endif
