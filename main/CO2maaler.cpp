#include "Arduino.h"
#include "CO2maaler.h"

CO2maaler::CO2maaler(int anPin) { //tar inn en analog pin til å lese fra sensoren.
    _anPin = anPin;
    pinMode(_anPin, INPUT);
}

//måler gjennomsnittet av CO2 over 2 sekund og returnerer det under enheten 'parts-per-million', ppm.
int CO2maaler::maalOver2Sek() {
    //fjernet CO2 liste ---------------debug-----------------
    _co2Average = 0;
    //gjør 10 målinger på totalt 2 sekund.
    for(int i = 0; i<10; i++) {
        _co2Average += analogRead(_anPin);
        delay(200);
    }
    return _co2Average/10;
}


