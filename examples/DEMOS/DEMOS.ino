/*! 
	@file    DEMOS.ino
	@brief   Demo hardware SPI test, SSD1331_LTSM, OLED, arduino library
	@author  Gavin Lyons.
	@details See USER OPTIONS 1-2 in SETUP function and in myOLED constructor
			    If dislib16_ADVANCED_GRAPHICS_ENABLE is not enabled it will not compile
	@test 
		-# 401 Radar demo
*/

#include "SSD1331_LTSM.hpp"
// Fonts needed
#include "fonts_LTSM/FontDefault_LTSM.hpp"

/// @cond

#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif
#ifndef dislib16_ADVANCED_GRAPHICS_ENABLE
#pragma message("gll: dislib16_ADVANCED_GRAPHICS_ENABLE is not defined. it is required for this demo")
#endif

// Section :: setup device
// Set contrast values at startup, range 0-0xFF
SSD1331_LTSM::Constrast_values_t userContrast = { 0x7A, 0x7A, 0x7A };         //normal mode
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
  DemoRadar(15);
  EndTests();
}
// *** End of Main **

void DemoRadar(uint16_t sweeps) {
  Serial.print("Demo Radar: Draw line at angle function, ends at: ");
  Serial.println(sweeps);
  myOLED.setFont(FontDefault);
  myOLED.setTextColor(myOLED.C_BLACK, myOLED.C_TAN);
  myOLED.fillScreen(myOLED.C_TAN);
  const int centerX = 32;
  const int centerY = 32;
  const int radarRadius = 30;
  const int lineLength = 28;
  int targetX = 24;
  int targetY = 20;
  // Draw radar circles
  myOLED.fillCircle(centerX, centerY, radarRadius, myOLED.C_BLACK);
  myOLED.drawCircle(centerX, centerY, radarRadius, myOLED.C_GREEN);
  myOLED.drawCircle(centerX, centerY, radarRadius - 1, myOLED.C_GREEN);
  uint16_t prevAngle = 0;
  uint16_t hitCount = 0;
  myOLED.setCursor(71, 55);
  myOLED.print(hitCount);
  // Sweep loop
  for (uint16_t j = 0; j < sweeps; j++) {
    for (uint16_t angle = 0; angle < 360; angle += 2) {
      // Erase previous line
      myOLED.drawLineAngle(centerX, centerY, prevAngle, 0, lineLength, -90, myOLED.C_BLACK);
      // Draw new radar line
      myOLED.drawLineAngle(centerX, centerY, angle, 0, lineLength, -90, myOLED.C_GREEN);
      if (j != 0) { myOLED.fillCircle(targetX, targetY, 2, myOLED.C_RED); }
      // Check if the radar line hits the target
      if (angle == 320) {
        myOLED.fillCircle(targetX, targetY, 2, myOLED.C_WHITE);  // Flash effect
        delay(50);        
        hitCount++;
        myOLED.fillRectBuffer(71, 55, 40, 10, myOLED.C_TAN);
        myOLED.setCursor(71, 55);
        myOLED.print(hitCount);
      }
      // Update angle display
      if ((angle % 10) == 0) {
        myOLED.fillRectBuffer(71, 5, 28, 10, myOLED.C_TAN);
        myOLED.setCursor(71, 5);
        myOLED.print(angle);
      }
      prevAngle = angle;
      delay(15);
    }
    Serial.print(j);
    Serial.print('\r');
  }
  myOLED.fillScreen(myOLED.C_BLACK);
}

void EndTests(void) {
  myOLED.fillScreen(myOLED.C_BLACK);
  delay(1000);
  myOLED.PowerDown();  // Power down device
  Serial.println("End");
  while(1){};
}


/// @endcond
