#ifndef Savior_Util_h
#define Savior_Util_h

#include <Stream.h>


class Util {
public:
  static float SPEED_LIMIT; //constant
  static float INTERSECTION_DISTANCE;
  static float STRAIGHT_LINE_DISTANCE;
  static float LEFT_TURN_DISTANCE;
  static float RIGHT_TURN_DISTANCE;
  static unsigned long BLINK_INTERVAL;
  static unsigned long BLINK_DURATION;

  /*** Serial monitor output functions ***/
  static void debugMsg(String msg);
  static void debugMsg(String msg, float v);
  static void debugMsg(String msg, int v);
  static void debugMsg(String msg, unsigned long v);

  /*** LCD output functions ***/
  static void printLcd(String msg);
  static void printLcd(String msg, float v);

private:
  static void lcdSetup();
  static void serialSetup();

};

#endif
