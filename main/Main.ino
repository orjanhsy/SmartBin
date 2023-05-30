#include "Avstandsmaaler.h"
#include "CO2maaler.h"
#include "Kodelaas.h"
#include "Person.h"
#include <ezButton.h>

//definerer bruksområde for pins:
int servoPin = 5;
int echoPin = 6;
int pingPin = 7;
ezButton knappA(2);
ezButton knappB(3);
ezButton knappC(4);
ezButton laasOppVedFeil(12);

//instanser av maalere, laas og én person:
Kodelaas laasen(servoPin);
Avstandsmaaler avstandsmaaler(echoPin, pingPin);
CO2maaler co2maaler(A0);
Person egil("ABBC", 9,10,11);

//definerer andre variabler i programmet:
Person personer[] = {egil};
bool erLukket;
bool erLaast;

//referansemålinger til sensorene.
int daarligLukt = 600; //CO2, testet grense for når mat typisk begynner å lukte vondt.
//avstander:      
int fulltSoppel = 6;
int tomtSoppelUtenPose = 15; 
int tomtSoppelMedPose = 14; 

//delay-tider:
int halvTime = 1000*60*30; //brukes som delay etter poeng er gitt, for å motvirke juks.
int debounceDelay = 80;
int lukketid = 2000;
int avventSjekk = 500;
int laasemargin = 500;

void setup() {
  Serial.begin(9600); //starter seiel monitor for debugging.
  laasen.attach();  //knytter servoen låsen bruker til en pin som er oppgitt når et objekt av Kodelaas opprettes. 

  //bestemmer debouncetid for knappene.
  knappA.setDebounceTime(debounceDelay);
  knappB.setDebounceTime(debounceDelay);
  knappC.setDebounceTime(debounceDelay);
  laasOppVedFeil.setDebounceTime(debounceDelay);
}

void loop() {
  //låser visst CO2-nivået er for høyt (indikasjon på at det lukter).
  if (co2maaler.maalOver2Sek() > daarligLukt && sjekkAtLukket()) {
    Serial.println("Laast - CO2");
    erLaast = true;
    delay(laasemargin);
    laasen.laas();
  }

  //låser visst avstand til søppelet blir for kort (altså det er fult).
  if (avstandsmaaler.distanseTilObjekt() < fulltSoppel && sjekkAtLukket()) {
    Serial.println("Laast - Fullt");
    erLaast = true;
    delay(laasemargin);
    laasen.laas();
  }

  String passordforsoek = "";
  while (erLaast) {
    //oppdaterer tilstanden til passord-knappene.
    knappA.loop();
    knappB.loop();
    knappC.loop();
    
    //sjekker om knapper trykkes, legger til bokstaver i passordforsøket.
    if (knappA.isPressed()) {
      passordforsoek += "A";
      Serial.println("A");
    }
    if (knappB.isPressed()) {
      passordforsoek += "B";
      Serial.println("B");
    }
    if (knappC.isPressed()) {
      passordforsoek += "C";
      Serial.println("C");
    }

    //sjekker om de siste 4 bokstavene trykket inn passer med noen av personene sitt passord.
    Person* sisteTilAaLaaseOpp = nullptr;
    bool funnetPerson = false;
    sjekkPassordMotPersoner(passordforsoek.substring(passordforsoek.length() - 4), sisteTilAaLaaseOpp, funnetPerson);

    //venter på at bruker skal åpne og lukke bøtten.
    while(sjekkAtLukket() && !erLaast) {
      Serial.println("Venter paa at lokket skal aapnes.");
      delay(avventSjekk);
    }
    while(!sjekkAtLukket()) {
      Serial.println("Venter paa at lokket skal lukkes.");
      Serial.println(avstandsmaaler.distanseTilObjekt());
      delay(avventSjekk);
    }

    //hvis en person har skrevet inn riktig passord, - gi/fjern poeng dersom de har tømt søppelet.
    if (funnetPerson) {
      delay(lukketid);  //gir brukeren tid til å lukke lokket helt.
      oppdaterLaasenBasertPaaHandling();
      if (sjekkAtLukket()) {  //visst bøtten har blitt lukket slik den skal
        oppdaterPoeng(*sisteTilAaLaaseOpp);
      } else {
        while (!sjekkAtLukket()) { //håndterer tilfeller hvor avstandsmåleren er streifet mens lokket er åpent.
          Serial.println("Venter med aa oppdatere poengsum til lokket er lukket.");
          if (erLaast) {
            //når lokket er åpent vil denne knappen håndtere tilfeller hvor bøtten har gått i lås.
            laasOppVedFeil.loop();
            if (laasOppVedFeil.isPressed()) { 
              erLaast = false;
              delay(laasemargin);
              laasen.laasOpp();
            }
          }
        }
        delay(lukketid);  //gir bruker tid til å lukke lokket helt.
        oppdaterLaasenBasertPaaHandling();
        oppdaterPoeng(*sisteTilAaLaaseOpp);
      }
    }
  } 
}

//sjekker hvorvidt bøtten er lukket.
bool sjekkAtLukket() {
  if (avstandsmaaler.distanseTilObjekt() > tomtSoppelUtenPose) {
    erLukket = false;
  } else {
    erLukket = true;
  }
  return erLukket;
}

//sjekker om et gitt passord passer med noen av de registrerte personene sitt, låser opp hvis det er tilfelle.
void sjekkPassordMotPersoner(String forsoek, Person*& aktuellPerson, bool& funnetPerson) {
  for (Person& person: personer) {
      if (person.sjekkPassord(forsoek)) {
        Serial.println("Fant person");
        laasen.laasOpp();
        erLaast = false;
        aktuellPerson = &person;
        funnetPerson = true;
        return ;
      }
  }
}

//laaser bøtten visst den er full, låser opp visst den er ca. tom.
void oppdaterLaasenBasertPaaHandling() {
  if (avstandsmaaler.distanseTilObjekt() > tomtSoppelMedPose - 3) {  // -3 er brukt som margin for å unngå feil som kan oppstå pga. av posen. 
    erLaast = false;
    Serial.println("Laast opp - gikk ut med soppel");
    delay(laasemargin);
    laasen.laasOpp();
  } else {
      erLaast = true;
      Serial.println("Laast - tok ikke ut soppel");
      delay(laasemargin);
      laasen.laas();
  }
}

//endrer poengsum basert på låsens tilstand.
void oppdaterPoeng(Person person) {
  Person* skalObserveres;
  skalObserveres = &person;
  if (!erLaast) { //gir poeng til personen dersom de har tatt ut søppelet.
    skalObserveres -> giPoeng();
    if (skalObserveres -> giGevinst()) {  //"gir gevinst" dersom poengsummen er full ved å blinke poeng-lysene.
      restartPoengsummer();
    }
    delay(halvTime);
  } else {
      skalObserveres -> fjernEttPoeng();
    }
}

//restarter alle brukernes poengsummer.
void restartPoengsummer() {
  for (Person person : personer) {
    person.fjernAllePoeng();
  }
}