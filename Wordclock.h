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

#ifndef H_WORDCLOCK_H
#define H_WORDCLOCK_H

#include <Adafruit_WS2801.h>
#include "RTCWrapper.h"

/************************ Data structure definitions ***********************************/

/* struct Color
   This structure stores an RGB value. In addition, there are static expressions
   for hue values in HSV color space.
*/
struct Color
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  // HSV definitions
  static constexpr double HUE_RED_MIN = 0.0;
  static constexpr double HUE_RED_MAX = 1.0;
  static constexpr double HUE_GREEN = 0.333;
  static constexpr double HUE_BLUE = 0.666;

  // Constructors/Destructors
  Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
  Color() {}
  Color(Color& ref_color) : r(ref_color.r), g(ref_color.g), b(ref_color.b) {}
  ~Color() {}
};


/* struct clock_word
   This structure assigns pixels to a certain word in the clock.
   The pixel indices are stored in the array 'pixels'. The number of pixels
   defines how many pixels are used per word.
*/

// Maximum number of pixels per word. Adapt this definition if necessary.
#define MAX_LEDS_PER_WORD 2

struct clock_word
{
  uint8_t num_pixels;
  uint8_t pixels[MAX_LEDS_PER_WORD];
};

/* struct clockface
   This structure stores the words of the word clock.
*/
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

/****************************** Wordclock Class **************************************/


class Wordclock
{
    // Pin configuration
    uint8_t dpin = 3;
    uint8_t cpin = 2;
    // Adafruit Pixel Array
    Adafruit_WS2801 pixels;
    uint8_t num_pixels = 26;
    // RTC Wrapper
    RTCWrapper rtc_wrapper;
    // Clockface structure
    struct clockface clock_words;

    // Delays
    uint32_t update_delay = 1000;
    uint32_t test_delay = 1000;

    // Color modes - Data
    Color cur_color = {150,30,0}; 
    double color_rotation_factor = 1;
    /*********************** Rainbow mode parameters **********************************/
    uint16_t num_steps_rainbow = 100;
    uint16_t num_steps_rainbow_per_word = 40;
    double rainbow_hue_min = Color::HUE_RED_MIN;
    double rainbow_hue_max = Color::HUE_BLUE;    
    uint8_t mode = MODE_FIXED;
    
  public:
    /*************************** Mode definitions *************************************/
    static constexpr uint8_t MODE_FIXED = 0;
    static constexpr uint8_t MODE_RAINBOW = 1;
    static constexpr uint8_t MODE_RAINBOW_BOUNDED = 2;
    static constexpr uint8_t MODE_RAINBOW_EACH_WORD = 3;
    static constexpr uint8_t MODE_RAINBOW_EACH_WORD_BOUNDED = 4;
    
    /**************************** Initialization **************************************/
    Wordclock() {}

    /*
       This function initializes basic Wordclock functions.
       @param num_pixels: Number of pixels in clock face
       @param cpin: WS2801 Clock pin
       @param dpin: WS2801 Data pin
       @param struct clockface words: Clockface structure containing the clock words and the corresponding pixels
    */
    void begin(uint8_t num_pixels, uint8_t cpin, uint8_t dpin, struct clockface& words);

    /*
       This function updates the wordclock. The mode of the clock must be set beforehand with the function setMode.
       @param cur_hour: Current hour
       @param cur_min: Current minute
    */
    void updateWordClock(uint8_t cur_hour, uint8_t cur_min);
    /*
       This function updates the wordclock. The mode of the clock must be set beforehand with the function setMode.
    */
    void updateWordClock();

    /************************************ Configuration functions ************************************/

    /*
     * This function sets the test delay of the clock
     * @param test_delay: delay in ms
     */
    void setTestDelay(uint32_t test_delay);

    
    /*
     * This function sets the update delay of the clock.
     * @param update_delay: delay in ms
     */
    void setUpdateDelay(uint32_t update_delay);

    /*
     * This function sets the mode of the clock.
     * @param mode: mode definition 
     */
    void setMode(uint8_t mode);

    /* Set color of word clock. Only use this in mode MODE_FIXED.
     * @param color: color to set
     */
    void setColor(Color& color);

    /* Set minimum hue for rainbow clock. Used in modes RAINBOW_BOUNDED and RAINBOW_EACH_WORD_BOUNDED.
     *  @param hue_min: minimum hue, range [-1,1]
     */
    void setRainbowHueMin(double hue_min);
    
    /* Set maximum hue for rainbow clock. Used in modes RAINBOW_BOUNDED and RAINBOW_EACH_WORD_BOUNDED.
     *  @param hue_max: maximum hue
     */
    void setRainbowHueMax(double hue_max);
    
    /* Set number of color steps in rainbow. Used in all rainbow modes. 
     *  @param num_steps: number of steps
     */
    void setNumberOfRainbowSteps(uint16_t num_steps);
    
    /* Set number of color steps in rainbow for different words. Used modes RAINBOW_EACH_WORD and RAINBOW_EACH_WORD_BOUNDED.
     *  @param num_steps: number of steps
     */
    void setNumberOfRainbowStepsPerWord(uint16_t num_steps);

    /************************************** Test functions ***************************************/

    /*
       This function calls a selftest by setting all pixels to red, then green, then blue color.
       The delay between the color switches can be set by the param test_delay.
    */
    void RGB_selftest();

    /*
       This function tests the functionality of each pixel.
       The function iterates through all pixels of the word clock and sets each pixel
       to a red color for a time.
    */
    void pixelTest();

    /*
       This function tests the fucntionality of the time display on the clockface.
       The function iterates through all possible time values and sets the clockface accordingly.
    */
    void TimeTest();
  
    /*
     * 
     */

  /********************************** HELPER FUNCTIONS - NOT PUBLIC ******************************************/
  private:
    /********************************** Pixel configuration functions ****************************************/

    /*  This function sends the current clockface configuration to the clock.*/
    void updateClockface();

    /* This function deactivates all pixels. The clockface must then be updated by updateClockFace() */
    void switchAllPixelsOff();

    /*
       This function sets all pixels of the clock to a certain color. Configuration must be
       transmitted by updateClockface().
       @param r,g,b: Color definition in RGB.
    */
    void setAllPixelsToColor(uint8_t r, uint8_t g, uint8_t b);

    /*
       This function sets a certain word of the clockface to the specified color. To update the clock, updateClockFace()
       must be called.
       @param struct clock_word& word_to_set: Members of the clockface struct can be passed here
       @param r,g,b: Color definition in RGB
    */
    void setWord(struct clock_word& word_to_set, struct Color& c);

    /*
       This function sets a certain word of the clockface to the specified color. To update the clock, updateClockFace()
       must be called.
       @param word_to_set: Reference to member of the clockface struct can be passed here
       @param color: Color definition in Color struct
    */
    void setWord(struct clock_word& word_to_set, uint8_t r, uint8_t g, uint8_t b);

    /*
       Set set of pixels in the word clock to specified color. To update the clock, updateClockFace()
       must be called.
       @param pixel_numbers: Pointer to pixel indices - Length of array must be specified in parameter num_pixels_to_set
       @param num_pixels_to_set: Number of pixels
       @param r,g,b: Color definition in RGB
    */
    void setSetOfPixels(uint8_t *pixel_numbers, uint8_t num_pixels_to_set, uint8_t r, uint8_t g, uint8_t b);

    /****************************** Color update functions ****************************************/

    /*
       This function updates the current color based on the HSV color space.
       The hue of the color is increased, leading to a different color of the rainbow.
        => hue = 0: red color
        => hue = 0.33: green color
        => hue = 0.66: blue color
        => hue = 1: red color
       @param cur_min: Current color
       @param num_color_steps: This parameter defines the number of steps that are needed
        to go from hue 0 to hue 1
    */
    void updateHue(Color& cur_color, double num_color_steps);

    /*
       This function updates the current color based on the HSV color space.
       The hue of the color is increased, leading to a different color of the rainbow. The hue is
       bounded between 0 and 1. If the bound is reached, the hue is reduced, changing the color back into the original color.
       With this function, the clock can be set to switch color e.g. between red and green. The static members of the
       Color class can be used in this function.
       @param cur_min: Current color
       @param num_color_steps: This parameter defines the number of steps that are needed
        to go from hue 0 to hue 1
       @param hue_min: Minimum hue value, e.g. HUE_RED_MIN, HUE_GREEN, HUE_BLUE or a floating point value
       @param hue_max: Maximum hue value, e.g. HUE_GRREN, HUE_BLUE, HUE_RED_MAX or a floating point value
    */
    void updateHueBounded(Color& cur_color, double num_color_steps, double hue_min = 0, double hue_max = 1.0);

    /****************************** Time update functions ****************************************/

    /*
       This function updates the displayed time of the clockface. All words are shown
       in the same color or in different colors depending on the mode.
       @param cur_hour: Current hour
       @param cur_min: Current minute
       @param cur_color: Color of words
    */
    void updateTime(uint8_t cur_hour, uint8_t cur_min, Color& cur_color);    
};
#endif
