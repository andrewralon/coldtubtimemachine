#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2 // Data wire is plugged into Arduino port 2
#define TEMPERATURE_PRECISION 9

// Setup a OneWire instance and pass to DallasTemperature
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Arrays to hold device addresses
DeviceAddress waterHigh, waterLow, airAmbient;

// Update addresses below to valid device addresses on your bus
DeviceAddress waterHigh  = { 0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF0 };
DeviceAddress waterLow   = { 0x28, 0x3F, 0x1C, 0x31, 0x2, 0x0, 0x0, 0x2  };
DeviceAddress airAmbient = { 0x28, 0x3F, 0x1C, 0x31, 0x2, 0x0, 0x0, 0x2  };
DeviceAddress devices[] = {
  waterHigh,
  waterLow,
  airAmbient
}

const int deviceCount = 3;
const int timeDelay = 1000;

float tempC = 0;
float tempF = 0;

/*
 * Setup - Initialize the sensors here
 */
void setup(void)
{
  // Start serial port and library
  Serial.begin(9600);
  Serial.println("O HAI");
  sensors.begin();

  // Get all devices on the bus
  Serial.print("Locating devices... ");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // Get parasite power requirements
  Serial.print("Parasite power: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  // Get the address for all devices
  oneWire.reset_search();
  if (!oneWire.search(waterHigh))  Serial.println("Address for waterHigh  not found");
  if (!oneWire.search(waterLow))   Serial.println("Address for waterLow   not found");
  if (!oneWire.search(airAmbient)) Serial.println("Address for airAmbient not found");

  // Show the addresses found on the bus
  Serial.print("waterHigh  Device Address: ");
  printAddress(waterHigh);
  Serial.println();
  Serial.print("waterLow   Device Address: ");
  printAddress(waterLow);
  Serial.println();
  Serial.print("airAmbient Device Address: ");
  printAddress(waterLow);
  Serial.println();

  // Show device resolutions - BEFORE
  Serial.print("waterHigh  Device Resolution: ");
  Serial.print(sensors.getResolution(waterHigh), DEC);
  Serial.println();

  Serial.print("waterLow   Device Resolution: ");
  Serial.print(sensors.getResolution(waterLow), DEC);
  Serial.println();

  Serial.print("airAmbient Device Resolution: ");
  Serial.print(sensors.getResolution(airAmbient), DEC);
  Serial.println();
  
  // Set the device resolutions to 9 bits
  sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
  sensors.setResolution(outsideThermometer, TEMPERATURE_PRECISION);

  // Show device resolutions - AFTER
  Serial.print("waterHigh  Device Resolution: ");
  Serial.print(sensors.getResolution(waterHigh), DEC);
  Serial.println();

  Serial.print("waterLow   Device Resolution: ");
  Serial.print(sensors.getResolution(waterLow), DEC);
  Serial.println();

  Serial.print("airAmbient Device Resolution: ");
  Serial.print(sensors.getResolution(airAmbient), DEC);
  Serial.println();
}

// Function to show device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // Pad the address with zeroes if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// Function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print(tempC);
  Serial.print(" C / ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
  Serial.print(" F");
}

// Function to show device resolution
void printResolution(DeviceAddress deviceAddress)
{
  printAddress(deviceAddress);
  Serial.print(" Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();
}

// Function to show device info
void printData(DeviceAddress deviceAddress, )
{
  Serial.print("Device Address: ");
  printAddress(deviceAddress);
  Serial.print(" - ");
  printTemperature(deviceAddress);
  Serial.println();
}

/*
 * Main - Get and show the temperature
 */
void loop(void)
{ 
  // Request temperatures from all bus devices
  Serial.print("Requesting temperatures....");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println(" Done.");

  delay(timeDelay);

  // Show the device info
  printData(waterHigh);
  printData(waterLow);
  printData(airAmbient);

  // Show the device info
  for (int index = 0; index < deviceCount; index++) {
    printData(device[index]);
  }
}
