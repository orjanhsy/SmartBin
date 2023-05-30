#include "Arduino.h"
#include "Avstandsmaaler.h"

Avstandsmaaler::Avstandsmaaler(int echoPin, int pingPin) { // tar inn 2 digitale pins til å sende/motta signal fra sensoren.
  _echoPin = echoPin;
  _pingPin = pingPin;

  pinMode(echoPin, INPUT);
  pinMode(pingPin, OUTPUT);
}

//gjør tiden signalet bruker om til avstand.
long Avstandsmaaler::tidTilAvstand(long microseconds) {
  return microseconds * 0.034/2;  // deles på 2 siden signalet skal både frem og tilbake, altså går det avstanden 2 ganger.
}

//sender ut signal og returnerer distansen signalet reiser.
long Avstandsmaaler::distanseTilObjekt() {
  digitalWrite(_pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(_pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_pingPin, LOW);
  _duration = pulseIn(_echoPin, HIGH);
  _cm = tidTilAvstand(_duration);
  return _cm;
}


