#include <StackArray.h>

#include "AlertDecisionGraph.h"
#include "Util.h"

//The stuff behind the single colon is the constructor initializer. It initializes the member variables while the compiler allocates memory for them.
Street::Street(String name, String id, boolean isOneWay) : _name(name), _id(id), _isOneWay(isOneWay), _i1(0), _i2(0) {
}

Street::~Street() {
}

Intersection::Intersection(float x, float y, String id, float speedLimit, RgbLED *light) 
  : _x(x), _y(y), _id(id), _speedLimit(speedLimit), _light(light), _streets(new LinkedList<Street*>()) {
}

Intersection::~Intersection() {
  delete _light; //It's not a problem that some intersections' _light values are 0, because delete 0 is an automatic save function.
}

/* This is the only constructor provided, with the input parameter of a function pointer. Introduce an anonymous function to make it easier. */
AlertDecisionGraph::AlertDecisionGraph() : _intersectionMap(SimpleMap<String, Intersection*>([](String& a, String& b) -> int { 
    if (a == b) return 0;
    if (a > b) return 1;
    if (a < b) return -1;
  })) {
    
  }
AlertDecisionGraph::~AlertDecisionGraph() {
  SimpleMap<String, Street*> streetMap([](String& a, String& b) -> int { 
    if (a == b) return 0;
    if (a > b) return 1;
    if (a < b) return -1;
  });
  for (int i = 0; i < _intersectionMap.size(); i++) {
    Intersection *intersection = _intersectionMap.getData(i);
    LinkedList<Street*> *streetList = intersection->getStreets();
    for (int j = 0; j < streetList->size(); j++) {
      Street *street = streetList->get(j);
      if(!streetMap.has(street->getId())) {
        streetMap.put(street->getId(), street);
      }
    }
    delete intersection;
   }
   for (int i = 0; i < streetMap.size(); i++) {
    Street *street = streetMap.getData(i);
    delete street;
  }
}
 
void Intersection::addStreet(Street *street) {
  _streets->add(street);
}

void Street::setIntersectionPair(Intersection *i1, Intersection *i2) {
  _i1 = i1;
  _i2 = i2;
}

Intersection* Street::getOtherIntersection(Intersection *i) {
  return (i == _i1) ? _i2 : _i1;
}

boolean Intersection::hasTrafficLight() {
  return _light != 0; //as long as the _light does not equal to zero, return true and intersection has traffic light
}

void AlertDecisionGraph::build() {
  /*** Vertical Streets ***/
  Street *an1 = new Street("An1", "An1", false);
  Street *an2 = new Street("An2", "An2", false); //allocate object with datatype Street in memory and give address an2
  Street *an3 = new Street("An3", "An3", false);
  Street *a14 = new Street("A14", "A14", false);
  Street *a25 = new Street("A25", "A25", false);
  Street *a38 = new Street("A38", "A38", false);
  Street *a46 = new Street("A46", "A46", false);
  Street *a57 = new Street("A57", "A57", false);
  Street *a6s = new Street("A6s", "A6s", false);
  Street *a7s = new Street("A7s", "A7s", false);
  Street *a8s = new Street("A8s", "A8s", false);
  
  /*** Horizontal Streets ***/
  Street *s12 = new Street("S12", "S12", false);
  Street *s23 = new Street("S23", "S23", false);
  Street *s45 = new Street("S45", "S45", false);
  Street *s67 = new Street("S67", "S67", false);
  Street *s78 = new Street("S78", "S78", false);

  /*** Rgb LEDs for needed intersections. ***/
  RgbLED *i1_light = new RgbLED("i1_light", 39, 40, 41);
  RgbLED *i2_light = new RgbLED("i2_light", 27, 28, 29);
  _i2_light = i2_light;
  RgbLED *i3_light = new RgbLED("i3_light", 30, 31, 32);
  RgbLED *i4_light = new RgbLED("i4_light", 36, 37, 38);
  RgbLED *i7_light = new RgbLED("i7_light", 33, 34, 35);
  RgbLED *i8_light = new RgbLED("i8_light", 24, 25, 26);
  i2_light->turnOnGreen();
  i7_light->turnOnGreen();
  i1_light->turnOnRed();
  i3_light->turnOnRed();
  i4_light->turnOnRed();
  i8_light->turnOnRed();
  
  /*** Intersection Information ***/
  Intersection *i1 = new Intersection(0.0, 1.0, "I1", Util::SPEED_LIMIT, i1_light);
  Intersection *i2 = new Intersection(1.0, 1.0, "I2", Util::SPEED_LIMIT, i2_light);
  Intersection *i3 = new Intersection(2.0, 1.0, "I3", Util::SPEED_LIMIT, i3_light);
  Intersection *i4 = new Intersection(0.0, 2.0, "I4", Util::SPEED_LIMIT, i4_light);
  Intersection *i5 = new Intersection(1.0, 2.0, "I5", Util::SPEED_LIMIT, 0);
  Intersection *i6 = new Intersection(0.0, 3.0, "I6", Util::SPEED_LIMIT, 0);
  Intersection *i7 = new Intersection(1.0, 3.0, "I7", Util::SPEED_LIMIT, i7_light);
  Intersection *i8 = new Intersection(2.0, 3.0, "I8", Util::SPEED_LIMIT, i8_light);

  
  /*** Add following streets for each intersection. The arrow is for dereferencing because i1 is pointer value.***/
  /* Intersection 1 */
  i1->addStreet(s12); 
  i1->addStreet(a14);
  i1->addStreet(an1);
  
  /* Intersection 2 */
  i2->addStreet(an2); 
  i2->addStreet(s12);
  i2->addStreet(s23);
  i2->addStreet(a25);
  
  /* Intersection 3 */
  i3->addStreet(s23);
  i3->addStreet(an3);
  i3->addStreet(a38);
  
  /* Intersection 4 */
  i4->addStreet(s45);
  i4->addStreet(a14);
  i4->addStreet(a46);
  
  /* Intersection 5 */
  i5->addStreet(a25);
  i5->addStreet(a57);
  i5->addStreet(s45);
  
  /* Intersection 6 */
  i6->addStreet(s67);
  i6->addStreet(a46);
  i6->addStreet(a6s);
  
  /* Intersection 7 */
  i7->addStreet(a57);
  i7->addStreet(s78);
  i7->addStreet(s67);
  i7->addStreet(a7s);
  
  /* Intersection 8 */
  i8->addStreet(s78);
  i8->addStreet(a38);
  i8->addStreet(a8s);

  an1->setIntersectionPair(0, i1);
  an2->setIntersectionPair(0, i2);
  an3->setIntersectionPair(0, i3);
  a14->setIntersectionPair(i1, i4);
  a25->setIntersectionPair(i2, i5);
  a38->setIntersectionPair(i3, i8);
  a46->setIntersectionPair(i4, i6);
  a57->setIntersectionPair(i5, i7);
  a6s->setIntersectionPair(i6, 0);
  a7s->setIntersectionPair(i7, 0);
  a8s->setIntersectionPair(i8, 0);
  
  s12->setIntersectionPair(i1, i2);
  s23->setIntersectionPair(i2, i3);
  s45->setIntersectionPair(i4, i5);
  s67->setIntersectionPair(i6, i7);
  s78->setIntersectionPair(i7, i8);

  _intersectionMap.put("I1", i1);
  _intersectionMap.put("I2", i2);
  _intersectionMap.put("I3", i3);
  _intersectionMap.put("I4", i4);
  _intersectionMap.put("I5", i5);
  _intersectionMap.put("I6", i6);
  _intersectionMap.put("I7", i7);
  _intersectionMap.put("I8", i8);
}
/* Input parameters are the IDs of detection intersection and exit street. */
LinkedList<Intersection*>* AlertDecisionGraph::dfsTraverse(String intersectionId, String streetId) { //pass by references
  LinkedList<Intersection*> *retList = new LinkedList<Intersection*>(); //return list of intersections to light
  /* Given the intersection id, look at the map and find starting intersection. Key: intersection id. Value: intersection pointer. */
  Intersection *startIntersect = _intersectionMap.get(intersectionId);
  /* Find all streets connected to start intersection. Not a static function so an object needs to be defined. */
  LinkedList<Street*> *streetList = startIntersect->getStreets();
  Street *startStreet = 0; //initialize the start street 
  for (int i = 0; i < streetList->size(); i++) {
    /* find the (i-1)th street in the list due to how indexes work (the for loop keeps iterating this so that all the streets in the list are retrieved once */
    startStreet = streetList->get(i); 
    if (streetId == startStreet->getId()) { //if the id's are the same
      break; //exit the for loop
    }
  }
  
  /* Inline function, return type is int. This is the constructor, the input parameter is an anonymous function w/o a name. */
  SimpleMap<String, Intersection*> visitedIntersectionMap([](String& a, String& b) -> int { 
    if (a == b) return 0;
    if (a > b) return 1;
    if (a < b) return -1;
  });
  /* Introduce a stack with elements that are intersection pointers for DFS */
  StackArray<Intersection*> stack; 
  /* The starting intersection is placed into the visited intersection map. */
  visitedIntersectionMap.put(startIntersect->getId(), startIntersect); 
  /* Retrieves the next intersection by finding the intersection the starting intersection is connected to through the starting street. */
  Intersection *nextIntersect = startStreet->getOtherIntersection(startIntersect);
  if (nextIntersect->hasTrafficLight()) {
    retList->add(nextIntersect);
  } else {
    stack.push(nextIntersect);
    visitedIntersectionMap.put(nextIntersect->getId(), nextIntersect);
    while (!stack.isEmpty()) {
      Intersection *curIntersect = stack.peek();
      streetList = curIntersect->getStreets();
      Street *adjacentStreet = 0;
      for (int i = 0; i < streetList->size(); i++) {
        adjacentStreet = streetList->get(i);
        Intersection *adjacentIntersect = adjacentStreet->getOtherIntersection(curIntersect);
        if (visitedIntersectionMap.has(adjacentIntersect->getId())) {
          continue; //skip rest of lines
        }
        if (adjacentIntersect->hasTrafficLight()) {
          retList->add(adjacentIntersect);
        } else { 
          stack.push(adjacentIntersect);
          visitedIntersectionMap.put(curIntersect->getId(), curIntersect);
          break;
        }
      }
      if (stack.peek() == curIntersect) { //nothing new is added to the stack because entire for loop is iterated
        stack.pop();
      }
    }
  }
  return retList;
}

RgbLED* AlertDecisionGraph::getI2Light() {
  return _i2_light;
}
