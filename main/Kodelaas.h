#ifndef Kodelaas_h
#define Kodelaas_h
#include "Arduino.h"
#include <Servo.h>


class Kodelaas {

    private:
        int _servoPin;
        String _passord[3];
        Servo _Servo1;

    public:
        Kodelaas(int servoPin);
        
        void laas();
        void laasOpp();
        void attach();
};
#endif