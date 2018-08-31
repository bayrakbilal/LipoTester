#include "Lipotester.h"
#include <Arduino.h>
#include <Adafruit_INA219.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

#define I2C_ADDRESS 0x3C
#define B_UP    2
#define B_DOWN  3
#define B_OK    4

SSD1306AsciiAvrI2c OLED;
Adafruit_INA219 INA(0x40);

float V = 0;
float mA = 0;
float mW = 0;
float mAh = 0;
float mWh = 0;
bool AKTIF = false;
unsigned long ZAMAN = 0;
unsigned long CALISMA = 0;
unsigned long TURZAMANI = 0;
unsigned long FARK = 0;
unsigned long TEST = 0;

Lipotester::Lipotester() {
  return;
}

void Lipotester::setup() {
  OLED.begin(&Adafruit128x64, I2C_ADDRESS);
  OLED.clear();
  OLED.setFont(Cooper21);
  OLED.setCursor(36,1);
  OLED.print("LIPO");
  OLED.setCursor(17,4);
  OLED.print("TESTER");
  delay(100);
  INA.begin();
  delay(1000);
  OLED.clear();
}

void Lipotester::loop() {
  getValues();
  showValues();
}

void Lipotester::getValues() {
  FARK = (millis()-TURZAMANI);
  if (FARK<100) {
    delay(100-FARK+5);
  }
  TURZAMANI = millis();

  V   = INA.getBusVoltage_V() + (INA.getShuntVoltage_mV() / 1000);
  mA  = INA.getCurrent_mA();
  if (mA >= 1 || mA <= -1) {
    if (AKTIF) {
      CALISMA = CALISMA + (millis() - ZAMAN);
    }
    ZAMAN = millis();
    AKTIF = true;
    mAh = mAh + (mA / 3600 / 10);
    mW  = V * mA;
    mWh = mWh + (V * mA / 3600 / 10);
  } else {
    if (AKTIF) {
      CALISMA = CALISMA + (millis() - ZAMAN);
      ZAMAN = millis();
    }
    mA = 0;
    mW = 0;
    AKTIF = false;
  }
}

void Lipotester::showValues() {
  //OLED.clear();
  OLED.setFont(lcdnums12x16);
  OLED.setCursor(0, 0);
  OLED.print(V);
  OLED.setFont(lcd5x7);
  OLED.setCursor(50, 1);
  OLED.print("V");

  OLED.setFont(lcd5x7);
  OLED.setCursor(69, 0);
  // Saat
  if (((CALISMA/1000) / 3600) < 10 ) {
    OLED.print("0");  
  }
  OLED.print(((CALISMA/1000) / 3600));
  OLED.print(":");  
  // Dakika
  if ((((CALISMA/1000) / 60) % 60) < 10 ) {
    OLED.print("0");  
  }
  OLED.print((((CALISMA/1000) / 60) % 60));
  OLED.print(":");  
  // Saniye
  if (((CALISMA/1000) % 60) < 10 ) {
    OLED.print("0");  
  }
  OLED.print(((CALISMA/1000) % 60));

  OLED.setCursor(69, 1);
  if (AKTIF) {
    if (mA > 0) {
      OLED.print(" DESARJ ");
    } else {
      OLED.print("  SARJ  ");
    }
  } else {
    OLED.print("BEKLIYOR");
  }


  OLED.setFont(lcdnums12x16);
  if ((int)mAh < 10) {
    OLED.setCursor(36, 3);
  } else if ((int)mAh < 100) {
    OLED.setCursor(24, 3);
  } else if ((int)mAh < 1000) {
    OLED.setCursor(12, 3);
  } else {
    OLED.setCursor(0, 3);
  }
  OLED.print((int)mAh);
  OLED.setFont(lcd5x7);
  OLED.setCursor(50, 4);
  OLED.print("mAh");

  OLED.setFont(lcd5x7);
  OLED.setCursor(74, 4);
  if (mA < 10) {
    OLED.print("   ");
  } else if (mA < 100) {
    OLED.print("  ");
  } else if (mA < 1000) {
    OLED.print(" ");    
  }
  OLED.print(mA);


  OLED.setFont(lcdnums12x16);
  if ((int)mWh < 10) {
    OLED.setCursor(36, 6);
  } else if ((int)mWh < 100) {
    OLED.setCursor(24, 6);
  } else if ((int)mWh < 1000) {
    OLED.setCursor(12, 6);
  } else {
    OLED.setCursor(0, 6);
  }
  OLED.print((int)mWh);
  OLED.setFont(lcd5x7);
  OLED.setCursor(50, 7);
  OLED.print("mWh");

  OLED.setFont(lcd5x7);
  OLED.setCursor(74, 7);
  if (mW < 10) {
    OLED.print("   ");
  } else if (mW < 100) {
    OLED.print("  ");
  } else if (mW < 1000) {
    OLED.print(" ");    
  }
  OLED.print(mW);
}