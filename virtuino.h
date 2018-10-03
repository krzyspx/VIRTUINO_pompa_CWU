// Wszystko co niezbędne dla uruchomienia VIRTUINO na ESP8266 z watchdogiem na wypadek zawieszenia programu

char ssid[] = "**********";
char pass[] = "*************";

#include <ESP8266WiFi.h>

#include "Virtuino_ESP_WifiServer.h"
WiFiServer server(8000);                    //port 8000
Virtuino_ESP_WifiServer virtuino(&server);

#include <Ticker.h> //do watchdoga
Ticker tickerOSWatch;
#define OSWATCH_RESET_TIME 9 //wd w sek

static unsigned long last_loop; //dla WD
void ICACHE_RAM_ATTR osWatch(void) { //watchdog 30 sek
  unsigned long t = millis();
  unsigned long last_run = abs(t - last_loop);
  if (last_run >= (OSWATCH_RESET_TIME * 1000)) {
    // save the hit here to eeprom or to rtc memory if needed
    ESP.restart();  // normal reboot
    //ESP.reset();  // hard reset
  }
}

int flaga_led1 = 1;
void virtuino_timer() { //miganie vLED w aplikacji
  flaga_led1 = !flaga_led1;
  virtuino.vDigitalMemoryWrite(0, flaga_led1);
}

void virtuino_setup() {
  akcja.attach(1, 2000, virtuino_timer);

  //stały IP
  if (procesor == D1MINI)   Serial.println(""); Serial.println("Connecting to " + String(ssid));
  IPAddress ip(192, 168, 2, 177); IPAddress gateway(192, 168, 2, 1);  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  //wachdog
  last_loop = millis();
  tickerOSWatch.attach_ms(((OSWATCH_RESET_TIME / 3) * 1000), osWatch);

  virtuino.DEBUG = true;  virtuino.DEBUG1 = false;

  virtuino.password = "1234";                  // Use only numbers or text characters
  server.begin();
  if (procesor == D1MINI) Serial.println("VIRTUINO Server started");
}

void virtuino_akcja() {
  last_loop = millis(); //wd
  virtuino.run();
}

int getTextID() {    // odczyt numeru kanału odebranej wiadomości - ekstra funkcja wymagająca zmiany w bibliotece VIRTUINO
  if (virtuino.textReceivedCommandBuffer.length() > 5) {
    char c = virtuino.textReceivedCommandBuffer.charAt(3);
    return ((uint8_t) c) - 48;
  }
  return -1;
}

