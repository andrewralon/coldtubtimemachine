#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>

// Setup variables
const char degreeSymbol[] = "\xC2\xB0";
const int sensorCount = 2;
const int tempSamples = 10;
int column = 0;
int row = 0;
float temps[sensorCount] = {0};
float tempStart = 0;
float tempAverage = 0;
float tempDelta = 0;
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
  // Start serial port, on-board LED, sensors, LCD
  Serial.begin(9600);
  Serial.println(F("O HAI"));
  pinMode(LED_BUILTIN, OUTPUT);
  dt.begin();
  lcd.init();
  lcd.backlight();

  // Create degree and arrow symbols in the LCD display
  byte charDegree[8] = {
    0b00110, 0b01001, 0b01001, 0b00110,
    0b00000, 0b00000, 0b00000, 0b00000
  };
  lcd.createChar(0, charDegree);
  byte charArrow[8] = {
    0b00000, 0b00100, 0b00010, 0b11111,
    0b00010, 0b00100, 0b00000, 0b00000
  };
  lcd.createChar(1, charArrow);

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
      Serial.print(F(" | resolution: "));
      Serial.println(dt.getResolution(sensors[i]), DEC);
    }
    else
    {
      Serial.println(F(" | *** address not found ***"));
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

    isLedOn = !isLedOn; // Toggle LED on and off
    digitalWrite(LED_BUILTIN, isLedOn ? HIGH : LOW);

    // Get temperatures
    dt.requestTemperatures();
    tempAverage = 0;
    for (int i = 0; i < sensorCount; i++)
    {
      temps[i] = dt.getTempF(sensors[i]);
      tempAverage += temps[i];
    }
    tempAverage /= sensorCount;

    for (int i = 0; i < sensorCount; i++)
    {
      Serial.print(i);
      Serial.print(F(": "));
      if (temps[i] >= 0 && temps[i] < 10)
      {
        Serial.print(F(" ")); // Pad temperature spacing
      }
      Serial.print(temps[i], 1);
      Serial.print(degreeSymbol);
      Serial.println(F("F"));
    }

    if (tempStart == 0)
    {
      tempStart = tempAverage;
    }
    tempDelta = tempAverage - tempStart;

    // Display temperatures on two rows of a 16x2 display
    lcd.clear();
    row = 0;
    column = 1;
    lcd.setCursor(column, row);
    for (int i = 0; i < sensorCount; i++)
    {
      lcd.print(i);
      lcd.print(":");
      if (temps[i] >= 0 && temps[i] < 10)
      {
        column++; // Pad with a space
      }
      lcd.print(temps[i], 1);
      lcd.print(" ");
    }

    Serial.print(F("Start:   "));
    row = 1;
    column = 0;
    if (tempStart >= 0 && tempStart < 10)
    {
      column++; // Pad with a space
      Serial.print(F(" "));
    }
    Serial.print(tempStart, 1);
    Serial.print(degreeSymbol);
    Serial.println(F("F"));
    lcd.setCursor(column, row);
    lcd.print(tempStart, 1);
    lcd.setCursor(4, row);
    lcd.print((char)1);

    Serial.print(F("Average: "));
    column = 5;
    if (tempAverage >= 0 && tempAverage < 10)
    {
      column++; // Pad with a space
      Serial.print(F(" "));
    }
    Serial.print(tempAverage, 1);
    Serial.print(degreeSymbol);
    Serial.println(F("F"));
    lcd.setCursor(column, row);
    lcd.print(tempAverage, 1);
    lcd.print(":");

    Serial.print(F("Delta:   "));
    column = 10;
    if (tempDelta >= 0 && tempDelta < 10)
    {
      column++; // Pad with a space
      Serial.print(F(" "));
    }
    Serial.print(tempDelta, 1);
    Serial.print(degreeSymbol);
    Serial.println(F("F"));
    lcd.setCursor(column, row);
    lcd.print(tempDelta, 1);
    lcd.print((char)0);
    lcd.print("F");

    column = 0;
    lcd.setCursor(column, row);


  }
}
