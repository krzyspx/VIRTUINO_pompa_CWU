
//#define pirpompa 14013756
#define pirpompa 5526612 //k3off
//#define pompaoff 4478228 //zmienic
//#define pompaon  4478225
#define pompaoff 5522772  //test
#define pompaon  5522769
#define pompapilot 7730435 //

#define przekaznik   0  //przekaznik w ESP-01S Relay

void pompazal();
void pompawyl();
int czasobiegu = 10;  // ilość sek włączenia pompy x 2
int obieg = -2;

#include <EEPROM.h>
#define mem_conf 100    // początek zapisu konfiguracji we flash
void save_conf () { //zapisz tabele do flash

  EEPROM.begin(1024);
  EEPROM.put(mem_conf, czasobiegu);
  EEPROM.end(); //  EEPROM.commit();
}

void read_conf() { // odczyt tabeli z flash

  EEPROM.begin(1024);
  EEPROM.get(mem_conf, czasobiegu);
  EEPROM.end(); //  EEPROM.commit();
}

int vd10 = 0;
int stvd10 = 0;
int v13 = 0;
int stv13 = 0;

void read_vD10() {
  vd10 = virtuino.vDigitalMemoryRead(10);            // Read virtual memory 10 from Virtuino app
  if (vd10 == stvd10) ; else {
    if (procesor == D1MINI) Serial.println("-------Virtual pin v10 is changed to=" + String(vd10));
    if (vd10 == 1) obieg = czasobiegu;
    stvd10 = vd10;
  }
}
void read_v13() {
  v13 = virtuino.vMemoryRead(13);
  if (v13 == stv13) ; else {
    czasobiegu = v13;
    save_conf ();
    stv13 = v13;
  }
}

void virtuino_read() { //czytanie danych przesłanych z aplikacji
  read_vD10();
  read_v13();
  //  read_ch1();
}


void pompasetup() {
  pinMode(przekaznik, OUTPUT); //przekaznik w ESP-01S Relay
  digitalWrite(przekaznik, HIGH);

  read_conf();
  virtuino.vMemoryWrite(13, czasobiegu);
}
void pompazal() //wysyłanie kodu do załączenia pompy
{
  virtuino.vDigitalMemoryWrite(10, 1);
  sendkodpilot(pompaon);
  digitalWrite(przekaznik, LOW);
  virtuino.vMemoryWrite(11, obieg);
  akcja.updateInterval(2, 1000);
}
void pompawyl() //wysyłanie kodu do wyłączenia pompy
{
  virtuino.vDigitalMemoryWrite(10, 0);
  sendkodpilot(pompaoff);
  digitalWrite(przekaznik, HIGH);
  virtuino.vMemoryWrite(11, 0);
  akcja.updateInterval(2, 2000);
}
void pompaakcja() {
  if ((o_kod == pirpompa) || (o_kod == pompapilot)) {
    obieg = czasobiegu;
    pompazal();
    o_kod =  0;
    //   Serial.println(pompaon);
  }
}

void pompa_timer() {
  if (obieg < -2) obieg = -2; else {
    if (obieg < 1) {
      pompawyl();
    }  else {
      pompazal();
    }
  }
  obieg--;
}

int oldtimeronoff = 1;
void pomparunpir() { //sprawdzenie czy mozna włączyć pompę w odpowiednim przedziale czasu
  if (oldtimeronoff == 1) {
    obieg = czasobiegu;
    pompazal();
  }
}


