// #include "HID.hpp"

void command(const char *command)
{
	Serial1.write(command);

	delay(500);

	while (Serial1.available())
	{
		Serial.write(Serial1.read());
	}
	Serial.println();

	delay(100);
}

void setup()
{
	Serial1.begin(9600);

	command("AT+POWE3"       );
	command("AT+NAMESafelock");
	command("AT+CHAR0xFFE1"  );
	command("AT+RELI0"       );
	command("AT+SHOW1"       );
	command("AT+MODE2"       );
}

void loop()
{
	while(Serial1.available())
	{
		Serial.write(Serial1.read());
	}
	delay(100);

	while(Serial.available())
	{
		Serial1.write(Serial.read());
	}
	delay(100);
}
