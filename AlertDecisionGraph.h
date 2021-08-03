#ifndef Alert_Decision_Graph_h
#define Alert_Decision_Graph_h

#include <Arduino.h>
#include <LinkedList.h>
#include <SimpleMap.h>
#include "RgbLED.h"

class Street;

class Intersection {
public:
  Intersection(float x, float y, String id, float speedLimit, RgbLED *light);
  ~Intersection();
  
  String getId() {
    return _id;
  }
  
  LinkedList<Street*>* getStreets() {
    return _streets;
  }
  
  void addStreet(Street *street);

  boolean hasTrafficLight();

  RgbLED* getLight() {
    return _light;
  }
  
private:
  float _x;
  float _y;
  String _id; //identifier
  float _speedLimit;
  RgbLED *_light; 

  LinkedList<Street*> *_streets; //each list stores the addresses of each street
};
  
class Street {
public: 
  ~Street();
  Street(String name, String id, boolean isOneWay);
  String getId() {
    return _id;
  }
  void setIntersectionPair(Intersection *i1, Intersection *i2);
  Intersection* getOtherIntersection(Intersection *i); //find the adjacent intersection connected by a street
  
private:
  String _name;
  String _id;
  boolean _isOneWay;

  //Pointers
  Intersection *_i1;
  Intersection *_i2;
};

class AlertDecisionGraph {
public:
  AlertDecisionGraph();
  ~AlertDecisionGraph();
  void build();
  RgbLED* getI2Light();

  /* Given the intersection and exit route, find the intersections to alert. The return is a list of intersection pointers. */
  LinkedList<Intersection*>* dfsTraverse(String intersectionId, String streetId); 

private:
  SimpleMap<String, Intersection*> _intersectionMap;
  RgbLED* _i2_light;
};

#endif 
