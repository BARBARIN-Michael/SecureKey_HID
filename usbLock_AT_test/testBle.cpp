#include "testBle.hpp"

Ble::Ble(usb_serial_class &serial, HardwareSerial &serial_hm10) : _serial(serial), _serial_hm10(serial_hm10)
{
}

bool Ble::analyseBleBaudRate(usb_serial_class &serial, HardwareSerial &serial_hm10, int baud)
{
    delay(50);
    serial.print("test analyse: "); serial.print(baud); serial.print("\n");
    serial_hm10.begin(baud);
    serial_hm10.write("AT");
    serial_hm10.flush();
    delay(50);
    if (serial_hm10.readString() == "OK")
    {
        if (DEBUG)
            serial.println("BLE OK");
        return true;
    }
    else
        serial_hm10.end();
    return false;
}

bool Ble::analyseBleBaudRate(int baud)
{
    delay(50);
    _serial_hm10.begin(baud);
    _serial_hm10.write("AT");
    _serial_hm10.flush();
    delay(50);
    if (_serial_hm10.readString() == "OK")
    {
        if (DEBUG)
            _serial.println("BLE OK");
        return true;
    }
    else
        _serial_hm10.end();
    return false;
}

bool Ble::detectBleBaudRate(usb_serial_class &serial, HardwareSerial &serial_hm10, int *baud)
{
    int i = 0;
    String response = "";

    serial.print("detect\n");

    while (i < NBBAUDS)
    {
        if (analyseBleBaudRate(serial, serial_hm10, bauds[i])) {
            *baud = bauds[i];
            return true;
        }
    }
    return false;
}

bool Ble::detectBleBaudRate(int *baud)
{
    int i = 0;
    String response = "";
    this->_serial.print("detectBLE\n");

    while (i < NBBAUDS)
    {
        if (analyseBleBaudRate(this->_serial, this->_serial_hm10, bauds[i])) {
            *baud = bauds[i];
            return true;
        }
        ++i;
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
