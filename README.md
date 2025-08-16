# SSD1331 OLED Readme

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

## Table of contents

* [Overview](#overview)
* [Installation](#installation)
* [Dependency](#dependency)
* [Documentation](#documentation)
* [Software](#software)
	* [User options](#user-options)
	* [Examples](#examples)
* [Hardware](#hardware)
* [Tested](#tested)
* [Output](#output)
* [Notes and Issues](#notes-and-issues)


## Overview

* Name: SSD1331_LTSM
* Author: Gavin Lyons
* Description:

C++ Arduino library for a TFT SPI LCD: SSD1331 Driver.
SSD1331 96x 64 RGB Dot Matrix OLED/PLED Segment/Common Driver with Controller.

1. Contrast, dim mode, Invert colour, scroll, rotate, sleep modes supported.
2. Hardware & software SPI
3. Tested on 96x64 Display
4. Arduino eco-system library.
5. 16 ASCII fonts included, fonts can easily be removed or added.
6. Advanced graphics class included.
7. Advanced frame buffer mode included.
8. Bitmaps supported: 1, 8 and 16 bit.
9. Hardware & software SPI options
10. [Project url link](https://github.com/gavinlyonsrepo/SSD1331LTSM)

## Installation

The library is included in the official Arduino library manger and the optimum way to install it is using the library manager which can be opened by the *manage libraries* option in Arduino IDE. 

## Dependency

This library requires the Arduino library 'display16_LTSM' as a dependency. display16_LTSM library contains
the graphics, bitmaps, and font methods as well as font data and bitmap test data. Its also 
where the user sets options(debug, advanced graphics and frame buffer mode).
When you install 'SSD1331_LTSM' with Arduino IDE. It should install 'display16_LTSM' as well after 
a prompt, if it does not you have to install it same way as 'SSD1331_LTSM'.
The 'display16_LTSM' project and readme is at [URL github link.](https://github.com/gavinlyonsrepo/display16_LTSM)
'display16_LTSM' is also written by author of this library. 

## Documentation

Code is commented for the 'doxygen' API generation tool.
Documents on fonts, bitmaps and graphics can be found at 
the dependency 'display16_LTSM' repository, [URL github link](https://github.com/gavinlyonsrepo/display16_LTSM)

## Software

### User options

In the example files, there are 2 sections in "setup()" function 
where user can make adjustments. The constructor also takes a number of arguments.

 * User Option 0 constructor() Color and contrast settings
 * User Option 1 setup()       GPIO + SPI Settings
 * User Option 2 setup()       Screen Setup 

*User Option  Color and contrast settings*

Here the user can select RGB or BGR mode depending on type of display, They can also
set the color contrast levels for the 3 channels (A B and C) for normal and dim mode.
The range is 0-255. 

*User Option SPI Settings* 

Here the user can pass the SPI Bus freq in Hertz.
If users wants software SPI just call this method 
with just one argument for the optional GPIO software uS delay,
which by default is zero. Setting this higher can be used to slow down Software SPI 
which may be beneficial in some setups.
Set the five GPIO pins. If using Hardware SPI: clock and data pins will be tied to 
the chosen interface on MCU.

*User Option Screen size*

User can adjust screen pixel height, screen pixel width.

### Examples

There are 5 example files included.

| Filename .ino | Function  | Note |
| --- | --- | --- |
| HELLO WORLD | Hello world basic use case | --- |
| MISC| Text, Graphics & Functions | dislib16 ADVANCED GRAPHICS ENABLE must be enabled for all tests to work|
| BITMAP_FUNC | bitmaps + Functions tests | Bitmap test data is stored in arrays |
| DEMO| A demo | dislib16 ADVANCED GRAPHICS ENABLE must be enabled |
| FRAME BUFFER | Testing frame Buffer mode | dislib16 ADVANCED SCREEN BUFFER ENABLE must be enabled user option 2 |

## Hardware

Connections as setup in example  file.

| TFT PinNum | Pindesc |  HW SPI |  SW SPI |
| --- | --- | --- | --- |
| 1 | GND | GND | GND |
| 2 | VCC | VCC 3.3V | VCC 3.3V  |
| 3 | SCLK | MCU SPI CLK | GPIO12 |
| 4 | SDA | MCU SPI_MOSI | GPIO13 |
| 5 | RESET | GPI04 | GPIO4 |
| 6 | DC | GPIO5 | GPIO5 |
| 7 | CS | GPIO15  | GPIO15|

1. Pick any GPIO you want for SW SPI, for HW SPI SCLK and MOSI will be tied to MCU SPI interface pins.

[![Image SSD](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ssd1331.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ssd1331.jpg)

## Tested

Tested with both software and hardware SPI on:

- **ESP32**
- **Arduino UNO R4 Minima**  
 
Compiled only (not fully hardware-tested) on:

- **Arduino UNO**
- **ESP8266**
- **STM32 “Blue Pill”**
- **RP2040 PICO**

> Some examples on low-RAM MCUs will fail( insufficient memory ), if numerous fonts and bitmap data are included.  
> Frame buffer mode example requires sufficient dynamic memory for the buffer — see the README in display16_LTSM for details.

## Output

[![ demo pic 2](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ssd1331output.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ssd1331output.jpg)


