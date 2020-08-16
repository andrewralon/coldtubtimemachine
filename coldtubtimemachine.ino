#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
// #include <LiquidCrystal.h>
// LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define GETNAMEOF(name) getnameof(#name, (name))
#define ONE_WIRE_BUS 2			// Data wire is plugged into Arduino port 2

// Setup a OneWire instance, pass it to DallasTemperature, and create sensor objects
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dt(&oneWire);
DeviceAddress s1, s2, s3;
DeviceAddress sensors[] = { { s1, s2, s3 } };

const int sensorCount = 2;
const int timeDelay = 1000;
float tempC = 0;
float tempF = 0;
bool led = false;
const byte degreeSymbol[8] = {
  0b00110,
  0b01001,
  0b01001,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

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
    // Pad address segment with zero if less than one char
    if (deviceAddress[i] < 16)
    {
      Serial.print("0");
    }
    Serial.print(deviceAddress[i], HEX);
  }
}

// Function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  // float tempC = dt.getTempC(deviceAddress);
  // float tempF = DallasTemperature::toFahrenheit(tempC);
  float tempF = dt.getTempF(deviceAddress); // * 100.0) / 100.0;
  
  // lcd.home();
  // lcd.setCursor(0, 0);

  //  if (tempC > -10 && tempC < 10)
  //  {
  //    // Serial.print(" ");
  //    // lcd.print(" ");
  //  }
  //  Serial.print(tempC);
  //  lcd.print(tempC);
  //  Serial.print(" C / ");
  //  lcd.print(" C");
  //  lcd.setCursor(0, 1);

  if (tempF > -10 && tempF < 10)
  {
    Serial.print(" ");
    lcd.print(" ");
  }
  Serial.print(tempF);
  lcd.print(tempF, 1);
  // Serial.print((char)degreeSymbol);
  lcd.print((char)0);
  Serial.print("*F");
  lcd.print("F");
}

// Setup - Initialize the sensors here
void setup(void)
{
  // Start serial port, on-board LED, sensors, and LCD
  Serial.begin(9600);
  Serial.println("O HAI");
  pinMode(LED_BUILTIN, OUTPUT);
  dt.begin();
  lcd.init();     //initialize the lcd
  lcd.backlight(); //open the backlight
  lcd.createChar(0, degreeSymbol);
  // lcd.begin(16, 2);

  // Get all devices on the bus
  Serial.print("Locating devices.... Found ");
  Serial.println(dt.getDeviceCount(), DEC);

  // Get parasite power requirements
  Serial.print("Parasite power: ");
  if (dt.isParasitePowerMode())
    Serial.println("ON");
  else
    Serial.println("OFF");

  // Get the addresses for all devices
  oneWire.reset_search();

  for (int index = 0; index < sensorCount; index++)
  {
    Serial.print("index: ");
    Serial.print(index);

    // Serial.print(" | name: ");
    // GETNAMEOF(sensors[index]);
    // Serial.print();

    if (oneWire.search(sensors[index]))
    {
      // Show the address found on the bus
      Serial.print(" | address: ");
      printAddress(sensors[index]);
      // printAddress(s1);
      // Serial.print();

      // Show device resolution
      Serial.print(" | resolution: ");
      Serial.println(dt.getResolution(sensors[index]), DEC);
    }
    else
    {
      Serial.println("*** Address not found ***");
    }
  }
}

// Main - Get and show temperatures
void loop(void)
{
  // Request and show temperatures from all bus devices
  dt.requestTemperatures();
  lcd.clear();
  for (int index = 0; index < sensorCount; index++)
  {
    lcd.setCursor(0, index);
    // GETNAMEOF(sensors[index]);
    Serial.print(index);
    lcd.print(index);
    Serial.print(": ");
    lcd.print(": ");
    printTemperature(sensors[index]);
    Serial.println();
    // delay(50);
  }

  if (led == true)
  {
    led = false;
    digitalWrite(LED_BUILTIN, LOW);
  }
  else
  {
    led = true;
    digitalWrite(LED_BUILTIN, HIGH);
  }

  delay(timeDelay);
}
