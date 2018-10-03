
#define led_ziel 16      //program OK miganie led
#define sw   13
#define led_wew   2

void ledsetup() {
  pinMode(led_wew, OUTPUT); //LED na module
  digitalWrite(led_wew, HIGH);
  pinMode(led_ziel, OUTPUT); //LED na module
  digitalWrite(led_ziel, HIGH);

  pinMode(sw, INPUT_PULLUP);
}

int flaga_led = 0;
void led_timer()
{
  flaga_led = !flaga_led;
  digitalWrite(led_ziel, flaga_led);
}
