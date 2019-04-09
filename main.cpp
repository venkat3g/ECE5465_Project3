#include "TSISensor.h"
#include "mbed.h"

int main(void) {
  PwmOut red(LED_GREEN);
  PwmOut green(LED_RED);
  PwmOut blue(LED_BLUE);
  TSISensor tsi;
  Serial other(D0, D1);

  while (true) {
    if (other.readable()) {
      int val = (int)other.getc();
    }
    if (other.writeable()) {
      char val = (char)100 * tsi.readPercentage();
    }
    red = 1.0 - tsi.readPercentage() * 0.5;
    green = 1.0 - tsi.readPercentage() * 0.1;
    blue = 1.0 - tsi.readPercentage() * 0.2;
    wait(0.1);
  }
}