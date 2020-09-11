#include <NOKIA5110_TEXT.h>
#include <NOKIA5110_TEXT_FONT.h>
#include <NOKIA5110_TEXT_FONT_EIGHT.h>
#include <NOKIA5110_TEXT_FONT_FIVE.h>
#include <NOKIA5110_TEXT_FONT_FOUR.h>
#include <NOKIA5110_TEXT_FONT_NINE.h>
#include <NOKIA5110_TEXT_FONT_SEVEN.h>
#include <NOKIA5110_TEXT_FONT_SIX.h>
#include <NOKIA5110_TEXT_FONT_THREE.h>
#include <NOKIA5110_TEXT_FONT_TWO.h>

//#include <Adafruit_PCD8544.h>
//#include <Adafruit_GFX.h>      // include adafruit graphics library
#include <DallasTemperature.h>
//#include <Nokia_LCD.h>
#include <OneWire.h>

#define WIDTH 5
#define inverse  false
#define contrast 0xB4 // default is 0xBF set in LCDinit, Try 0xB1 - 0xBF, if your display is too dark
#define bias 0x12 // LCD bias mode 1:48: Try 0x12 or 0x13 or 0x14 usually
#define FontNumber 1 //1-9, 1 is default ,  Change define in NOKIA5110_TEXT.h if using non default

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
const char degreeSymbol[] = "\xC2\xB0";
const int sensorCount = 3;
int row = 0;
int column = 0;
int character = 0;
float temps[sensorCount] = {0};
float tempStart = 0;
float tempAverage = 0;
float tempDelta = 0;
bool isLedOn = false;
unsigned long updateTimeLast = 0; // Timestamp of last update
const int updateDelay = 1000;     // Time in ms between updates
unsigned long elapsedMillis = 0;

#define RST 9   // Reset pin
#define CE 10   // Chip enable
#define DC 11   // Data or command
#define DIN 12  // Serial Data input
#define CLK 13  // Serial clock

// Setup LCD, OneWire, DallasTemperature, sensor objects
// DISPLAY    ARDUINO
// 1 RST   -> 9
// 2 CE    -> 10
// 3 DC    -> 11
// 4 DIN   -> 12
// 5 CLK   -> 13
// 6 VCC   -> 3.3V (optional! backlight is brighter with it)
// 7 LIGHT -> 8
// 8 GND   -> GND
// Parameters: CLK, DIN, DC, CE, RST, (LIGHT is optional)
//Nokia_LCD lcd(13, 12, 11, 10, 9, 8);
OneWire oneWire(2);
DallasTemperature dt(&oneWire);
DeviceAddress sensors[sensorCount] = {{}};

// Nokia 5110 LCD module connections (CLK, DIN, D/C, CS, RST)
//Adafruit_PCD8544 lcd = Adafruit_PCD8544(13, 12, 11, 10, 9);

// RST 1/ CD 2/ DC 3/ DIN 4/ CLK 5
NOKIA5110_TEXT lcd(RST, CE, DC, DIN, CLK);
//NOKIA5110_TEXT lcd(9, 10, 11, 12, 13);

//const unsigned char degree[8] = {0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00};
const unsigned char degree[7] = {0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00};



//void defChar (Adafruit_PCD8544 &lcd, int asc, const unsigned char row[8]) {
//  int i;
//  if ((asc < 0) || (asc > 7)) return;
//  lcd.command (0x40 | (asc << 3));
//  for (i = 0; i < 8; i++) lcd.write(row[i]);
//  //lcd.home();
//}

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

  // Create degree and arrow symbols in the LCD display
  
  //lcd.createChar(0, charDegree);
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
  //lcd.createChar(0, degree);

  // Start serial port, on-board LED, sensors, LCD
  // NOTE: Backlight logic is reversed! true = OFF, false = ON

  pinMode(LED_BUILTIN, OUTPUT);
  dt.begin();

  //lcd.begin();
  //lcd.clearDisplay();
  //lcd.setBacklight(true);
  //lcd.setContrast(50);
  //lcd.setTextSize(1);
  //defChar(lcd, 0x00, degree);
  //defChar(lcd, 0x00, arrow);

  lcd.LCDInit(inverse, contrast, bias); // init the lCD
  lcd.LCDClear(0x00); //clear whole screen
  lcd.LCDFont(FontNumber); // Set the font

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
  if ((millis() - updateTimeLast) > updateDelay) {
    updateTimeLast = millis();

    // Get elapsed time
    elapsedMillis = millis();
    unsigned long durSS = (elapsedMillis / 1000) % 60;  //Seconds
    unsigned long durMM = (elapsedMillis / (60000));    //Minutes

    // Get temperature average from first two sensors (water)
    dt.requestTemperatures();
    tempAverage = 0;
    for (int i = 0; i < sensorCount; i++) {
      temps[i] = dt.getTempF(sensors[i]);
      if (i < 2) tempAverage += temps[i]; // Only average first two (water)
    }
    tempAverage /= 2;

    // First time, set CSV header row & starting temperature
    if (tempStart == 0) {
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
    for (int i = 0; i < sensorCount; i++) {
      Serial.print(temps[i], 1);
      Serial.print(F(","));
    }
    Serial.print(tempStart, 1);
    Serial.print(F(","));
    Serial.print(tempAverage, 1);
    Serial.print(F(","));
    Serial.println(tempDelta, 1);

    // Display temperatures on 14x6 display
    // Row 12345678901234
    //  0:  51.9°  52.3°
    //  1: W  52.1°
    //  2: W0 50.0°
    //  3: Change   2.1°
    //  4: Air     80.5°
    //  5: Time 101:23
    // Row 12345678901234
    // Row 12345678901234
    //  0:  51.9°  52.3°
    //  1: Water   52.1°
    //  2:  52.1 - 50.0=2.1°
    //  3: Change   2.1°
    //  4: Air     80.5°
    //  5: Time 101:23
    // Row 12345678901234
    // Row 12345678901234
    //  0: 101:23    2.1°
    //  1: Calories   123
    //  2: Water    52.1°
    //  3: Air      80.5°
    //  4:
    //  5:  51.9°  52.3°
    // Row 12345678901234
    //lcd.clear();

    //lcd.clearDisplay();
    //lcd.setCursor(0, 0);
    //lcd.display();

    lcd.LCDClear(0x00);
    lcd.LCDgotoXY(0, 0);
    lcd.LCDString("12345678901234");
    lcd.LCDgotoXY(0, 1);
    lcd.LCDString("1.23");
    lcd.LCDCustomChar(degree, sizeof(degree) / sizeof(unsigned char), 0x00, false);
    lcd.LCDString("F");

    //lcd.drawCircle(2, 2, 2, BLACK);
    //lcd.display();
    //lcd.println("12345678901234");
    //lcd.write("1.23");
    //lcd.write((char)0x00);
    //lcd.drawCircle(2, 2, 2, BLACK);
    //lcd.println("F");


    // Row 0 - Water sensors
    // "101:23    2.1°"
    character = 0;
    row = 1;
    column = ((character * WIDTH));
    //lcd.setCursor(column, row);
    //if (durMM < 100) lcd.print(" ");
    //if (durMM < 10) lcd.print(" ");
    //lcd.print(durMM);
    //lcd.print(":");
    //if (durSS < 10) lcd.print("0");
    //lcd.print(durSS);

    character = 9;
    column = ((character * WIDTH) - 1);
    //lcd.setCursor(column, row);
    //if (tempDelta >= 0 && tempDelta < 10) lcd.print(" ");
    //lcd.print(tempDelta, 1);
    //lcd.println((char)223);


    //lcd.print(temps[0], 1);
    // Sensor2 (water)
    character = 6;
    column = ((character * WIDTH) - 1);
    //lcd.setCursor(column, row);
    //lcd.print(temps[1], 1);

    // Row 1
    // Sensor3 (air)
    character = 11;
    row = 1;
    column = ((character * WIDTH) - 1);
    if (temps[2] < 100) character++;
    column = ((character * WIDTH) - 1);
    //lcd.setCursor(column, row);
    //lcd.print(temps[2], 1);

    // Row 1
    // Current water temperature (average of first two)
    row = 1;
    column = 0;
    if (tempAverage >= 0 && tempAverage < 10) column++;
    //lcd.setCursor(column, row);
    //lcd.print(tempAverage, 1);
    // Temperature change (current - starting)
    column = 5;
    if (tempDelta >= 0 && tempDelta < 10) column++;
    //lcd.setCursor(column, row);
    //lcd.print(tempDelta, 1);
    //lcd.print((char)0);
    // Time elapsed (MMM:SS)
    column = 10;
    //lcd.setCursor(column, row);
    // if (durMM < 100) lcd.print(" ");
    // if (durMM < 10) lcd.print(" ");
    //lcd.print(durMM);
    //lcd.print(":");
    // if (durSS < 10) lcd.print("0");
    //lcd.print(durSS);

    //lcd.display();
  }
}
