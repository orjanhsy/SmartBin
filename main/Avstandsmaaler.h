#ifndef Avstandsmaaler_h
#define Avstandsmaaler_h
#include "Arduino.h"

class Avstandsmaaler {

  private:
    int _echoPin, _pingPin, _duration, _cm;

    long tidTilAvstand(long microseconds);
  
  public:
    Avstandsmaaler(int echoPin, int pingPin);

    long distanseTilObjekt();
};
#endif
