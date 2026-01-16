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

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define LEFT_BTN   21 
#define OK_BTN     22
#define RIGHT_BTN  23

void drawMenu();
void executeOption(int option);


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
  tft.setTextColor(ST77XX_WHITE);

  const int maxOption = 3; 
  const char* options[] = {
    "1. Scan Wi-Fi", "2. Check Temp/Humidity", "3. Option Three", "4. Option Four"};

  for (int i = 0; i <= maxOption; i++) {
    if (i == option) {
      tft.setTextColor(ST77XX_YELLOW); // Highlight selected option
    } else {
      tft.setTextColor(ST77XX_WHITE);
    }
    tft.setCursor(20, 40 + i * 30);
    tft.println(options[i]);
  }

}

// -----------------------------------------Execute the selected option
void executeOption(int opt) {
  if (option == 0)
  {
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(10, 10);
    tft.setTextSize(2);
    tft.setTextColor(ST77XX_CYAN);
    tft.println("Scanning Wi-Fi...");
    Serial.println("Scanning Wi-Fi...");

    scanWiFiNetworks();
  } 
  
}
