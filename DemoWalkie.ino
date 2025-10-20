#include "FastLED.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// On an arduino UNO: A4(SDA), A5(SCL)

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // 0x3D or _0x3C_
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUM_LEDS 10
#define LEDS_PIN 2
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

#define vol_up 7
#define vol_down 8
#define freq 6
#define lights 9

bool light_state = 0;
int vol = 4;
int rad_num = 1;
int battery = 50;

void setup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }

  display.clearDisplay();
  display.drawPixel(10, 10, SSD1306_WHITE);
  display.display();
  

  FastLED.addLeds<WS2812, LEDS_PIN, COLOR_ORDER>(leds, NUM_LEDS);

  pinMode(vol_up, INPUT);
  pinMode(vol_down, INPUT);
  pinMode(freq, INPUT);
  pinMode(lights, INPUT);

  cycleLEDS();
  
  Serial.begin(9600);
  delay(500);
}

void loop() {
  home_text();

  if (light_state) {
    whiteLEDS();
  } else {
    offLEDS();
  }

  if (digitalRead(vol_up) == 1 && vol < 10) {
    vol++;
    delay(300);
  }
  else if (digitalRead(vol_down) == 1 && vol > 0) {
    vol--;
    delay(300);
  }
  else if (digitalRead(freq) == 1) {
    if (rad_num < 4) {
      rad_num++;
    } else {
      rad_num = 1;
    }
    delay(300);
  }
  else if (digitalRead(lights) == 1) {
    light_state = !light_state;
    delay(300);
  }  
  
}

void cycleLEDS() {
  for (int i = 0; i < 10; i++) {
    leds[i] = CRGB(0, 255, 0);
    FastLED.show();
    delay(100);
    leds[i] = CRGB(0, 0, 0);
  }
}

void whiteLEDS() {
  for (int i = 0; i < 10; i++) {
    leds[i] = CRGB(255, 255, 255);
  }
  FastLED.show();
}

void offLEDS() {
  for (int i = 0; i < 10; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
  FastLED.show();
}

void home_text(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  
  display.print(F("Bat: "));
  display.print(battery);
  display.println(F("%"));

  display.setCursor(10, 20);
  
  display.print(F("Volume: "));
  display.println(vol);

  display.print(F("Channel: "));
  display.println(rad_num);

  display.display();
}
