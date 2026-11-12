/*!
	@file	    DEMOS_2.ino
	@brief	  Demo, SSD1331_LTSM, OLED, arduino library
	@author	  Gavin Lyons
	@details	96x64 SSD1331 OLED. Uses hardware or software SPI setup as required.
	@test     
			-# Test 402 Analog clock
*/

#include "SSD1331_LTSM.hpp"
#include "fonts_LTSM/FontDefault_LTSM.hpp"

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
	ClockDemo(90000U);
	while (true)
	{
		delay(1000);
	}
}

// millis() supplies elapsed time since startup; this is not wall-clock time.
void ClockDemo(uint32_t durationMs)
{{
	const int centerX = OLED_WIDTH / 2;
	const int centerY = OLED_HEIGHT / 2;
	const int radius = (OLED_HEIGHT - 10) / 2;

	int previousSecondX = centerX;
	int previousSecondY = centerY;
	int previousMinuteX = centerX;
	int previousMinuteY = centerY;
	int previousHourX = centerX;
	int previousHourY = centerY;

	uint32_t startMillis = millis();
	uint32_t lastSecond = UINT32_MAX;

	myOLED.fillScreen(myOLED.C_DGREY);
	myOLED.fillCircle(centerX, centerY, radius, myOLED.C_BLACK);

	while (static_cast<uint32_t>(millis() - startMillis) < durationMs)
	{{
		const uint32_t elapsedSeconds = static_cast<uint32_t>(millis() - startMillis) / 1000U;
		if (elapsedSeconds != lastSecond)
		{{
			const uint32_t second = elapsedSeconds % 60U;
			const uint32_t minute = (elapsedSeconds / 60U) % 60U;
			const uint32_t hour = (elapsedSeconds / 3600U) % 12U;

			// Erase the previous hands.
			myOLED.drawLine(centerX, centerY, previousHourX, previousHourY, myOLED.C_BLACK);
			myOLED.drawLine(centerX, centerY, previousMinuteX, previousMinuteY, myOLED.C_BLACK);
			myOLED.drawLine(centerX, centerY, previousSecondX, previousSecondY, myOLED.C_BLACK);

			// Calculate and draw the new hands.
			float angle = 2.0f * PI * static_cast<float>(hour) / 12.0f;
			previousHourX = centerX + static_cast<int>((radius / 2) * sinf(angle));
			previousHourY = centerY - static_cast<int>((radius / 2) * cosf(angle));
			myOLED.drawLine(centerX, centerY, previousHourX, previousHourY, myOLED.C_RED);

			angle = 2.0f * PI * static_cast<float>(minute) / 60.0f;
			previousMinuteX = centerX + static_cast<int>((2 * radius / 3) * sinf(angle));
			previousMinuteY = centerY - static_cast<int>((2 * radius / 3) * cosf(angle));
			myOLED.drawLine(centerX, centerY, previousMinuteX, previousMinuteY, myOLED.C_GREEN);

			angle = 2.0f * PI * static_cast<float>(second) / 60.0f;
			previousSecondX = centerX + static_cast<int>((radius - 2) * sinf(angle));
			previousSecondY = centerY - static_cast<int>((radius - 2) * cosf(angle));
			myOLED.drawLine(centerX, centerY, previousSecondX, previousSecondY, myOLED.C_YELLOW);

			lastSecond = elapsedSeconds;
		}}
		delay(10);
	}}

	myOLED.fillScreen(myOLED.C_BLACK);
	Serial.println("Analog clock demo over");
}}


/// @endcond
