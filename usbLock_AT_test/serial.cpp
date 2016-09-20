#include "serial.hpp"


CoreNetwork::CoreNetwork(usb_serial_class &serial, HardwareSerial &serial_hm10) : _serial(serial), _serial_hm10(serial_hm10)
{
  int baud_hm10;
  serial.begin(9600);
  delay(1000);
  serial.print("Initialize serial hm10\n");
  _isWritable = true;
  _testBle = new Ble(serial, serial_hm10);
  _testBle->detectBleBaudRate(&baud_hm10);
  this->_serial_hm10.begin(baud_hm10);
}

CoreNetwork::CoreNetwork(usb_serial_class &serial, HardwareSerial &serial_hm10, int bauds_serial) : _serial(serial), _serial_hm10(serial_hm10)
{
  int baud_hm10;
  serial.begin(9600);
  delay(1000);
  serial.print("Initialize serial hm10\n");
  _isWritable = true;
  _testBle = new Ble(serial, serial_hm10);
  _testBle->detectBleBaudRate(&baud_hm10);
  this->_serial_hm10.begin(baud_hm10);
}

unsigned int CoreNetwork::hash(String s)
{
    unsigned int hash = this->_InitialFNV;
    unsigned int i = 0;
    while (i < s.length())
    {
        hash = hash ^ (s[i]);
        hash = hash * this->_FNVMultiple;
        ++i;
    }
    return hash;
}

void CoreNetwork::pullFunction(String str)
{
/*  unsigned int hashArray[6] = {  hash(String("stopWrite")),
                                              hash(String("startWrite")),
                                              hash(String("stopthis->_debug")),
                                              hash(String("startthis->_debug")),
                                              hash(String("stopLoop")),
                                              hash(String("startLoop"))
                                            };


int i = 0;
while (i < 6)
{
  this->_serial.print(hashArray[i]);
  this->_serial.print("\n");
  ++i;
}
*/
                    
    switch(hash(str))
    {
        case 1640957532: //stopWrite
            this->_serial.print("stopWrite OK\n");
            setWritable(false);
            break ;
        case 2810289378: //startWrite
            this->_serial.print("startWrite OK\n");
            setWritable(true);
            break ;
        case 4228732056: //stopthis->_debug
            this->_serial.print("DEBUG OFF\n");
            this->_debug = false;
            break ;
        case 1169187646: //startthis->_debug
            this->_serial.print("DEBUG ON\n");
            this->_debug = true;
            break ;
        case 1950459243: // stopLoop
            this->_serial.print("Stop loop\n");
            this->_loop = false;
            break ;
        case 1161153397: // startLoop
            this->_serial.print("Start loop\n");
            this->_loop = true;
            break ;
         default:
            this->_serial.print("Command not found :");
            this->_serial.print(str);
            this->_serial.print("( id: ");
            this->_serial.print(hash(str));
            this->_serial.print(" )");
            this->_serial.print("\n");
            break ;
    }
}

void CoreNetwork::ATFunction(String str)
{
    if (this->_debug)
    {
        this->_serial.print("{ -> }\t");
        this->_serial.println(str);
    }
    if (_isWritable)
        this->_serial_hm10.print(str);
    else
        this->_serial.print("cannot write in HM10, please send command startWrite for start write\n");
}

void CoreNetwork::getAndSendSerial_hm10()
{
    String str = "";

    if (this->_serial_hm10.available())
    {
        str = this->_serial_hm10.readString();
        if (this->_debug)
        {
            this->_serial.print("{ <- }\t");
            this->_serial.println(str);
        }
    }
}

void CoreNetwork::getAndSendSerial()
{
  String str = "";

  if (this->_serial.available())
  {
    str = this->_serial.readString();
    if (str.substring(0, 2) != "AT")
      this->pullFunction(str);
    else
    {
        this->ATFunction(str);
        delay(20);
        if (this->_debug)
            this->getAndSendSerial_hm10();
    }
  }
}

usb_serial_class &CoreNetwork::getSerial()
{
    return this->_serial;
}

HardwareSerial &CoreNetwork::getSerial_hm10()
{
    return this->_serial_hm10;
}

unsigned int CoreNetwork::getBauds_hm10()
{
  return this->_bauds_hm10;
}

void CoreNetwork::setBauds_hm10(unsigned int bauds)
{
  this->_bauds_hm10 = bauds;
}

void CoreNetwork::setWritable(bool val)
{
    this->_isWritable = val;
}
