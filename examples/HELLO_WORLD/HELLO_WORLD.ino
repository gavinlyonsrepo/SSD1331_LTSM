/*! 
	@file    HELLO_WORLD.ino
	@brief   Hello World test, for SSD1331_LTSM OLED arduino library
	@author  Gavin Lyons.
  @details set 'bhardwareSPI' to choose SPI mode 
	@note    See USER OPTIONS 1-2 in SETUP function and in myOLED constructor
	@test 
		-# 101 Hello World
*/

// libraries
#include "SSD1331_LTSM.hpp"
// Fonts needed
#include "fonts_LTSM/FontDefault_LTSM.hpp"

/// @cond

#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

// Section :: setup device
// Set contrast values at startup, range 0-0xFF
SSD1331_LTSM::Constrast_values_t userContrast = { 0x7D, 0x7D, 0x7D };         //normal mode
SSD1331_LTSM::Dim_Constrast_values_t userDimContrast = { 0x1E, 0x1E, 0x1E };  // dim mode
SSD1331_LTSM myOLED(SSD1331_LTSM::COLORORDER_RGB, userContrast, userDimContrast);

// true for hardware SPI, false for software SPI
bool bhardwareSPI = true;

void setup(void) {
  Serial.begin(38400);
  delay(1000);
  // === USER OPTION 1 GPIO + SPI_SPEED + TYPE ===
  int8_t DC_OLED = 5;
  int8_t RST_OLED = 4;
  int8_t CS_OLED = 15;
  if (bhardwareSPI == true) {      // hw spi
    uint32_t SCLK_FREQ = 8000000;  // Spi freq in Hertz
    myOLED.SetupGPIO_SPI(SCLK_FREQ, RST_OLED, DC_OLED, CS_OLED);
  } else {                        // sw spi
    uint16_t SWSPICommDelay = 0;  // optional SW SPI GPIO delay in uS
    int8_t SDIN_OLED = 13;
    int8_t SCLK_OLED = 12;
    myOLED.SetupGPIO_SPI(SWSPICommDelay, RST_OLED, DC_OLED, CS_OLED, SCLK_OLED, SDIN_OLED);
  }
  // ===
  // === USER OPTION 2 Screen Setup ===
  uint16_t OLED_WIDTH = 96;   // Screen width in pixels
  uint16_t OLED_HEIGHT = 64;  // Screen height in pixels
  myOLED.InitScreenSize(OLED_WIDTH, OLED_HEIGHT);
  // ===
  myOLED.SSD1331Initialize();
  delay(100);
  Serial.println("Start");
}

//  *** Main ***
void loop(void) {
  HelloWorld();
  EndTests();
}
// *** End of Main **

//  Function Space
void HelloWorld(void) {
  myOLED.fillScreen(myOLED.C_BLACK);
  myOLED.setCursor(0, 0);
  myOLED.setTextColor(myOLED.C_GREEN, myOLED.C_BLACK);
  myOLED.setFont(FontDefault);
  myOLED.print("Hello world");
  delay(7000);
}

void EndTests(void) {
  myOLED.fillScreen(myOLED.C_BLACK);
  delay(1000);
  myOLED.PowerDown();  // Power down device
  Serial.println("End");
  while (1) {};
}

/// @endcond
