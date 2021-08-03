#include "AlertSystem.h"
#include "Util.h"

AlertSystem::AlertSystem() {
  _graph.build();
  _i2_light = _graph.getI2Light();
}


void AlertSystem::alertTrafficLights(LinkedList<DetectionResult*>* detectionResult) {
  for (int i = 0; i < detectionResult->size(); i++) {
    DetectionResult* result = detectionResult->get(i);
    LinkedList<Intersection*> *retList = _graph.dfsTraverse(result->intersectionId, result->exitStreetId);
    for (int j = 0; j < retList->size(); j++) {
      Intersection *intersection = retList->get(j);
      if (intersection->hasTrafficLight()) {
        RgbLED *light = intersection->getLight();
        light->setStartBlinkingTime(millis());
        boolean existing = false;
        for (int k = 0; k < _alertedLights.size(); k++) {
          if (_alertedLights.get(k) == light) {
            existing = true;
            break;
          }
        }
        if (!existing) {
          _alertedLights.add(light);
        }
      } 
    }
    delete retList;  
  } 
}

void AlertSystem::blinkAlertedLights() {
  for (int i = 0; i < _alertedLights.size(); i++) {
    RgbLED *light = _alertedLights.get(i);
    light->blinkLED();
  }
}

void AlertSystem::resetAlertedLights() {
  unsigned long curTime = millis();
  int i = 0;
  while (i < _alertedLights.size()) {
    RgbLED *light = _alertedLights.get(i);
    if (curTime - light->getStartBlinkingTime() >= Util::BLINK_DURATION) {
      light->turnOffBlink();
      _alertedLights.remove(i);
    } else {
      i++;
    }
  }
}

void AlertSystem::turnI2Red() {
  Util::debugMsg("Turn i2 light to red...");
  _i2_light->turnOnRed();
}
