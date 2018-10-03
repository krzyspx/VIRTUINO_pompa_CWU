#include <RCSwitch.h> // biblioteka obsługi nadawania i odbioru 433 MHz
RCSwitch mySwitch = RCSwitch();

unsigned long o_kod = 0;

void no433setup()
{
  if (procesor == D1MINI) {
    mySwitch.enableReceive(12);  // port odbiornika
    mySwitch.enableTransmit(14); // nadajnika
  }
  if (procesor == ESP01) {
    mySwitch.enableReceive(3);  //
    mySwitch.enableTransmit(1); //
  }
}

String mess;
void no433akcja()
{
  if (mySwitch.available()) {
    mess = "Kod " + String(mySwitch.getReceivedValue()) + " / " + String(mySwitch.getReceivedBitlength()) + " IMP " + String(mySwitch.getReceivedDelay())
           + " Prot  "  + " / " + String(mySwitch.getReceivedProtocol());
    if (procesor == D1MINI) Serial.println(mess);
    virtuino.sendText(0, mess);
    o_kod = mySwitch.getReceivedValue();
    mySwitch.resetAvailable();
  }
}

void sendkod(unsigned long code, int impuls, byte bity, byte prot) { // wysłanie kodu z routera
  mySwitch.setPulseLength(impuls);
  mySwitch.setProtocol(prot);
  mySwitch.setRepeatTransmit(4);
  mySwitch.send(code, bity);
}

void sendkodpilot (unsigned long code) {
  sendkod(code, 340, 24, 1);
}

