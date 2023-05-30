#include "Arduino.h"
#include "Person.h"

Person::Person(String passord, int poeng1, int poeng2, int poeng3) { //tar inn 3 pins til LED-lys og en streng til passord.
    _passord = passord;
    _poeng[0] = poeng1;
    _poeng[1] = poeng2;
    _poeng[2] = poeng3;

    for(int poengPin : _poeng) {
        pinMode(poengPin, OUTPUT);
    }
}

//--------------------manipulerer poengsum i form av LED-lys.--------------------

void Person::giPoeng() {
    for (int poeng : _poeng) {
        if (digitalRead(poeng) == LOW) {
            digitalWrite(poeng, HIGH);
            return;
        }
    } 
}

void Person::fjernAllePoeng() {
    for (int poeng : _poeng) {
      digitalWrite(poeng, LOW);
    } 
}

void Person::fjernEttPoeng() {
    for (int i = 2; i >= 0; i--) {
        if (digitalRead(_poeng[i]) == HIGH) {
            digitalWrite(_poeng[i], LOW);
            return;
        }
    }
}

//blinker alle poenglysene til personen i 10sek dersom personen har 3 poeng (har vunnet).
bool Person::giGevinst() {
  if (digitalRead(_poeng[2]) == HIGH) {
    // unsigned long startGevinsttid = millis();
    unsigned long sluttGevinsttid = millis() + 10000;
    while (sluttGevinsttid > millis()) {
      digitalWrite(_poeng[0], LOW);
      digitalWrite(_poeng[1], LOW);
      digitalWrite(_poeng[2], LOW);
      delay(200);
      digitalWrite(_poeng[0], HIGH);
      digitalWrite(_poeng[1], HIGH);
      digitalWrite(_poeng[2], HIGH);
      delay(200);
    }
    return true;
  } 
  return false;
}

bool Person::sjekkPassord(String string) { 
    if (string == _passord) {
        return true;
    }
    return false;
}

//derfinerer '==' for Person-objekter.
bool Person::operator==(Person& annen) {
  if (this == &annen) {
    return true;
  }
  return false;
}