#include "AlertSystem.h"
#include "DetectSystem.h"
#include "Util.h"

DetectSystem dSys;
AlertSystem aSys;
unsigned long previousMillis = 0;

boolean testRedLight = false;

void setup() 
{
}


void loop() {
  if (testRedLight) {
    delay(5000);
    aSys.turnI2Red();
    dSys.lightTurnRed();
    testRedLight = false;
  }

  LinkedList<DetectionResult*>* detectionResults = dSys.detectAggressiveDriving();
  if (detectionResults->size() > 0) {
    aSys.alertTrafficLights(detectionResults);
    Util::debugMsg("Alert Response Time End: ", millis());
  }
  if (millis() - previousMillis >= Util::BLINK_INTERVAL) {
    previousMillis = millis();
    aSys.blinkAlertedLights();
  }
  cleanup(detectionResults);
  aSys.resetAlertedLights();
}

void cleanup(LinkedList<DetectionResult*>* detectionResults) {
  for (int i = 0; i < detectionResults->size(); i++) {
    DetectionResult* result = detectionResults->get(i);
    delete result;
  }
  delete detectionResults;
}
