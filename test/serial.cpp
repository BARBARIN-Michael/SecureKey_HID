#include "serial.hpp"


CoreNetwork::CoreNetwork()
{
  int baud_hm10;
  isWritable = true;
  this->serial = new HardwareSerial();
  this->serial->begin(9600);
  this->serial_hm10 = new SoftwareSerial(0, 1);
    
  testBle->detectBleBaudRate(&baud_hm10);
  this->serial_hm10->begin(baud_hm10);
}

unsigned int CoreNetwork::hash(String s)
{
    unsigned int hash = InitialFNV;
    unsigned int i = 0;
    while (i < s.length())
    {
        hash = hash ^ (s[i]);
        hash = hash * FNVMultiple;
        ++i;
    }
    return hash;
}

void CoreNetwork::pullFunction(String str)
{
  unsigned int hashArray[6] = {  hash(String("stopWrite")),
                            hash(String("startWrite")),
                            hash(String("stopDebug")),
                            hash(String("startDebug")),
                            hash(String("stopDebug")),
                            hash(String("startLoop")) };
    this->serial->print(hashArray[0]);
       /*                   };
    switch(hash(str))
    {
        case hashArray[0]:
            setWritable(false);
            break ;
        case hashArray[1]
            setWritable(true);
            break ;
        case hashArray[2]
            #undef DEBUG
            #define DEBUG false
            break ;
        case hashArray[3]
            #undef DEBUG
            #define DEBUG true
            break ;
        case hashArray[4]
            #undef loop
            #define loop false
            break ;
        case hashArray[5]
            #undef loop
            #define loop true
            break ;
         default:
            this->serial->print("Command not found");
    }*/
}

void CoreNetwork::ATFunction(String str)
{
    if (DEBUG)
    {
        this->serial->print("{ -> }\t");
        this->serial->println(str);
    }
    if (isWritable)
        this->serial_hm10->print(str);
    else
        this->serial->print("cannot write in HM10, please send command startWrite for start write");
}

void CoreNetwork::getAndSendSerial_hm10()
{
    String str = "";

    if (this->serial_hm10->available())
    {
        str = this->serial_hm10->readString();
        if (DEBUG)
        {
            this->serial->print("{ <- }\t");
            this->serial->println(str);
        }
    }
}

void CoreNetwork::getAndSendSerial()
{
  String str = "";

  if (this->serial->available())
  {
    str = this->serial->readString();
    if (str.substring(0, 2) != "AT")
      this->pullFunction(str);
    else
    {
        this->ATFunction(str);
        if (DEBUG)
            this->getAndSendSerial_hm10();
    }
  }
}

HardwareSerial *CoreNetwork::getSerial()
{
    return this->serial;
}

SoftwareSerial *CoreNetwork::getSerial_hm10()
{
    return this->serial_hm10;
}

void CoreNetwork::setWritable(bool val)
{
    this->isWritable = val;
}
