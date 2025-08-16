/*! 
	@file   BITMAP_FUNC.ino
	@brief  Testing bitmap, functions hardware SPI test, SSD1331_LTSM OLED
	@author Gavin Lyons.
	@note   See USER OPTIONS 1-2 in SETUP function and in myOLED constructor
	@test 
		-# Test 300 Sprites, small bitmaps
		-# Test 303 16 bit color image bitmap from data array
		-# Test 500 RGB color OK?
		-# Test 501 change modes test -> Invert, display on/off and Sleep.
		-# Test 502 Rotate
		-# Test 503 windows command tests
		-# Test 504 Scroll
*/

// libraries
 #include "SSD1331_LTSM.hpp"
// Fonts needed
#include "fonts_LTSM/FontDefault_LTSM.hpp"
// bitmap test data
#include "bitmap_test_data_LTSM/Bitmap_TEST_Data_16color2.hpp"
#include "bitmap_test_data_LTSM/Bitmap_TEST_Data_bicolor.hpp"

/// @cond

// Section :: setup device
// Set contrast values at startup, range 0-0xFF
SSD1331_LTSM::Constrast_values_t userContrast = { 0x7A, 0x7A, 0x7A };        //normal mode
SSD1331_LTSM::Dim_Constrast_values_t userDimContrast = { 0x1E, 0x1E, 0x1E }; // dim mode
SSD1331_LTSM myOLED(SSD1331_LTSM::COLORORDER_RGB, userContrast, userDimContrast);

//  Test timing related defines
#define TEST_DELAY5 5000
#define TEST_DELAY2 2000
#define TEST_DELAY  1000

#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

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


// *** Main ***
void loop(void) 
{

	Test300();
	Test303();
	Test500();
	Test501();
	Test502();
	Test503();
	Test504();
	EndTests();
}
// *** End of Main **

//  Section ::  Function Space 

void Test300(void){
	myOLED.fillScreen(myOLED.C_BLACK);
	myOLED.drawBitmap(2,  2,  16, 8, myOLED.C_BLACK, myOLED.C_WHITE,  SignalIcon);
	myOLED.drawBitmap(20, 2,  16, 8, myOLED.C_BLACK, myOLED.C_WHITE,  MsgIcon);
	myOLED.drawBitmap(40, 2,  8,  8, myOLED.C_BLACK, myOLED.C_WHITE,  AlarmIcon);
	myOLED.drawBitmap(80, 2,  16, 8, myOLED.C_BLACK, myOLED.C_WHITE,  BatIcon);
	myOLED.drawBitmap(5,  20, 16, 8, myOLED.C_GREEN, myOLED.C_BLACK,  PowerIcon);
	myOLED.drawBitmap(20, 20, 16, 8, myOLED.C_RED,   myOLED.C_YELLOW, SpeedIcon);
	delay(5000);

	myOLED.drawSpriteData(5, 5, SpriteTest16, 32, 32, myOLED.C_LBLUE, false);
	myOLED.drawSpriteData(35, 35, SpriteTest16, 32, 32, myOLED.C_LBLUE, true);
	delay(5000);
	myOLED.fillScreen(myOLED.C_BLACK);

	myOLED.drawBitmap(0, 0, 40, 16, myOLED.C_CYAN, myOLED.C_BLACK, SunTextImage);
	myOLED.drawBitmap(20, 30, 40, 16, myOLED.C_RED, myOLED.C_BLACK, SunTextImage);
	myOLED.drawBitmap(30, 48, 40, 16, myOLED.C_YELLOW, myOLED.C_RED, SunTextImage);
	delay(5000);
	myOLED.fillScreen(myOLED.C_BLACK);

}

void Test303(void){
	myOLED.drawBitmap16Data(0, 0, PosterImage, 80, 48);
	delay(5000);
	myOLED.fillScreen(myOLED.C_BLACK);
}

void Test500(void)
{
	Serial.println("Test 500: Color Test:: Red,green,blue,yellow,white, black background");
	Serial.println("If wrong colour check color order setting in constructor");
	myOLED.fillScreen(myOLED.C_BLACK);
	myOLED.setTextColor(myOLED.C_GREEN,myOLED.C_BLACK);
	colorBlock();
	delay(TEST_DELAY5);

}

void colorBlock(void)
{
	myOLED.fillRect(0, 0, 60, 8, myOLED.C_RED);
	myOLED.fillRect(0, 8, 60, 8, myOLED.C_GREEN);
	myOLED.fillRect(0, 16, 60, 8, myOLED.C_BLUE);
	myOLED.fillRect(0, 24, 60, 8, myOLED.C_YELLOW);
	myOLED.fillRect(0, 32, 60, 8, myOLED.C_WHITE);
}

void Test501()
{
	Serial.println("Test 501: Mode Tests");
	// Invert on and off
	myOLED.changeDisplayMode(myOLED.DISPLAY_INVERSE);
	Serial.println("Test 501-1: Invert on ");
	delay(TEST_DELAY5);
	myOLED.changeDisplayMode(myOLED.DISPLAY_NORMAL);
	Serial.println("Test 501-2: Invert off ");
	delay(TEST_DELAY5);

	// ALL pixels on and off
	myOLED.changeDisplayMode(myOLED.DISPLAY_ALL_OFF);
	Serial.println("Test 501-3: All pixels off ");
	delay(TEST_DELAY5);
	myOLED.changeDisplayMode(myOLED.DISPLAY_ALL_ON);
	Serial.println("Test 501-4: All pixels on  ");
	delay(TEST_DELAY5);
	myOLED.changeDisplayMode(myOLED.DISPLAY_NORMAL);
	delay(TEST_DELAY2);

	// sleep mode
	myOLED.sleepDisplay();
	Serial.println("Test 501-5: Display off, sleep mode");
	delay(TEST_DELAY5);
	myOLED.enableDisplay();
	Serial.println("Test 501-6: Turn Display back on");
	delay(TEST_DELAY5);

	// dim mode
	myOLED.DimDisplay();
	Serial.println("Test 501-7: Display dim mode on");
	delay(TEST_DELAY5);
	myOLED.enableDisplay();
	Serial.println("Test 501-8: Display dim mode off");
	delay(TEST_DELAY2);
}


void Test502(void)
{
	Serial.println("Test 502: Window command Tests");
	Serial.println("Test 502-1 line draw command OLED command (not using graphics class)");
	myOLED.DrawLineCmd(0, 0, 50, 50, myOLED.C_GREEN);
	delay(TEST_DELAY5);
	Serial.println("Test 502-2 draw rect OLED command (not using graphics class)");
	myOLED.DrawRectCmd(0, 0, 80, 40, myOLED.C_YELLOW, false);
	delay(TEST_DELAY5);
	Serial.println("Test 502-3 fill rect OLED command (not using graphics class)");
	myOLED.DrawRectCmd(0, 0, 95, 63, myOLED.C_CYAN, true);
	delay(TEST_DELAY5);
	Serial.println("Test 502-4 Window clear command");
	myOLED.ClearWindowCmd(0, 0, 40, 40);
	delay(TEST_DELAY5);

	Serial.println("Test 502-5 Window copy command");
	myOLED.fillScreen(myOLED.C_BLACK);
	myOLED.fillRect(0, 0, 20, 20, myOLED.C_GREEN);
	delay(TEST_DELAY);
	myOLED.CopyWindowCmd(0,0, 20, 20, 76,0);
	delay(TEST_DELAY5);

	Serial.println("Test 502-6 Window Dim command");
	myOLED.fillScreen(myOLED.C_GREEN);
	delay(TEST_DELAY);
	myOLED.DimWindowCmd(0, 0, 40, 40);
	delay(TEST_DELAY5);
	
}

void Test503()
{
	Serial.println("Test 503: Rotate");
	myOLED.setFont(FontDefault);
	myOLED.setTextColor(myOLED.C_RED,myOLED.C_BLACK);
	char teststr0[] = "Rot 0";
	char teststr1[] = "Rot 90";
	char teststr2[] = "Rot 180";
	char teststr3[] = "R 270";

	myOLED.fillScreen(myOLED.C_BLACK);
	myOLED.setRotation(myOLED.Degrees_0);
	myOLED.writeCharString(0, 0, teststr0);
	myOLED.writeCharString(0, 50, teststr0);
	delay(TEST_DELAY2);

	myOLED.fillScreen(myOLED.C_BLACK);
	myOLED.setRotation(myOLED.Degrees_90);
	myOLED.writeCharString(0, 0, teststr1);
	myOLED.writeCharString(0, 50, teststr1);
	delay(TEST_DELAY2);

	myOLED.fillScreen(myOLED.C_BLACK);
	myOLED.setRotation(myOLED.Degrees_180);
	myOLED.writeCharString(0, 0, teststr2);
	myOLED.writeCharString(0, 50, teststr2);
	delay(TEST_DELAY2);

	myOLED.fillScreen(myOLED.C_BLACK);
	myOLED.setRotation(myOLED.Degrees_270);
	myOLED.writeCharString(0, 0, teststr3);
	myOLED.writeCharString(0, 50, teststr3);
	delay(TEST_DELAY2);

	myOLED.setRotation(myOLED.Degrees_0);
	myOLED.fillScreen(myOLED.C_BLACK);
}


void Test504(void)
{
	Serial.println("Test 504: Scroll");
	 // test horizontal scroll slow 
	Serial.println("Test 504-1: Scroll test horizontal");
	colorBlock();
	uint8_t  horOffset = 95;
	uint8_t startRowAddr = 0; 
	uint8_t numOfRows = 62;
	uint8_t verOffset = 0; 
	myOLED.ScrollSetup(horOffset, startRowAddr,  numOfRows, verOffset , myOLED.SCROLL_FRAME_6);
	myOLED.Scroll(myOLED.SCROLL_ACTIVATE);
	delay(10000);
	myOLED.Scroll(myOLED.SCROLL_DEACTIVE);
	myOLED.fillScreen(myOLED.C_BLACK);

	// test horizontal scroll slow 
	Serial.println("Test 504-2: Scroll test Vertical");
	colorBlock();
	horOffset = 0;
	numOfRows = 0;
	verOffset = 62; 
	myOLED.ScrollSetup(horOffset, startRowAddr,  numOfRows, verOffset , myOLED.SCROLL_FRAME_10);
	myOLED.Scroll(myOLED.SCROLL_ACTIVATE);
	delay(10000);
	myOLED.Scroll(myOLED.SCROLL_DEACTIVE);
	delay(5000);
	myOLED.fillScreen(myOLED.C_BLACK);
}


void EndTests(void)
{
	myOLED.fillScreen(myOLED.C_BLACK);
	delay(1000);
	myOLED.PowerDown(); // Power down device
	Serial.println("End Test");
  while(1){};
}

// *************** EOF ****************

/// @endcond
