#include "testBle.hpp"

Ble::Ble(HardwareSerial *serial, SoftwareSerial *serial_hm10)
{
    this->serial = serial;
    this->serial_hm10 = serial_hm10;
}

bool Ble::analyseBleBaudRate(HardwareSerial *serial, SoftwareSerial *serial_hm10, int baud)
{
    delay(50);
    serial_hm10->begin(baud);
    serial_hm10->write("AT");
    serial_hm10->flush();
    delay(50);
    if (serial_hm10->readString() == "OK")
    {
        if (DEBUG)
            serial->println("BLE OK");
        return true;
    }
    else
        serial_hm10->end();
    return false;
}

bool Ble::detectBleBaudRate(HardwareSerial *serial, SoftwareSerial *serial_hm10, int *baud)
{
    int i = 0;
    String response = "";

    while (i < NBBAUDS)
    {
        if (analyseBleBaudRate(serial, serial_hm10, bauds[i])) {
            *baud = bauds[i];
            return true;
        }
    }
}
bool Ble::analyseBleBaudRate(int baud)
{
    delay(50);
    serial_hm10->begin(baud);
    serial_hm10->write("AT");
    serial_hm10->flush();
    delay(50);
    if (serial_hm10->readString() == "OK")
    {
        if (DEBUG)
            serial->println("BLE OK");
        return true;
    }
    else
        serial_hm10->end();
    return false;
}

bool Ble::detectBleBaudRate(int *baud)
{
    int i = 0;
    String response = "";

    while (i < NBBAUDS)
    {
        if (analyseBleBaudRate(serial, serial_hm10, bauds[i])) {
            *baud = bauds[i];
            return true;
        }
    }
    return false;
}

bool Ble::testBLE(int baud)
{
    if (!analyseBleBaudRate(baud))
    {
        if (DEBUG)
            digitalWrite(LED, LOW);
        return false;
    }
    if (DEBUG)
        digitalWrite(LED, HIGH);
    return true;
}
