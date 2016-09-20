#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include "testBle.hpp"
#include "config.h"

class CoreNetwork
{
  private:
    const unsigned int _InitialFNV = 2166136261U;
    const unsigned int _FNVMultiple = 16777619;
    bool _isWritable;
    usb_serial_class &_serial;
    HardwareSerial &_serial_hm10;
    Ble *_testBle;
    unsigned int _bauds_hm10;
    boolean _debug;
    boolean _loop;

  public:
    CoreNetwork(usb_serial_class &serial, HardwareSerial &serial_hm10);
    CoreNetwork(usb_serial_class &serial, HardwareSerial &serial_hm10, int bauds_serial);
    void pullFunction(String str);
    void ATFunction(String str);
    void getAndSendSerial();
    void setWritable(bool val);
    void getAndSendSerial_hm10();
    unsigned int hash(String str);
    usb_serial_class &getSerial();
    HardwareSerial &getSerial_hm10();
    unsigned int getBauds_hm10();
    void setBauds_hm10(unsigned int bauds);
};
