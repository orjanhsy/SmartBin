#ifndef CO2maaler_h
#define CO2maaler_h
#include "Arduino.h"

class CO2maaler {

  private:
    int _anPin, _co2Average;
  
  public:
    CO2maaler(int anPin);

    int maalOver2Sek();
};
#endif