/*
   main.ino - Wordclock library

   This Arduino library is set to control a RGB  LED wordclock. The clock is defined to work in
   5-minute steps, time format is 0-12h. The clock uses a DS3231 RTC module for time measurement and
   the Adafruit WS2801 as LED controller.

   The library implements the following color modes:
    - Fixed color mode (defined by RGB value)
    - Rainbow Mode - All words in fixed color
    - Rainbow mode - Words in different colors
    - Rainbow mode Bounded - Color transition between different colors as bounds

   This library uses the HSV color space. For the conversion Robert Atkins' RGB Converter library is used.
   https://github.com/ratkins/RGBConverter

   The implementation uses the Adafruit WS2801 as wordclock pixels and the Adafruit_WS2801 Arduino library
   https://github.com/adafruit/Adafruit-WS2801-Library

   For RTC control, the RTClib is used.
   https://github.com/NeiroNx/RTCLib
   
   Big thanks to the creators of these libraries!

   Sandra Wilfling
   Github:
   Instructables:

*/

#include <avr/io.h>
#include "Wordclock.h"

Wordclock w_clock;  

// Define clockface
struct clockface clock_words = {
  // Word       Number of pixels   Pixel addresses
   .w_o_clock = {   1,                   {0}    },
   .w_to =      {   1,                   {14}   },
   .w_past =    {   1,                   {15}   },
   .w_five =    {   1,                   {16}   },
   .w_minutes = {   2,                  {17,18} },
   .w_twenty =  {   2,                  {19,20} },
   .w_quarter = {   2,                  {21,22} },
   .w_itis =    {   1,                   {23}   },
   .w_ten =     {   1,                   {24}   },
   .w_half =    {   1,                   {25}   },
   // Hours
   .hours = 
   {
                {   2,                  {1,2}   },  //TWELVE
                {   1,                   {13}   },  //ONE
                {   1,                   {12}   },  //TWO
                {   1,                   {9}    },  //THREE
                {   1,                   {10}   },  //FOUR
                {   1,                   {11}   },  //FIVE
                {   1,                   {8}    },  //SIX
                {   1,                   {7}    },  //SEVEN
                {   1,                   {6}    },  //EIGHT
                {   1,                   {3}    },  //NINE
                {   1,                   {4}    },  //TEN
                {   1,                   {5}    }   //ELEVEN
   }
};
void setup() {  
  // Setup: Start Wordclock, selftest 
  uint8_t dpin = 13;
  uint8_t cpin = 12;
  uint8_t num_pixels = 26;  

  // Init wordclock
  w_clock.begin(num_pixels, cpin, dpin, clock_words);
  
  // Wordclock Selftests
  w_clock.setTestDelay(1000);
  w_clock.RGB_selftest();
  //w_clock.pixelTest();
  //w_clock.TimeTest();
  
  // Set mode
  w_clock.setUpdateDelay(1000);
  w_clock.setMode(Wordclock::MODE_RAINBOW_EACH_WORD_BOUNDED);
  // Set parameters
  w_clock.setNumberOfRainbowSteps(30);
  w_clock.setRainbowHueMin(Color::HUE_BLUE);
  w_clock.setRainbowHueMax(Color::HUE_GREEN);
}  

void loop() {
  
  // Update Wordclock
  w_clock.updateWordClock();
}
