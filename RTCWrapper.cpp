/*
   RTCWrpapper.cpp - Wordclock library

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
#include "RTCWrapper.h"

/*
 * Helper function: Print time through serial
 * @param curtime: DateTime struct cotaining current time
 */
void RTCWrapper::print_time(DateTime& curtime)
{
#ifdef DEBUG_SERIAL
    Serial.print("Current Time: ");
    Serial.print(curtime.hour());
    Serial.print(":");
    Serial.print(curtime.minute());
    Serial.print(":");
    Serial.println(curtime.second());
#endif
}

/*
 * Helper function: Print time through serial
 * @param now: DateTime struct cotaining current time
 */
void RTCWrapper::print_time()
{
#ifdef DEBUG_SERIAL
    DateTime curtime = rtc.now();
    Serial.print("Current Time: ");
    Serial.print(curtime.hour());
    Serial.print(":");
    Serial.print(curtime.minute());
    Serial.print(":");
    Serial.println(curtime.second());
#endif
}

/*
 * Helper function: Setup DS3231
 * Call this in setup ()
 * Adjust this function to include different RTC modules
 */
void RTCWrapper::begin()
{

#ifdef DEBUG_SERIAL
  Serial.begin(9600);
#endif
  // Setup: Start DS3231
  rtc.begin();
  // Optional - Set clock to compile time - Only use this when in connection with Arduino
  if (!rtc.isrunning()) {
    
#ifdef DEBUG_SERIAL
    Serial.println("RTC is NOT running!");
#endif
    setCurrentTime();
  }
}

/* 
*  Helper function: Set current time of RTC to sketch compile time.
*/
void RTCWrapper::setCurrentTime() 
{ 
  rtc.adjust(DateTime(__DATE__, __TIME__)); 
#ifdef DEBUG_SERIAL  
  Serial.print("Compile time: ");
  Serial.println(F(__TIME__));
#endif
}
