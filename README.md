# Wordclock - RGB LED Wordclock library

This Arduino library is set to control a RGB LED wordclock, which was a college project of mine.
The clock is defined to work in 5-minute steps, time format is 0-12h. This clock uses a DS3231 RTC module for time measurement and
the Adafruit WS201 as LED controller.

## Color Modes

The library implements the following color modes:
- Fixed color mode (defined by RGB value)
- Rainbow Mode - All words in rainbow color
- Rainbow mode - Words in different colors
- Rainbow mode Bounded - All words Color transition between different colors, e.g. red to green. 
- Rainbow mode Bounded - Rainbow transitions between different colors for each word, e.g. red to green. 

## Files

The library contains the following files:
- Wordclock.h
- Wordclock.cpp
- main.ino - Example main file for Arduino

## Dependencies - Used Libraries

This library uses the HSV color space. For the conversion Robert Atkins' RGB Converter library is used.
https://github.com/ratkins/RGBConverter

The implementation uses the Adafruit WS2801 as wordclock pixels and the Adafruit_WS2801 Arduino library
https://github.com/adafruit/Adafruit-WS2801-Library

For RTC control, the RTClib is used.
https://github.com/NeiroNx/RTCLib

Big thanks to the creators of these libraries!

Sandra Wilfling
