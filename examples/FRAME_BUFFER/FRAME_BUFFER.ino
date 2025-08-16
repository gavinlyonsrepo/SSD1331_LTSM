/*!
	@file   FRAME_BUFFER.ino
	@author Gavin Lyons
	@brief  Example  file for SSD1331_LTSM library. Tests  frame buffer mode
	@note   See USER OPTIONS 1-2 in SETUP function
			    dislib16_ADVANCED_SCREEN_BUFFER_ENABLE must be enabled for this example
			    or it will not compile 
	@test
		-# tTest 1500 Frame buffer mode.
*/

// libraries
#include "SSD1331_LTSM.hpp"
// Fonts needed
#include "fonts_LTSM/FontDefault_LTSM.hpp"
// bitmap test data
#include "bitmap_test_data_LTSM/Bitmap_TEST_Data_16color2.hpp"
#include "bitmap_test_data_LTSM/Bitmap_TEST_Data_bicolor.hpp"

/// @cond

#ifndef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is not defined. it is required for this example")
#endif

// Section :: Globals
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
  if (myOLED.setBuffer() != DisLib16::Success) {
    Serial.println("set up buffer error");
    while (1) {};
  } else {
    myOLED.setTextCharPixelOrBuffer(true);  // set to use pixel mode for text
    Serial.println("Start");
  }
}

//  Section ::  MAIN loop
void loop(void) {
  Test1();
  TestBitmap();
  EndTests();
}
// *** End OF MAIN **

void EndTests(void) {
    myOLED.fillScreen(myOLED.C_BLACK);
    myOLED.clearBuffer(myOLED.C_BLACK);
    myOLED.destroyBuffer();
    myOLED.fillScreen(myOLED.C_BLACK);
    delay(1000);
    myOLED.PowerDown();  // Power down device
    Serial.println("End Test");
    while (1) {};
  }

  void ScreenReset(void) {
    myOLED.writeBuffer(1);
    delay(2500);
    myOLED.clearBuffer(myOLED.C_BLACK);
  }

  void Test1(void) {
    Serial.println("Test1");
    colorBlock();
    delay(2500);
    myOLED.fillScreen(myOLED.C_BLACK);

    myOLED.fillCircle(20, 20, 10, myOLED.C_RED);
    delay(2500);
    myOLED.fillScreen(myOLED.C_BLACK);

    myOLED.clearBuffer(myOLED.C_RED);
    ScreenReset();

    myOLED.clearBuffer(myOLED.C_GREEN);
    ScreenReset();

    myOLED.clearBuffer(myOLED.C_BLUE);
    ScreenReset();

    myOLED.clearBuffer(myOLED.C_BLACK);
    myOLED.setTextColor(myOLED.C_GREEN, myOLED.C_BLACK);
    myOLED.setCursor(0, 0);
    myOLED.setFont(FontDefault);
    myOLED.print("Buffer Mode     Test");
    ScreenReset();
  }

  void TestBitmap(void) {
    // Bitmap 1-bit
    Serial.println("Bitmap 1-bit");
    myOLED.drawBitmap(10, 10, 40, 16, myOLED.C_CYAN, myOLED.C_BLACK, SunTextImage);
    ScreenReset();
    // Bitmap 16-bit
    Serial.println("Bitmap 16-bit");
    myOLED.drawBitmap16Data(10, 10, PosterImage, 80, 48);
    ScreenReset();
  }

  void colorBlock(void) {
    myOLED.fillRect(0, 0, 60, 8, myOLED.C_RED);
    myOLED.fillRect(0, 8, 60, 8, myOLED.C_GREEN);
    myOLED.fillRect(0, 16, 60, 8, myOLED.C_BLUE);
    myOLED.fillRect(0, 24, 60, 8, myOLED.C_YELLOW);
    myOLED.fillRect(0, 32, 60, 8, myOLED.C_WHITE);
  }
  /// @endcond
