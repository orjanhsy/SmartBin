#ifndef Person_h
#define Person_h
#include "Arduino.h"

class Person {
    private:
        String _passord;
        int _poeng[3];
    
    public:
        Person(String passord, int poeng1, int poeng2, int poeng3);

        void  giPoeng();
        bool  giGevinst();
        void  fjernEttPoeng();
        void fjernAllePoeng();
        bool sjekkPassord(String string);
        bool operator==(Person &annen);
};
#endif