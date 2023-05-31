#include "Arduino.h"
#include "Kodelaas.h"
#include <Servo.h>

Kodelaas::Kodelaas(int servoPin) { //tar inn en digital pin.
    _servoPin = servoPin;
}

//knytter servoen til en pin, dette må gjøres i setup()
void Kodelaas::attach() {
  _Servo1.attach(_servoPin);
}

//setter servoen i en posisjon hvor bøtten låses.
void Kodelaas::laas() {
    _Servo1.write(70);
}

//setter servoen i en posisjon hvor bøtten låses opp.
void Kodelaas::laasOpp() {
    _Servo1.write(120);
}