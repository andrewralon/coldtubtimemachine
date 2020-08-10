#include <OneWire.h>
#include <DallasTemperature.h>

#define GETNAMEOF(name) getnameof(#name, (name))
#define ONE_WIRE_BUS 2			// Data wire is plugged into Arduino port 2
#define TEMPERATURE_PRECISION 9 // Temperature sensor precision in bits

// Setup a OneWire instance, pass it to DallasTemperature, and create sensor objects
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress waterHigh, waterLow, airAmbient;
DeviceAddress devices[] = {
	waterHigh,
	waterLow,
	airAmbient}

const int deviceCount = 3;
const int timeDelay = 1000;
float tempC = 0;
float tempF = 0;

// Setup - Initialize the sensors here
void setup(void)
{
	Serial.println("O HAI");

	// Start serial port and library
	Serial.begin(9600);
	sensors.begin();

	// Get all devices on the bus
	Serial.print("Locating devices... ");
	Serial.print("Found ");
	Serial.print(sensors.getDeviceCount(), DEC);
	Serial.println(" devices.");

	// Get parasite power requirements
	Serial.print("Parasite power: ");
	if (sensors.isParasitePowerMode())
		Serial.println("ON");
	else
		Serial.println("OFF");

	// Get the addresses for all devices
	oneWire.reset_search();

	for (int index = 0; index < deviceCount; index++)
	{
		GETNAMEOF(devices[index]);
		Serial.println();

		if (oneWire.search(devices[index]))
		{
			// Show the addresses found on the bus
			Serial.print("Address: ");
			printAddress(devices[index]);
			Serial.println();

			// Show device resolutions - BEFORE
			Serial.print("Resolution (before): ");
			Serial.println(sensors.getResolution(devices[index]), DEC);

			// Set the device resolutions to 9 bits
			sensors.setResolution(devices[index], TEMPERATURE_PRECISION);

			// Show device resolutions - BEFORE
			Serial.print("Resolution (after):  ");
			Serial.println(sensors.getResolution(devices[index]), DEC);
		}
		else
		{
			Serial.println("*** Address not found ***");
		}
	}
}

// Function to get the name of a variable
void getnameof(char *name, int value) // char[] ???
{
	// Serial.print("name: %s\tvalue: %d\n", name, value);
	Serial.print(name);
}

// Function to show device address
void printAddress(DeviceAddress deviceAddress)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		// Pad the address with zeroes if necessary
		if (deviceAddress[i] < 16)
			Serial.print("0");
		Serial.print(deviceAddress[i], HEX);
	}
}

// Function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
	float tempC = sensors.getTempC(deviceAddress);
	float tempF = DallasTemperature::toFahrenheit(tempC);

	if (tempC < 10)
		Serial.print(" ");
	Serial.print(tempC);
	Serial.print(" C / ");
	if (tempF < 10)
		Serial.print(" ");
	Serial.print(tempF);
	Serial.print(" F");
}

// Main - Get and show temperatures
void loop(void)
{
	// Request temperatures from all bus devices
	Serial.print("Requesting temperatures.... ");
	sensors.requestTemperatures();
	Serial.println("Done.");

	// Show the device info
	for (int index = 0; index < deviceCount; index++)
	{
		GETNAMEOF(devices[index]);
		Serial.print(": ");
		printTemperature(deviceAddress);
		Serial.println();
	}

	delay(timeDelay);
}
