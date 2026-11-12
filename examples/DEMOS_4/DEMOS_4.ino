/*!
	@file	    DEMOS_4.ino
	@brief	  Demo, SSD1331_LTSM, OLED, arduino library
	@author	  Gavin Lyons
	@details	96x64 SSD1331 OLED. Uses hardware or software SPI setup as required.
	@test     
			-# Test 404 Simulated VU meter
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

// Fixed-size storage avoids dynamic allocation.
static constexpr uint8_t NUM_BARS = 10;

struct VuBar_t
{
	int level = 10;	// Current level, 0-100%.
	int peak = 10;	// Peak hold position.
};

static std::array<VuBar_t, NUM_BARS> bars{{}};

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
	VUmeter(37500U);
	while (true)
	{
		delay(1000);
	}
}

// Demo: Simulated VU meter. 

static uint16_t getColorForLevel(const int level)
{
	// Simple green-to-red RGB565 level scale.
	const uint8_t red = static_cast<uint8_t>((level * 31) / 100);
	const uint8_t green = static_cast<uint8_t>(((100 - level) * 63) / 100);
	return static_cast<uint16_t>((red << 11) | (green << 5));
}

static void updateLevels(void)
{
	for (auto &bar : bars)
	{
		const int change = static_cast<int>(random(0, 5)) - 2;
		bar.level = constrain(bar.level + change, 10, 100);
		bar.peak = max(bar.peak - 1, bar.level);
	}
}

static void drawVuMeter(void)
{
	const int barWidth = OLED_WIDTH / NUM_BARS;
	const int maxHeight = OLED_HEIGHT - 5;

	myOLED.fillScreen(myOLED.C_BLACK);
	for (uint8_t i = 0; i < NUM_BARS; ++i)
	{
		const int height = (bars[i].level * maxHeight) / 100;
		const int peakY = OLED_HEIGHT - ((bars[i].peak * maxHeight) / 100);
		const uint16_t color = getColorForLevel(bars[i].level);

		myOLED.fillRectBuffer(i * barWidth, OLED_HEIGHT - height, barWidth - 2, height, color);
		myOLED.drawPixel(i * barWidth + barWidth / 2, peakY, myOLED.C_WHITE);
	}
}

void VUmeter(uint32_t durationMs)
{
	randomSeed(micros());
	const uint32_t startMillis = millis();

	while (static_cast<uint32_t>(millis() - startMillis) < durationMs)
	{
		updateLevels();
		drawVuMeter();
		delay(500);
	}

	myOLED.fillScreen(myOLED.C_BLACK);
	Serial.println("VU meter demo over");
}


/// @endcond
