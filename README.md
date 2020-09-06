# Wordclock - RGB LED Wordclock library
This Arduino library is set to control a RGB LED wordclock, which was a college project of mine.
The clock is defined to work in 5-minute steps, time format is 0-12h. This clock uses a DS3231 RTC module for time measurement and
RGB LEDs controlled by the Adafruit WS2801 controller.

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

## Examples

### Clock and data pin
The Arduino communicates with the WS2801 pixels through a synchronous serial interface. This interface consists of a clock wire and a data wire. The corresponding pins of the Arduino must be defined and passed to the ```Wordclock::begin()``` function.

### Defining the clockface
Each word of the word clock is defined by an array of pixels and the number of pixels in the array. The array 'pixels' contains the addresses of the WS2801 pixels. 
```struct clock_word {
  uint8_t num_pixels;
  uint8_t pixels[MAX_LEDS_PER_WORD];
}
```
The clockface contains the following words:
```
struct clockface
{
  struct clock_word w_o_clock;
  struct clock_word w_to;
  struct clock_word w_past;
  struct clock_word w_five;
  struct clock_word w_minutes;
  struct clock_word w_twenty;
  struct clock_word w_quarter;
  struct clock_word w_itis;
  struct clock_word w_ten;
  struct clock_word w_half;
  // Hours
  struct clock_word hours[12];
};
```
#### Adapting the maximum number of pixels per word - if necessary
The wordclock library is defined to have a maximum of 2 LED pixels per word. The maximum number of pixels can be adapted in Wordclock.h:
```#define MAX_LEDS_PER_WORD <maximum number of LEDs>```

### Mode settings
The mode of the clock can be set by:
```w_clock.setMode(Wordclock::MODE_RAINBOW_EACH_WORD_BOUNDED);```
Possible modes are: ```MODE_RAINBOW, MODE_FIXED, MODE_RAINBOW_EACH_WORD, MODE_RAINBOW_BOUNDED, MODE_RAINBOW_EACH_WORD_BOUNDED```

The parameters of the modes can be adapted.

