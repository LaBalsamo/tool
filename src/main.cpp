#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "wifi_scan.h"  //Wi-Fi scan functions
#include <DHT.h>

// Pins
#define TFT_CS   25
#define TFT_DC   26
#define TFT_RST  27

#define TFT_MOSI 34 //not used
#define TFT_SCLK 35 //not used

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK);

#define LEFT_BTN   21 
#define OK_BTN     22
#define RIGHT_BTN  23

#define DHT_PIN     2

void drawMenu();
void executeOption(int option);
int readDHT11Humidity(int pin);


int option = 0; // Track selected option
const int maxOption = 2;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting setup...");
  
  
  // Use a single, reliable initialization sequence:
  tft.initR(INITR_BLACKTAB); // Initialize the screen hardware
  tft.setRotation(1);       // Set rotation (0, 1, 2, or 3)
  tft.fillScreen(ST77XX_BLACK); // Clear the screen

  tft.setTextWrap(false);  
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
   Serial.println("Menu System Initialized");

  // ----------------------------------------Initialize buttons--------------------------
  pinMode(LEFT_BTN, INPUT_PULLUP);
  pinMode(OK_BTN, INPUT_PULLUP);
  pinMode(RIGHT_BTN, INPUT_PULLUP);

  //----------------------------------------- Draw initial menu-------------------------
  drawMenu();
  Serial.println("Setup complete, entering main loop.");
}

void loop() {
  // ------------------------------------Read buttons (active LOW)------------------------
  if (digitalRead(LEFT_BTN) == LOW) {
    option--;
    if (option < 0) option = maxOption;
    drawMenu();
    delay(200); // simple debounce

  if (digitalRead(RIGHT_BTN) == LOW) {
    option++;
    if (option > maxOption) option = 0;
    drawMenu();
    delay(200); // simple debounce
  }

  if (digitalRead(OK_BTN) == LOW) {
    executeOption(option);
    delay(200); // simple debounce
    Serial.print("\n> option ok");
  }}}


// -----------------------------------------Draw the menu on TFT
void drawMenu() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(10, 10);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_MAGENTA);
  tft.println("MENU:");

  const int maxOption = 2; 
  const char* options[] = {
    "1. Chat", "2. Check Temp/Humidity", "3. Screen saver"};

  for (int i = 0; i <= maxOption; i++) {
    if (i == option) {
      tft.setTextColor(ST77XX_WHITE); // Highlight selected option
    } else {
      tft.setTextColor(ST77XX_MAGENTA);
    }
    tft.setCursor(20, 40 + i * 30);
    tft.println(options[i]);
  }

}

// -----------------------------------------Execute the selected option
// --- define this function outside of executeOption ---
int readDHT11Humidity(int pin) {
    uint8_t data[5] = {0};

    pinMode(pin, OUTPUT); digitalWrite(pin, LOW); delay(18);
    digitalWrite(pin, HIGH); delayMicroseconds(40); pinMode(pin, INPUT);

    if (!pulseIn(pin, LOW, 100) || !pulseIn(pin, HIGH, 100)) return -1;

    for (int i = 0; i < 40; i++) {
        pulseIn(pin, LOW, 100);
        if (pulseIn(pin, HIGH, 100) > 40)
            data[i / 8] |= (1 << (7 - (i % 8)));
    }

    // simple checksum validation
    if (data[4] != (data[0] + data[1] + data[2] + data[3])) return -1;

    return data[0]; // humidity %
}

// --- your executeOption function ---
void executeOption(int option) {
    if (option == 0) {
        tft.fillScreen(ST77XX_BLACK);
        tft.setCursor(10, 10);
        tft.setTextSize(1);
        tft.setTextColor(ST77XX_MAGENTA);
        tft.println("Scanning Wi-Fi...");
        Serial.println("Scanning Wi-Fi...");

        scanWiFiNetworks();
    } 

    if (option == 1) {
        int humidity = readDHT11Humidity(2); // replace 2 with your DHT11 pin
        if (humidity >= 0) {
            tft.fillScreen(ST77XX_BLACK);
            tft.setCursor(10, 10);
            tft.setTextSize(1);
            tft.setTextColor(ST77XX_MAGENTA);
            tft.print("Humidity: ");
            tft.print(humidity);
            tft.println("%");
            Serial.print("Humidity: "); Serial.println(humidity);
        } else {
            Serial.println("DHT11 read failed");
        }
    }
}

