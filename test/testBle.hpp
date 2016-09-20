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
        HardwareSerial *serial;
        SoftwareSerial *serial_hm10;

    public :
        Ble(HardwareSerial *serial, SoftwareSerial *serial_hm10);
        bool detectBleBaudRate(int *baud);
        bool analyseBleBaudRate(int baud);
        bool analyseBleBaudRate(HardwareSerial *serial, SoftwareSerial *serial_hm10, int baud);
        bool detectBleBaudRate(HardwareSerial *serial, SoftwareSerial *serial_hm10, int *baud);
        bool analyseBaudRate(HardwareSerial *serial, SoftwareSerial *serial_hm10, int *baud);
        bool testBLE(int baud);
};
