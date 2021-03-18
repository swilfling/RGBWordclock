/*
   RTCWrapper - Wordclock library

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

#ifndef H_RTCWRAPPER_H
#define H_RTCWRAPPER_H

#include <RTClib.h>


// Comment this line to suppress debug output
#define DEBUG_SERIAL

/************************ Data structure definitions ***********************************/

/* This class is a wrapper for the RTC used in the wordclock. If neccessary, adjust 
 *  functions here to use different RTC modules.
 */

class RTCWrapper
{
  private:
    DS3231 rtc;
  public:
  
    /* Helper function: Return current time   */
    DateTime now() { return rtc.now(); } 
      
    /*
    * Helper function: Setup DS3231
    * Call this in setup ()
    * Adjust this function to include different RTC modules
    */
    void begin();
      
    /*
    * Helper function: Print time through serial
    * @param now: DateTime struct cotaining current time
    */
    void print_time();
    
    /*
    * Helper function: Print time through serial
    * @param curtime: DateTime struct cotaining current time
    */
    void print_time(DateTime& curtime);
    
    /* 
    *  Helper function: Set current time of RTC to sketch compile time.
    */
    void setCurrentTime();
};


#endif
