#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>

// Setup variables
const char degreeSymbol[] = "\xC2\xB0";
const int sensorCount = 3;
int column = 0;
int row = 0;
float temps[sensorCount] = {0};
float tempStart = 0;
float tempAverage = 0;
float tempDelta = 0;
bool isLedOn = false;
unsigned long updateTimeLast = 0; // Timestamp of last update
const int updateDelay = 1000;     // Time in ms between updates
unsigned long elapsedMillis = 0;

// Setup LCD, OneWire, DallasTemperature, sensor objects
LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire oneWire(2);
DallasTemperature dt(&oneWire);
DeviceAddress sensors[sensorCount] = {{}};

// Function to get device address
String getAddress(DeviceAddress sensor)
{
  String address = "";

  for (uint8_t i = 0; i < 8; i++)
  {
    if (sensor[i] < 16) address += "0";
    address += String(sensor[i], HEX);
  }

  return address;
}

// Setup - Initialize sensors here
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
    0b00110,
    0b01001,
    0b01001,
    0b00110,
    0b00000,
    0b00000,
    0b00000,
    0b00000
  };
  lcd.createChar(0, charDegree);
  byte charArrow[8] = {
    0b00000,
    0b00100,
    0b00010,
    0b11111,
    0b00010,
    0b00100,
    0b00000,
    0b00000
  };
  lcd.createChar(1, charArrow);

  // Get all devices on the bus
  Serial.print(F("Locating devices.... Found "));
  Serial.println(dt.getDeviceCount(), DEC);

  // Get sensor addresses
  oneWire.reset_search();
  for (int i = 0; i < sensorCount; i++)
  {
    Serial.print(F("i: "));
    Serial.print(i);

    if (oneWire.search(sensors[i]))
    {
      Serial.print(F(" | address: "));
      Serial.print(getAddress(sensors[i]));
      Serial.println();
    }
    else Serial.println(F(" | *** address not found ***"));
  }
}

// Main - ALL THE THINGS!
void loop(void)
{
  // Update each second (without sleep delays)
  if ((millis() - updateTimeLast) > updateDelay)
  {
    updateTimeLast = millis();

    // Toggle LED each update
    isLedOn = !isLedOn;
    digitalWrite(LED_BUILTIN, isLedOn ? HIGH : LOW);

    // Get elapsed time
    elapsedMillis = millis();
    unsigned long durSS = (elapsedMillis / 1000) % 60;  //Seconds
    unsigned long durMM = (elapsedMillis / (60000));    //Minutes

    // Get temperature average from first two sensors (water)
    dt.requestTemperatures();
    tempAverage = 0;
    for (int i = 0; i < sensorCount; i++)
    {
      temps[i] = dt.getTempF(sensors[i]);
      if (i < 2) tempAverage += temps[i]; // Only average first two (water)
    }
    tempAverage /= 2;

    // First time, set CSV header row & starting temperature
    if (tempStart == 0)
    {
      Serial.println(F("Time,Sensor1,Sensor2,Sensor3,Start,Current,Delta"));
      tempStart = tempAverage;
    }
    tempDelta = tempAverage - tempStart;

    // CSV data rows
    if (durMM < 10) Serial.print(F("0"));
    Serial.print(durMM);
    Serial.print(":");
    if (durSS < 10) Serial.print(F("0"));
    Serial.print(durSS);
    Serial.print(F(","));
    for (int i = 0; i < sensorCount; i++)
    {
      Serial.print(temps[i], 1);
      Serial.print(F(","));
    }
    Serial.print(tempStart, 1);
    Serial.print(F(","));
    Serial.print(tempAverage, 1);
    Serial.print(F(","));
    Serial.println(tempDelta, 1);

    // Display temperatures on 16x2 display
    //  Row 0: 51.9  52.3  80.5
    //  Row 1: 52.1  2.1° 01:23
    lcd.clear();

    // Row 0
    // Sensor1 (water)
    row = 0;
    column = 0;
    lcd.setCursor(column, row);
    lcd.print(temps[0], 1);
    lcd.print(" ");
    // Sensor2 (water)
    column = 5;
    if (temps[1] < 100) column++;
    lcd.setCursor(column, row);
    lcd.print(temps[1], 1);
    // Sensor3 (air)
    column = 11;
    if (temps[2] < 100) column++;
    lcd.setCursor(column, row);
    lcd.print(temps[2], 1);

    // Row 1
    // Current water temperature (average of first two)
    row = 1;
    column = 0;
    if (tempAverage >= 0 && tempAverage < 10) column++;
    lcd.setCursor(column, row);
    lcd.print(tempAverage, 1);
    // Temperature change (current - starting)
    column = 5;
    if (tempDelta >= 0 && tempDelta < 10) column++;
    lcd.setCursor(column, row);
    lcd.print(tempDelta, 1);
    lcd.print((char)0);
    // Time elapsed (MM:SS)
    column = 11;
    lcd.setCursor(column, row);
    if (durMM < 10) lcd.print("0");
    lcd.print(durMM);
    lcd.print(":");
    if (durSS < 10) lcd.print("0");
    lcd.print(durSS);
  }
}
