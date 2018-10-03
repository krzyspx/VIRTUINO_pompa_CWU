#define ESP01 1
#define D1MINI 2
int procesor = D1MINI;

#include <Timers.h> //  my favorite timer 
Timers <7> akcja; //

#include "virtuino.h"
#include "led.h"
#include "NO433.h" // obsługa transmisji 433 MHz
#include "pompa.h" // 

void setup()
{
  ledsetup();
  if (procesor == D1MINI) {
    Serial.begin(115200);  delay(100);
    Serial.println();  Serial.println(F(__FILE__));  //Arduino IDE.cc 1.8.3
  }

  //  akcja.attach(1, 2000, virtuino_timer);
  akcja.attach(2, 2000, led_timer);
  akcja.attach(3, 2000, pompa_timer); //

  virtuino_setup();
  no433setup();  //setup 433 MHz
  pompasetup();

}
void loop()
{
  akcja.process(); // timer
  virtuino_akcja();
  virtuino_read();
  no433akcja();
  pompaakcja();
}








