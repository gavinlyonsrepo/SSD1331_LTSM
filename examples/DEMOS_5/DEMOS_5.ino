/*!
	@file	    DEMOS_5.ino
	@brief	  Demo, SSD1331_LTSM, OLED, arduino library
	@author	  Gavin Lyons
	@details	96x64 SSD1331 OLED. Uses hardware or software SPI setup as required.
	@test     
			-# Test 405 Signal generator
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

static constexpr int WAVE_AMPLITUDE = 20;
static constexpr int WAVE_FREQUENCY = 3;
static constexpr int SWEEP_SPEED = 4;

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
	SignalGen(50000U);
	while (true)
	{
		delay(1000);
	}
}

// Demo Signal generator.

void SignalGen(uint32_t durationMs)
{
	static float phaseShift = 0.0f;
	const uint32_t startMillis = millis();
	const int waveAreaHeight = 48;
	const int labelHeight = 16;
	const int waveYOffset = 0;

	myOLED.fillScreen(myOLED.C_BLACK);
	myOLED.setTextColor(myOLED.C_WHITE, myOLED.C_BLUE);
	myOLED.fillRectBuffer(0, 48, OLED_WIDTH, labelHeight, myOLED.C_BLUE);

	while (static_cast<uint32_t>(millis() - startMillis) < durationMs)
	{
		const uint32_t elapsedMs = static_cast<uint32_t>(millis() - startMillis);
		const uint8_t waveType = static_cast<uint8_t>((elapsedMs / (durationMs / 4U)) % 4U);

		static const char * const waveNames[] =
		{
			"Sine",
			"Square",
			"Triangle",
			"Sawtooth"
		};

		myOLED.setCursor(10, 52);
		myOLED.fillRectBuffer(0, 48, OLED_WIDTH, labelHeight, myOLED.C_BLUE);
		myOLED.print(waveNames[waveType]);
		myOLED.fillRectBuffer(0, 0, OLED_WIDTH, waveAreaHeight, myOLED.C_BLACK);

		int previousY = waveYOffset + waveAreaHeight / 2;
		for (int x = 0; x < OLED_WIDTH; x += 2)
		{
			const float angle = ((x + phaseShift) * 2.0f * PI) /
				(static_cast<float>(OLED_WIDTH) / WAVE_FREQUENCY);
			int y = waveYOffset + waveAreaHeight / 2;

			switch (waveType)
			{
				case 0: // Sine wave.
					y += static_cast<int>(WAVE_AMPLITUDE * sinf(angle));
					break;

				case 1: // Square wave.
					y += (sinf(angle) > 0.0f) ? WAVE_AMPLITUDE : -WAVE_AMPLITUDE;
					if (x > 0 && ((sinf(angle) > 0.0f) !=
						(sinf(angle - 2.0f * PI / WAVE_FREQUENCY) > 0.0f)))
					{
						myOLED.drawFastVLine(x, min(previousY, y), abs(previousY - y), myOLED.C_GREEN);
					}
					break;

				case 2: // Triangle wave.
					y += static_cast<int>(WAVE_AMPLITUDE * (2.0f / PI) * asinf(sinf(angle)));
					break;

				case 3: // Sawtooth wave.
				{
					const float saw = atanf(tanf(angle / 2.0f));
					y += static_cast<int>(WAVE_AMPLITUDE * (2.0f / PI) * saw);
					const float previousSaw = atanf(tanf((angle - 2.0f * PI / WAVE_FREQUENCY) / 2.0f));
					if (x > 0 && saw < previousSaw)
					{
						myOLED.drawFastVLine(x, min(previousY, y), abs(previousY - y), myOLED.C_GREEN);
					}
					break;
				}
			}

			myOLED.drawPixel(x, y, myOLED.C_GREEN);
			previousY = y;
		}

		phaseShift += (SWEEP_SPEED * 2.0f * PI) /
			(static_cast<float>(OLED_WIDTH) / WAVE_FREQUENCY);
		const float phaseLimit = static_cast<float>(OLED_WIDTH) / WAVE_FREQUENCY;
		if (phaseShift >= phaseLimit)
		{
			phaseShift -= phaseLimit;
		}

		delay(500);
	}

	myOLED.fillScreen(myOLED.C_BLACK);
	Serial.println("Signal generator demo over");
}


/// @endcond
