#include <PCD8544.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define DEGREE 0
#define inverse false
#define contrast 0xB4 // default is 0xBF set in LCDinit, Try 0xB1 - 0xBF, if your display is too dark
#define bias 0x12 // LCD bias mode 1:48: Try 0x12 or 0x13 or 0x14 usually
#define FontNumber 1 //1-9, 1 is default ,  Change define in NOKIA5110_TEXT.h if using non default

static const byte degree[] = {0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00};
static const byte arrow[] = {0x00, 0x04, 0x02, 0x31, 0x02, 0x04, 0x00, 0x00};
const unsigned char logo[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x1C, 0x1C, 0x3C, 0xFC, 0xFC, 0xFC,
  0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC,
  0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC,
  0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x01, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
  0x07, 0x07, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x07,
  0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x27, 0x27, 0xE7, 0xE7,
  0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE6, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xC0, 0xC0,
  0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x3F, 0x1F, 0x0F, 0x0F, 0x0F, 0x07, 0x01, 0x01, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07,
  0x07, 0x0F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8, 0xF0, 0xF0, 0xE0,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x0F, 0x1F, 0x1F, 0x3F, 0x3F,
  0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3C, 0x38, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// Setup variables
const int sensorCount = 3;
int row = 0;
int column = 0;
int character = 0;
int calories = 0;
float temps[sensorCount] = {0};
float tempStart = 0;
float tempAverage = 0;
float tempDelta = 0;
bool isLedOn = false;
unsigned long elapsedMillis = 0;  // Timestamp of last update
bool backlightOn = true;
bool buttonState = false;
unsigned long backlightStartTime = 0;
#define BACKLIGHT_DURATION 1200000 // Time in ms to keep backlight on (20 min)
#define BACKLIGHT_BUTTON 9
#define UPDATE_DELAY 1000 // Time in ms between updates
#define WARMUP_DELAY 2000 // Time in ms before starting timer and temperature logging

OneWire oneWire(2);
DallasTemperature dt(&oneWire);
DeviceAddress sensors[sensorCount] = {{}};

#define LIGHT 8 // Backlight
#define RST 6   // Reset pin
#define CE 7    // Chip enable
#define DC 5    // Data or command
#define DIN 4   // Serial Data input
#define CLK 3   // Serial clock
static PCD8544 lcd;

// Function to get device address
String getAddress(DeviceAddress sensor) {
  String address = "";
  for (uint8_t i = 0; i < 8; i++) {
    if (sensor[i] < 16) address += "0";
    address += String(sensor[i], HEX);
  }
  return address;
}

// Setup - Initialize sensors here
void setup(void) {
  Serial.begin(9600);
  Serial.println(F("O HAI"));

  pinMode(BACKLIGHT_BUTTON, INPUT);
  pinMode(LIGHT, OUTPUT);

  // Start temp sensors and LCD
  // NOTE: Backlight logic is reversed! true = OFF, false = ON
  dt.begin();
  lcd.begin(84, 48);
  lcd.setContrast(50);
  lcd.createChar(0, degree);

  // Get all devices on the bus
  Serial.print(F("Locating devices.... Found "));
  Serial.println(dt.getDeviceCount(), DEC);

  // Get sensor addresses
  oneWire.reset_search();
  for (int i = 0; i < sensorCount; i++) {
    Serial.print(F("i: "));
    Serial.print(i);

    if (oneWire.search(sensors[i])) {
      Serial.print(F(" | address: "));
      Serial.print(getAddress(sensors[i]));
      Serial.println();
    }
    else Serial.println(F(" | *** address not found ***"));
  }
}

// Main - ALL THE THINGS!
void loop(void) {
  // Update each second (without sleep delays)
  if ((millis() - elapsedMillis) > UPDATE_DELAY) {
    // Backlight logic
    buttonState = digitalRead(BACKLIGHT_BUTTON);
    if (buttonState) {
      if (backlightOn) {
        backlightStartTime = millis();
      }
      else {
        backlightStartTime = 0;
      }
      backlightOn = !backlightOn;
    }
    if (!backlightOn &&
        millis() - backlightStartTime >= BACKLIGHT_DURATION) {
      backlightOn = true;
    }
    digitalWrite(LIGHT, backlightOn);

    // Get elapsed time
    unsigned long durSS = 0;
    unsigned long durMM = 0;
    int currentMillis = (int)millis() - WARMUP_DELAY;
    if (currentMillis > 0) {
      elapsedMillis = millis() - WARMUP_DELAY;
      durSS = (elapsedMillis / 1000) % 60;  // Seconds
      durMM = (elapsedMillis / (60000));    // Minutes
    }

    // Get temperature average from first two sensors (water)
    dt.requestTemperatures();
    tempAverage = 0;
    for (int i = 0; i < sensorCount; i++) {
      temps[i] = dt.getTempF(sensors[i]);
      if (i < 2) tempAverage += temps[i]; // Only average first two (water)
    }
    tempAverage /= 2;

    // First time, set CSV header row & starting temperature
    if (currentMillis > 0) {
      if (tempStart == 0) {
        Serial.println(F("Time,Sensor1,Sensor2,Sensor3,Start,Current,Delta"));
        tempStart = tempAverage;
      }
      tempDelta = tempAverage - tempStart;
    }

    // CSV data rows
    if (durMM < 10) Serial.print(F("0"));
    Serial.print(durMM);
    Serial.print(F(":"));
    if (durSS < 10) Serial.print(F("0"));
    Serial.print(durSS);
    Serial.print(F(","));
    for (int i = 0; i < sensorCount; i++) {
      Serial.print(temps[i], 1);
      Serial.print(F(","));
    }
    Serial.print(tempStart, 1);
    Serial.print(F(","));
    Serial.print(tempAverage, 1);
    Serial.print(F(","));
    Serial.println(tempDelta, 1);

    // Display information on 14x6 display
    int row = 0;
    lcd.clear();

    // Row 12345678901234
    //  0: Time    101:23
    //  1: Change 12.34°F
    //  2: Calories   123
    //  3: Water  52.13°F
    //  4:  51.90  52.35
    //  5: Air   101.52°F
    // Row 12345678901234

    // Row 0 - "Time    101:23"
    lcd.setCursor(0, row++);
    lcd.print("Time    ");
    if (durMM < 100) lcd.print(" ");
    if (durMM < 10) lcd.print(" ");
    lcd.print(durMM);
    lcd.print(":");
    if (durSS < 10) lcd.print("0");
    lcd.print(durSS);

    // Row 1 - "Change 12.34°F"
    lcd.setCursor(0, row++);
    lcd.print("Change ");
    if (tempDelta < 10 && tempDelta >= 0) lcd.print(" ");
    lcd.print(tempDelta, 2);
    lcd.write((char)DEGREE);
    lcd.print("F");

    // Row 2 - "Calories   123"
    calories = 123; // DEBUG ONLY
    lcd.setCursor(0, row++);
    lcd.print("Calories   ");
    if (calories < 100) lcd.print(" ");
    if (calories < 10) lcd.print(" ");
    lcd.print(calories);

    // Row 4 - "Water  52.13°F"
    lcd.setCursor(0, row++);
    lcd.print("Water ");
    if (tempAverage < 100) lcd.print(" ");
    if (tempAverage >= 0 && tempAverage < 10) lcd.print(" ");
    lcd.print(tempAverage, 2);
    lcd.write((char)DEGREE);
    lcd.print("F");

    // Row 4 - " 51.90  52.35 "
    lcd.setCursor(0, row++);
    for (int i = 0; i < 2; i++) {
      if (temps[i] < 100) lcd.print(" ");
      if (temps[i] < 10 && temps[i] >= 0) lcd.print(" ");
      lcd.print(temps[i], 2);
      if (i == 0) lcd.print(" ");
    }

    // Row 5 - "Air   101.52°F"
    lcd.setCursor(0, row++);
    lcd.print("Air   ");
    if (temps[2] < 100) lcd.print(" ");
    if (temps[2] < 10 && temps[2] >= 0) lcd.print(" ");
    lcd.print(temps[2], 2);
    lcd.write((char)DEGREE);
    lcd.print("F");
  }
}
