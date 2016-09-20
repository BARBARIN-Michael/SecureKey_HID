 // board:mk20dx256

#include "config.h"
#include "serial.hpp"
#include "testBle.hpp"

unsigned int baud_hm10 = 0;
CoreNetwork *coreNetwork;
bool test = false;

void setup()
{
    pinMode(LED, OUTPUT);
}

void loop()
{
  if (!test)
  {
    coreNetwork = new CoreNetwork(Serial, Serial1);
    if (coreNetwork->getBauds_hm10() > 0)
      test = true;
  }
  if (test)
    coreNetwork->getAndSendSerial();
}
