#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>

// Setup variables
const char degreeSymbol[] = "\xC2\xB0";
const int sensorCount = 2;
const int tempSamples = 10;
int tempsSampledStart = 0;
int tempsSampledEnd = 0;
float temps[sensorCount] = {0};
float tempsStart[tempSamples] = {0};
float tempsEnd[tempSamples] = {0};
bool isLedOn = false;
unsigned long updateTimeLast = 0; // Timestamp of last update
const int updateDelay = 1000; // Time in ms between updates

// Setup LCD, OneWire, DallasTemperature, sensor objects
LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire oneWire(2);
DallasTemperature dt(&oneWire);
DeviceAddress sensors[sensorCount] = {{}};

// Function to show device address
void printAddress(DeviceAddress sensor)
{
  // char address[16] = {""}; // Example: 28BE4D56B5013C8E
  for (uint8_t i = 0; i < 8; i++)
  {
    if (sensor[i] < 16) // Pad segment if less than one char wide
    {
      Serial.print(F("0"));
    }
    Serial.print(sensor[i], HEX);
  }
}

// Setup - Initialize the sensors here
void setup(void)
{
  // Start serial port, on-board LED, sensors, and LCD
  Serial.begin(9600);
  Serial.println(F("O HAI"));
  pinMode(LED_BUILTIN, OUTPUT);
  dt.begin();
  lcd.init();
  lcd.backlight();

  // Create the degree symbol in the LCD display
  byte degreeSymbolLcd[8] = {
    0b00110,
    0b01001,
    0b01001,
    0b00110,
    0b00000,
    0b00000,
    0b00000,
    0b00000
  };
  lcd.createChar(0, degreeSymbolLcd);

  // Get all devices on the bus
  Serial.print(F("Locating devices.... Found "));
  Serial.println(dt.getDeviceCount(), DEC);

  // Get device addresses and resolutions
  oneWire.reset_search();
  for (int i = 0; i < sensorCount; i++)
  {
    Serial.print(F("i: "));
    Serial.print(i);

    if (oneWire.search(sensors[i]))
    {
      Serial.print(F(" | address: "));
      printAddress(sensors[i]);
      // Serial.print(F(getAddress(sensors[i])));
      Serial.print(F(" | resolution: "));
      Serial.println(dt.getResolution(sensors[i]), DEC);
    }
    else
    {
      Serial.println(F(" | *** Address not found ***"));
    }
  }
}

// Main - Get and show temperatures
void loop(void)
{
  // Update timer without sleep delays
  if ((millis() - updateTimeLast) > updateDelay)
  {
    updateTimeLast = millis();

    // Toggle LED on and off
    isLedOn = !isLedOn;
    digitalWrite(LED_BUILTIN, isLedOn ? HIGH : LOW);

    // Get temperatures
    dt.requestTemperatures();
    float average = 0;
    for (int i = 0; i < sensorCount; i++)
    {
      temps[i] = dt.getTempF(sensors[i]);
      average += temps[i];
    }
    average /= sensorCount;

    if (tempsSampledStart < 10)
    {
      tempsStart[tempsSampledStart++] = average;
    }

    // Display temperatures on two rows of a 16x2 display
    lcd.clear();
    for (int i = 0; i < 2; i++)
    {
      lcd.setCursor(0, i);
      Serial.print(i);
      lcd.print(i);
      Serial.print(F(": "));
      lcd.print(": ");

      if (temps[i] > -10 && temps[i] < 10)
      {
        Serial.print(F(" ")); // Pad temperature spacing
        lcd.print(" ");
      }

      // Round to one decimal place; 76.17 -> 76.2
      Serial.print(temps[i], 1);
      lcd.print(temps[i], 1);
      Serial.print(degreeSymbol);
      lcd.print((char)0); // Degree symbol
      Serial.println(F("F"));
      lcd.print("F");
    }

    lcd.setCursor(10, 0); // Column 10, Row 0 
    Serial.print(F("Average: "));
    Serial.print(average, 1);
    lcd.print(average, 1);
    Serial.print(degreeSymbol);
    lcd.print((char)0);
    Serial.println(F("F"));
    lcd.print("F");
  }
}
