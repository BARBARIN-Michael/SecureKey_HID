#pragma once

#include <WProgram.h>
#include "config.h"
#include <SoftwareSerial.h>

#define NBBAUDS 9

class Ble {
    protected:
        unsigned int bauds[NBBAUDS] = {
            9600,
            57600,
            115200,
            19200,
            38400,
            4800,
            2400,
            1200,
            230400
        };

    private:
      usb_serial_class &_serial;
      HardwareSerial &_serial_hm10;

    public :
        Ble(usb_serial_class &serial, HardwareSerial &serial_hm10);
        bool detectBleBaudRate(int *baud);
        bool analyseBleBaudRate(int baud);
        bool analyseBleBaudRate(usb_serial_class &serial, HardwareSerial &serial_hm10, int baud);
        bool detectBleBaudRate(usb_serial_class &serial, HardwareSerial &serial_hm10, int *baud);
        bool analyseBaudRate(usb_serial_class &serial, HardwareSerial &serial_hm10, int *baud);
        bool testBLE(int baud);
};
