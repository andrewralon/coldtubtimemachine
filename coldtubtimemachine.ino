#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>

#define GETNAMEOF(name) getnameof(#name, (name))
#define ONE_WIRE_BUS 2			// Data wire is plugged into Arduino port 2

// Setup LCD, OneWire, DallasTemperature, and sensor objects
LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dt(&oneWire);
DeviceAddress s1, s2, s3;
DeviceAddress sensors[] = {{s1, s2, s3}};

// Setup variables
float tempsF[3] = { 0.0 };
bool led = false;
const int sensorCount = 2;
const int timeDelay = 1000;
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
void printAddress(DeviceAddress sensor)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (sensor[i] < 16)
    {
      Serial.print("0"); // Pad segment if less than one char wide
    }
    Serial.print(sensor[i], HEX);
  }
}

// Function to print the temperature for a device
float getTemperature(DeviceAddress sensor)
{
  return dt.getTempF(sensor);
}

// Setup - Initialize the sensors here
void setup(void)
{
  // Start serial port, on-board LED, sensors, and LCD
  Serial.begin(9600);
  Serial.println("O HAI");
  pinMode(LED_BUILTIN, OUTPUT);
  dt.begin();
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, degreeSymbol);

  // Get all devices on the bus
  Serial.print("Locating devices.... Found ");
  Serial.println(dt.getDeviceCount(), DEC);

  // Get device addresses and resolutions
  oneWire.reset_search();
  for (int i = 0; i < sensorCount; i++)
  {
    Serial.print("i: ");
    Serial.print(i);

    if (oneWire.search(sensors[i]))
    {
      Serial.print(" | address: ");
      printAddress(sensors[i]);
      Serial.print(" | resolution: ");
      Serial.println(dt.getResolution(sensors[i]), DEC);
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
  // Get temperatures
  dt.requestTemperatures();
  for (int i = 0; i < sensorCount; i++)
  {
    tempsF[i] = dt.getTempF(sensors[i]);
  }

  // Display temperatures on two rows of a 16x2 display
  lcd.clear();
  for (int i = 0; i < 2; i++)
  {
    lcd.setCursor(0, i);
    Serial.print(i);
    lcd.print(i);
    Serial.print(": ");
    lcd.print(": ");

    if (tempsF[i] > -10 && tempsF[i] < 10)
    {
      Serial.print(" ");
      lcd.print(" ");
    }

    Serial.print(tempsF[i]);
    lcd.print(tempsF[i], 1); // Round to one decimal place; 76.17 -> 76.2
    Serial.println(" F"); // Replace with degree symbol
    lcd.print((char)0);
    lcd.print("F");
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
