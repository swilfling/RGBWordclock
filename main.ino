/*
   Wordclock.h - Wordclock library

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

#include <Adafruit_WS2801.h>
#include <avr/io.h>
#include <RTClib.h>
#include "./Wordclock.h"

// Comment this line to suppress debug output
//#define DEBUG_SERIAL

Wordclock w_clock;  
DS3231 rtc;

/*
 * Helper function: Print time through serial
 * @param now: DateTime struct cotaining current time
 */
void print_time(DateTime& now)
{
    Serial.print("Current Time: ");
    Serial.print(now.hour());
    Serial.print(":");
    Serial.print(now.minute());
    Serial.print(":");
    Serial.println(now.second());
}

/*
 * Helper function: Setup DS3231
 * Call this in setup ()
 */
void setup_DS3231(DS3231& rtc)
{
  // Setup: Start DS3231
  rtc.begin();
 // rtc.adjust(DateTime(__DATE__, __TIME__));
  // Optional - Set clock to compile time - Only use this when in connection with Arduino
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
}

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
  #ifdef DEBUG_SERIAL
    Serial.begin(9600);
  #endif
  
  // Setup: Start Wordclock, selftest 
  uint8_t dpin = 13;
  uint8_t cpin = 12;
  uint8_t num_pixels = 26;  

  w_clock.begin(num_pixels, cpin, dpin, clock_words);

  // Wordclock Selftests
  uint32_t test_delay = 1000;
  w_clock.RGB_selftest(test_delay);
  //w_clock.pixelTest(test_delay);
  //w_clock.TimeTest(test_delay);

  // Setup RTC
  setup_DS3231(rtc); 
  
  #ifdef DEBUG_SERIAL
    DateTime cur_time = rtc.now(); 
    print_time(cur_time);
  #endif
}

void loop() {
  // Get time from RTC
  DateTime curtime = rtc.now();
 
  // Debug Output
  #ifdef DEBUG_SERIAL
    print_time(curtime);
  #endif
  
  uint8_t cur_minute = curtime.minute();
  uint8_t cur_hour = curtime.hour();
  
  // Set mode
  w_clock.setMode(Wordclock::MODE_RAINBOW_EACH_WORD_BOUNDED);
  w_clock.setRainbowHueMin(Color::HUE_RED_MIN);
  w_clock.setRainbowHueMax(Color::HUE_GREEN);
  // Update Wordclock
  w_clock.updateWordClock(cur_hour,cur_minute);
  delay(1000);
}
