 // board:mk20dx256

#include "config.h"
#include "serial.hpp"
#include "testBle.hpp"

unsigned int baud_hm10 = 0;
CoreNetwork *coreNetwork;
bool test = false;

void setup()
{
  Serial.begin(9600);
  delay(1000);
    pinMode(LED, OUTPUT);
}

void loop()
{
  delay(1000);
  if (!test)
  {
    coreNetwork = new CoreNetwork(Serial, Serial1);
    if (coreNetwork->getBauds() > 0)
      test = true;
  }
  delay(5000);
  if (test)
  {
     coreNetwork->pullFunction(String("TOTO"));
  }
  Serial.print("End LOOP\n");
}
