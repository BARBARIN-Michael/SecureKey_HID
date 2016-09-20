 // board:mk20dx256

#include "config.h"
#include "serial.hpp"
#include "testBle.hpp"

unsigned int baud_hm10 = 0;
CoreNetwork *serial;
int test = 0;

void setup()
{
  Serial.begin(9600);
  serial = new CoreNetwork();
    pinMode(LED, OUTPUT);
}

void loop()
{
  Serial.print("test\n");
  delay(10000);
  //if (test == 0)
     // serial->pullFunction(String("TOTO"));
   test = 1;
}
