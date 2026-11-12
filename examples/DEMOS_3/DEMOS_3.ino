/*!
	@file	    DEMOS_3.ino
	@brief	  Demo, SSD1331_LTSM, OLED, arduino library
	@author	  Gavin Lyons
	@details	96x64 SSD1331 OLED. Uses hardware or software SPI setup as required.
	@test     
			-# Test 403 Digital clock with bitmaps
*/

#include "SSD1331_LTSM.hpp"
#include "fonts_LTSM/FontDefault_LTSM.hpp"
#include "bitmap_test_data_LTSM/Bitmap_TEST_Data_bicolor.hpp" //bitmap test data

/// @cond

#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

// Set contrast values at startup, range 0-0xFF.
SSD1331_LTSM::Constrast_values_t userContrast = { 0x7A, 0x7A, 0x7A };
SSD1331_LTSM::Dim_Constrast_values_t userDimContrast = { 0x1E, 0x1E, 0x1E };
SSD1331_LTSM myOLED(SSD1331_LTSM::COLORORDER_RGB, userContrast, userDimContrast);

// true for hardware SPI, false for software SPI.
bool bhardwareSPI = true;

static constexpr uint16_t OLED_WIDTH = 96;
static constexpr uint16_t OLED_HEIGHT = 64;

void setup(void)
{
	Serial.begin(38400);
	delay(1000);

	// === USER OPTION 1: GPIO + SPI speed + type ===
	const int8_t DC_OLED = 5;
	const int8_t RST_OLED = 4;
	const int8_t CS_OLED = 15;

	if (bhardwareSPI)
	{
		const uint32_t SCLK_FREQ = 8000000;
		myOLED.SetupGPIO_SPI(SCLK_FREQ, RST_OLED, DC_OLED, CS_OLED);
	}
	else
	{
		const uint16_t SWSPICommDelay = 0;
		const int8_t SDIN_OLED = 13;
		const int8_t SCLK_OLED = 12;
		myOLED.SetupGPIO_SPI(SWSPICommDelay, RST_OLED, DC_OLED, CS_OLED, SCLK_OLED, SDIN_OLED);
	}

	// === USER OPTION 2: screen setup ===
	myOLED.InitScreenSize(OLED_WIDTH, OLED_HEIGHT);
	myOLED.SSD1331Initialize();
	delay(100);
	Serial.println("Start");
}

void loop(void)
{
	DigitalClock(90000U);
	while (true)
	{
		delay(1000);
	}
}


// millis() supplies elapsed time since startup; no RTC or network time is used.
void DigitalClock(uint32_t durationMs)
{
	const uint32_t startMillis = millis();

	myOLED.fillScreen(myOLED.C_BLACK);

	// Top status bar icons.
	myOLED.drawBitmap(0, 0, 16,  8, myOLED.C_BLACK, myOLED.C_WHITE, SignalIcon);
	myOLED.drawBitmap(16, 0, 16, 8, myOLED.C_BLACK, myOLED.C_WHITE, MsgIcon);
	myOLED.drawBitmap(32, 0, 8,  8, myOLED.C_BLACK, myOLED.C_WHITE, AlarmIcon);
	myOLED.drawBitmap(42, 0, 16, 8, myOLED.C_BLACK, myOLED.C_WHITE, BatIcon);
	myOLED.drawBitmap(60, 0, 16, 8, myOLED.C_GREEN, myOLED.C_BLACK, PowerIcon);
	myOLED.drawBitmap(80, 0, 16, 8, myOLED.C_RED, myOLED.C_YELLOW, SpeedIcon);

	myOLED.setFont(FontDefault);
	myOLED.setCursor(0, 20);
	myOLED.print("GLL");

	// Display the supplied SUN bitmap.
	myOLED.drawBitmap(40, 20, 40, 16, myOLED.C_CYAN, myOLED.C_BLACK, SunTextImage);

	myOLED.drawFastHLine(0, 18, 95, myOLED.C_WHITE);
	myOLED.drawFastHLine(0, 40, 95, myOLED.C_WHITE);

	uint32_t lastSecond = UINT32_MAX;
	while (static_cast<uint32_t>(millis() - startMillis) < durationMs)
	{
		const uint32_t elapsedSeconds = static_cast<uint32_t>(millis() - startMillis) / 1000U;
		if (elapsedSeconds != lastSecond)
		{
			const uint32_t hours = (elapsedSeconds / 3600U) % 100U;
			const uint32_t minutes = (elapsedSeconds / 60U) % 60U;
			const uint32_t seconds = elapsedSeconds % 60U;

			char timeText[9];
			snprintf(timeText, sizeof(timeText), "%02lu:%02lu:%02lu",
				static_cast<unsigned long>(hours),
				static_cast<unsigned long>(minutes),
				static_cast<unsigned long>(seconds));

			myOLED.setFont(FontDefault);
			myOLED.setTextColor(myOLED.C_RED, myOLED.C_BLACK);
			myOLED.fillRectBuffer(0, 41, 96, 8, myOLED.C_BLACK);
			myOLED.setCursor(5, 41);
			myOLED.print(timeText);

			myOLED.setTextColor(myOLED.C_GREEN, myOLED.C_BLACK);
			myOLED.setCursor(5, 52);
			myOLED.print("UPTIME");

			lastSecond = elapsedSeconds;
		}
		delay(20);
	}

	myOLED.fillScreen(myOLED.C_BLACK);
	Serial.println("Digital clock demo over");
}


/// @endcond
