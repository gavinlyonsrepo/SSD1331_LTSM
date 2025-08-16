/*! 
	@file    TEXT_GRAPHICS.ino
	@brief   Fonts + graphics test, for SSD1331_LTSM OLED arduino library
	@author  Gavin Lyons.
  @details set 'bhardwareSPI' to choose SPI mode 
	@note    See USER OPTIONS 1-2 in SETUP function and in myOLED constructor
	@test 
		-# 701 Test print all fonts
    -# 901 Test graphics
    -# 902 Test advanced graphics 
*/

// libraries
#include "SSD1331_LTSM.hpp"
// Included Fonts
#include <fonts_LTSM/FontArialBold_LTSM.hpp>
#include <fonts_LTSM/FontArialRound_LTSM.hpp>
#include <fonts_LTSM/FontDefault_LTSM.hpp>
#include <fonts_LTSM/FontGll_LTSM.hpp>
#include <fonts_LTSM/FontGroTeskBig_LTSM.hpp>
#include <fonts_LTSM/FontGroTesk_LTSM.hpp>
#include <fonts_LTSM/FontHallfetica_LTSM.hpp>
#include <fonts_LTSM/FontInconsola_LTSM.hpp>
#include <fonts_LTSM/FontMega_LTSM.hpp>
#include <fonts_LTSM/FontMint_LTSM.hpp>
#include <fonts_LTSM/FontOrla_LTSM.hpp>
#include <fonts_LTSM/FontPico_LTSM.hpp>
#include <fonts_LTSM/FontRetro_LTSM.hpp>
#include <fonts_LTSM/FontSevenSeg_LTSM.hpp>
#include <fonts_LTSM/FontSinclairS_LTSM.hpp>
#include <fonts_LTSM/FontSixteenSeg_LTSM.hpp>

/// @cond

//  Test timing related defines
#define TEST_DELAY5 5000
#define TEST_DELAY2 2000
#define TEST_DELAY 1000

#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif
#ifndef dislib16_ADVANCED_GRAPHICS_ENABLE
#pragma message("gll: define dislib16_ADVANCED_GRAPHICS_ENABLE is not defined. Test 902 will be run")
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
  myOLED.fillScreen(myOLED.C_BLACK);
}

//  *** Main ***
void loop(void) {
  TestFonts();
  TestGraphics();
#ifdef dislib16_ADVANCED_GRAPHICS_ENABLE
  TestAdvancedGraphics();
#endif
  EndTests();
}
// *** End of Main ***

//  Function Space
void TestFonts(void) {
  myOLED.setCursor(0, 0);
  myOLED.setTextColor(myOLED.C_GREEN, myOLED.C_BLACK);
  myOLED.setFont(FontDefault);
  myOLED.println("Default");
  myOLED.setFont(FontSinclairS);
  myOLED.println("Sinclair");
  myOLED.setFont(FontGll);
  myOLED.println("GLL");  // No lowercase letters.
  myOLED.setFont(FontRetro);
  myOLED.println("Retro");
  myOLED.setFont(FontPico);
  myOLED.println("Pico");
  resetScreen();

  myOLED.setCursor(0, 0);
  myOLED.setTextColor(myOLED.C_YELLOW, myOLED.C_BLACK);
  myOLED.setFont(FontMega);
  myOLED.println("Mega");
  myOLED.setFont(FontArialBold);
  myOLED.println("Aria B");
  myOLED.setFont(FontHallfetica);
  myOLED.println("Hall f");
  resetScreen();

  myOLED.setCursor(0, 0);
  myOLED.setTextColor(myOLED.C_RED, myOLED.C_BLACK);
  myOLED.setFont(FontOrla);
  myOLED.println("Orla");
  myOLED.setFont(FontArialRound);
  myOLED.println("Aria R");
  resetScreen();

  myOLED.setCursor(0, 0);
  myOLED.setTextColor(myOLED.C_GREEN, myOLED.C_BLACK);
  myOLED.setFont(FontGroTesk);
  myOLED.println("GroT");
  myOLED.setFont(FontInconsola);
  myOLED.println("Inco");
  resetScreen();

  myOLED.setCursor(0, 0);
  myOLED.setFont(FontMint);
  myOLED.println("Mint");
  resetScreen();

  myOLED.setCursor(0, 0);
  myOLED.setFont(FontSixteenSeg);  // numbers +  : . / - only
  myOLED.println("16");
  resetScreen();

  myOLED.setCursor(0, 0);
  myOLED.setFont(FontSevenSeg);  // numbers +  : . / - only
  myOLED.println("7");
  resetScreen();

  myOLED.setCursor(0, 0);
  myOLED.setFont(FontGroTeskBig);  // no lower case letters
  myOLED.println("GTB");
  resetScreen();
}

void TestGraphics(void) {
  DisLib16::Ret_Codes_e returnValue;
  // rectangle with local buffer
  returnValue = myOLED.fillRectBuffer(5, 5, 15, 15, myOLED.C_YELLOW);
  if (returnValue != DisLib16::Success) {
    Serial.print("An error occurred returnValue : ");
    Serial.println(returnValue);
  }
  // rectangle without buffer
  myOLED.drawRectWH(25, 5, 15, 15, myOLED.C_RED);
  myOLED.fillRect(45, 5, 15, 15, myOLED.C_GREEN);
  resetScreen();
  // rounded rectangle
  myOLED.drawRoundRect(5, 25, 25, 20, 5, myOLED.C_CYAN);
  myOLED.fillRoundRect(35, 25, 25, 20, 5, myOLED.C_WHITE);
  resetScreen();
  // circles
  myOLED.drawCircle(20, 50, 10, myOLED.C_GREEN);
  myOLED.fillCircle(50, 50, 10, myOLED.C_YELLOW);
  resetScreen();
  // triangles
  myOLED.drawTriangle(5, 60, 30, 35, 55, 60, myOLED.C_CYAN);
  myOLED.fillTriangle(60, 60, 80, 40, 95, 60, myOLED.C_RED);
  resetScreen();
}

#ifdef dislib16_ADVANCED_GRAPHICS_ENABLE
void TestAdvancedGraphics(void) {
  myOLED.fillScreen(myOLED.C_BLACK);
  // drawPolygon(x, y, sides, diameter, rotation, filled, color);
  // octagon outline
  myOLED.drawPolygon(48, 32, 8, 20, 0, false, myOLED.C_GREEN);
  delay(TEST_DELAY5);
  // filled octagon
  myOLED.drawPolygon(48, 32, 8, 20, 0, true, myOLED.C_YELLOW);
  resetScreen();
  // drawDotGrid(x, y, w, h, DotGridGap, color);
  // grid centered inside 96x64
  myOLED.drawDotGrid(8, 8, 80, 48, 4, myOLED.C_CYAN);
  resetScreen();
  // Define quadrilaterals to fit inside 96x64
  // Quadrilateral (top-left region)
  int16_t x0 = 5, y0 = 5;
  int16_t x1 = 35, y1 = 5;
  int16_t x2 = 35, y2 = 20;
  int16_t x3 = 5, y3 = 20;
  // Parallelogram (top-right region)
  int16_t x4 = 55, y4 = 5;
  int16_t x5 = 85, y5 = 10;
  int16_t x6 = 75, y6 = 25;
  int16_t x7 = 45, y7 = 20;
  // Trapezoid 1 (center region)
  int16_t x8 = 25, y8 = 30;
  int16_t x9 = 35, y9 = 45;
  int16_t x10 = 60, y10 = 45;
  int16_t x11 = 70, y11 = 30;
  // Trapezoid 2 (bottom region)
  int16_t x12 = 20, y12 = 50;
  int16_t x13 = 30, y13 = 60;
  int16_t x14 = 65, y14 = 60;
  int16_t x15 = 55, y15 = 50;
  // Draw them
  myOLED.drawQuadrilateral(x0, y0, x1, y1, x2, y2, x3, y3, myOLED.C_RED);
  myOLED.fillQuadrilateral(x4, y4, x5, y5, x6, y6, x7, y7, myOLED.C_GREEN);
  myOLED.fillQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myOLED.C_BLUE);
  myOLED.drawQuadrilateral(x12, y12, x13, y13, x14, y14, x15, y15, myOLED.C_YELLOW);
  resetScreen();
  // Format: drawEllipse(centerX, centerY, semiMajorAxis, semiMinorAxis, fill, color)
  // Unfilled ellipse (centered in screen)
  myOLED.drawEllipse(48, 32, 20, 12, false, myOLED.C_GREEN);
  delay(TEST_DELAY5);
  // Filled ellipse at same position
  myOLED.drawEllipse(48, 32, 20, 12, true, myOLED.C_RED);
  resetScreen();

  int16_t centerX = 48;  // X-coordinate of the circle center
  int16_t centerY = 32;  // Y-coordinate of the circle center
  int16_t radius = 30;   // Radius of the circle
  Serial.print(" Angle offset default : ");
  Serial.println(myOLED.getArcAngleOffset());
  //drawArc(cx,  cy,  radius, thickness, startAngle, endAngle, color);
  // _arcAngleOffset is zero default means 0 is positive Axis , Arc drawn counterclockwise
  // XXX 270 XXX
  // 180 XXX 000
  // XXX 090 XXX
  // Draw the full lower half (0° to 180°)
  myOLED.drawArc(centerX, centerY, radius, 7, 0.0f, 180.0f, myOLED.C_GREEN);
  // Draw the full upper half (180° to 360°)
  myOLED.drawArc(centerX, centerY, radius, 7, 180.0f, 360.0f, myOLED.C_BLUE);
  resetScreen();

  // Draw the first quarter (0° to 90°)
  myOLED.drawArc(centerX, centerY, radius, 2, 0.0f, 90.0f, myOLED.C_RED);
  // Draw the second quarter (90° to 180°)
  myOLED.drawArc(centerX, centerY, radius, 2, 90.0f, 180.0f, myOLED.C_YELLOW);
  // Draw the third quarter (180° to 270°)
  myOLED.drawArc(centerX, centerY, radius, 2, 180.0f, 270.0f, myOLED.C_CYAN);
  // Draw the fourth quarter (270° to 360°)
  myOLED.drawArc(centerX, centerY, radius, 2, 270.0f, 360.0f, myOLED.C_GREEN);
  resetScreen();
}
#endif

void EndTests(void) {
  myOLED.fillScreen(myOLED.C_BLACK);
  delay(1000);
  myOLED.PowerDown();  // Power down device
  Serial.println("End");
  while (1) {};
}

void resetScreen(void) {
  delay(TEST_DELAY5);
  myOLED.fillScreen(myOLED.C_BLACK);
}
/// @endcond
