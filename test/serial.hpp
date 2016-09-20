#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include "testBle.hpp"
#include "config.h"

class CoreNetwork
{
  private:
    const unsigned int InitialFNV = 2166136261U;
    const unsigned int FNVMultiple = 16777619;
    bool isWritable;
    HardwareSerial *serial;
    SoftwareSerial *serial_hm10;
    Ble *testBle;

  public:
    CoreNetwork();
    void pullFunction(String str);
    void ATFunction(String str);
    void getAndSendSerial();
    void setWritable(bool val);
    void getAndSendSerial_hm10();
    unsigned int hash(String str);
    HardwareSerial *getSerial();
    SoftwareSerial *getSerial_hm10();
};
