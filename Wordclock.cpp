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

#include "Wordclock.h"
#include "RGBConverter.h"

/* 
 * This function initializes basic Wordclock functions. 
 * @param num_pixels: Number of pixels in clock face
 * @param cpin: WS2801 Clock pin
 * @param dpin: WS2801 Data pin
 * @param struct clockface words: Clockface structure containing the clock words and the corresponding pixels
 */
void Wordclock::begin(uint8_t num_pixels, uint8_t cpin, uint8_t dpin, struct clockface& words)
{    
  this->dpin = dpin;
  this->cpin = cpin;
  this->num_pixels = num_pixels;
  this->clock_words = words;
  pixels = Adafruit_WS2801(num_pixels,dpin,cpin);
  pixels.begin();
}

/* This function deactivates all pixels. The clockface must then be updated by updateClockFace()  */
void Wordclock::switchAllPixelsOff() 
{
  int i = 0;
  for (i=0;i<num_pixels;i++)
    pixels.setPixelColor(i, 0,  0, 0);
}

/* This function sends the current clockface configuration to the clock. */
void Wordclock::updateClockface()
{
  pixels.show();
}

/*
 * This function sets all pixels of the clock to a certain color. Configuration must be 
 * transmitted by updateClockface().
 * @param r,g,b: Color definition in RGB.
 */
void Wordclock::setAllPixelsToColor(uint8_t r, uint8_t g, uint8_t b) 
{
  int i = 0;
  for (i=0;i<num_pixels;i++)
    pixels.setPixelColor(i, r, g, b);
}

/*
 * This function sets a certain word of the clockface to the specified color.
 * @param struct clock_word& word_to_set: Members of the clockface struct can be passed here
 * @param r,g,b: Color definition in RGB
 */
void Wordclock::setWord(struct clock_word& word_to_set, uint8_t r, uint8_t g, uint8_t b)
{
  int i = 0;
  for (i=0;i<word_to_set.num_pixels;i++)
  {
    uint8_t current_pixel = word_to_set.pixels[i];
    pixels.setPixelColor(current_pixel, r, g, b);
  }
}

/*
 * This function sets a certain word of the clockface to the specified color.
 * @param word_to_set: Reference to member of the clockface struct can be passed here
 * @param color: Color definition in Color struct
 */
void Wordclock::setWord(struct clock_word& word_to_set, struct Color& cur_color)
{
  int i = 0;
  for (i=0;i<word_to_set.num_pixels;i++)
  {
    uint8_t current_pixel = word_to_set.pixels[i];
    pixels.setPixelColor(current_pixel, cur_color.r, cur_color.g, cur_color.b);
  }
}

/*
 * Set set of pixels in the word clock to specified color
 * @param pixel_numbers: Pointer to pixel indices - Length of array must be specified in parameter num_pixels_to_set
 * @param num_pixels_to_set: Number of pixels
 * @param r,g,b: Color definition in RGB
 */  
void Wordclock::setSetOfPixels(uint8_t *pixel_numbers,uint8_t num_pixels_to_set, uint8_t r, uint8_t g, uint8_t b)
{
  int i = 0;
  for (i=0;i<num_pixels_to_set;i++)
  {
    uint8_t current_pixel = pixel_numbers[i];
    pixels.setPixelColor(current_pixel, r, g, b);
  }
}

/*
 * This function calls a selftest by setting all pixels to red, then green, then blue color. 
 * The delay between the color switches can be set by the param test_delay.
 * @param test_delay: Delay in microseconds
*/
void Wordclock::RGB_selftest(uint32_t test_delay)
{
  // Test 1: All Pixels RED
  setAllPixelsToColor(10,0,0);
  pixels.show();
  delay(test_delay);
  
  // Test 2: All Pixels GREEN
  setAllPixelsToColor(0,10,0);
  pixels.show();
  delay(test_delay);

  // Test 3: All Pixels BLUE  
  setAllPixelsToColor(0,0,10);
  pixels.show();
  delay(test_delay);
}

/*
 * This function tests the fucntionality of the clockface.
 * The function iterates through all possible time values and sets the clockface accordingly.
 * @param delay: delay between time steps
 */
void Wordclock::TimeTest(uint32_t test_delay)
{
  Color cur_color = {255,0,0};
  uint32_t cur_hour = 0, cur_min = 0;
  for(cur_hour = 0; cur_hour < 24; cur_hour++)
  {
    for(cur_min = 0; cur_min < 60; cur_min++)
    {
      updateTime(cur_hour, cur_min, cur_color);
      delay(test_delay);
    }
  }
}

/*
 * This function tests the functionality of each pixel.
 * The function iterates through all pixels of the word clock and sets each pixel 
 * to a red color for a time. 
 * @param test_delay: delay in microseconds
 */
void Wordclock::pixelTest(uint32_t test_delay)
{
  // Test 4: Loop through all pixels of the word clock - NOT IN ORDER OF WORDS!
  int j = 0;
  for (j = num_pixels - 1; j >= 0 ; j--)
  {
    // Switch all pixels off
    switchAllPixelsOff();
    // Set all pixels to red color
    pixels.setPixelColor(j,255,0,0);
          
    pixels.show();
    delay(test_delay);
  }
}

/*
 * This function updates the displayed time of the clockface. All words are shown
 * in the same color.
 * @param cur_hour: Current hour
 * @param cur_min: Current minute
 * @param cur_color: Color of words
*/
void Wordclock::updateTime(uint8_t cur_hour, uint8_t cur_min, Color& cur_color)
{     
  switchAllPixelsOff();
  
  setWord(clock_words.w_itis,cur_color);
  
  // five, ten, quarter, twenty, half    
  if(cur_min < 5)
      setWord(clock_words.w_o_clock,cur_color);
  if ((cur_min >= 5 && cur_min < 10) || (cur_min >= 55 && cur_min <= 59))
      setWord(clock_words.w_five,cur_color);
  if ((cur_min >= 10 && cur_min < 15) || (cur_min >= 50 && cur_min < 55))
      setWord(clock_words.w_ten,cur_color);
  if ((cur_min >= 15 && cur_min < 20) ||(cur_min >= 45 && cur_min < 50))
    setWord(clock_words.w_quarter,cur_color);
  if ((cur_min >= 20 && cur_min < 25) || (cur_min >= 40 && cur_min < 45))
    setWord(clock_words.w_twenty,cur_color);
  if ((cur_min >= 25 && cur_min < 30) || (cur_min >= 35 && cur_min < 40))
  {
    setWord(clock_words.w_twenty,cur_color);
    setWord(clock_words.w_five,cur_color);
  }
  if(cur_min >= 30 && cur_min < 35)     
    setWord(clock_words.w_half,cur_color);    
  
  // Minutes?
  if((cur_min >= 5 && cur_min < 15) || (cur_min >= 20 && cur_min < 30) 
    || (cur_min >= 35 && cur_min < 45) || (cur_min >= 50 && cur_min <= 59))    
    setWord(clock_words.w_minutes,cur_color);
  
  uint8_t hour_to_show = cur_hour;
  // Past or to?
  if (cur_min >= 35 && cur_min <= 59)
  {
    hour_to_show++;
    setWord(clock_words.w_to,cur_color);
  }
  else if(cur_min >= 5 && cur_min < 35)
    setWord(clock_words.w_past,cur_color);
  
  // Set hour
  hour_to_show = hour_to_show % 12;
  setWord(clock_words.hours[hour_to_show],cur_color);
  pixels.show();
}

/*
 * This function updates the displayed time of the clockface. The words are shown in different
 * colors based on the rainbow. The hue changed for each word by a factor based on num_steps_update.
 * @param cur_hour: Current hour
 * @param cur_min: Current minute
 * @param cur_color: Color of first word
 * @param num_steps_update: This parameter defines the number of steps in which the rainbow is split up.
*/
void Wordclock::updateTimeAndColor(uint8_t cur_hour, uint8_t cur_min, Color& cur_color, uint8_t num_steps_update)
{
  Color new_color;
  new_color.r = cur_color.r;
  new_color.g = cur_color.g;
  new_color.b = cur_color.b;
  
  switchAllPixelsOff();
  
  setWord(clock_words.w_itis,cur_color);

  if(cur_min >= 5)
    updateHue(new_color,num_steps_update);
  
  // Check to set words 'five', 'ten', 'quarter', 'twenty', 'twenty-five', 'half'
  if ((cur_min >= 5 && cur_min < 10) || (cur_min >= 55 && cur_min <= 59))
    setWord(clock_words.w_five,new_color);
  if ((cur_min >= 10 && cur_min < 15) || (cur_min >= 50 && cur_min < 55))
      setWord(clock_words.w_ten,new_color);
  if ((cur_min >= 15 && cur_min < 20) ||(cur_min >= 45 && cur_min < 50))
    setWord(clock_words.w_quarter,new_color);
  if ((cur_min >= 20 && cur_min < 30) || (cur_min >= 35 && cur_min < 45))
    setWord(clock_words.w_twenty,new_color);
  if ((cur_min >= 25 && cur_min < 30) || (cur_min >= 35 && cur_min < 40))
    setWord(clock_words.w_five,new_color);
  if(cur_min >= 30 && cur_min < 35)     
    setWord(clock_words.w_half,new_color);    

  // Set word 'minutes'?
  if((cur_min >= 5 && cur_min < 15) || (cur_min >= 20 && cur_min < 30) 
    || (cur_min >= 35 && cur_min < 45) || (cur_min >= 50 && cur_min <= 59))    
  {
    updateHue(new_color,num_steps_update);
    setWord(clock_words.w_minutes,new_color);
  }
  
  uint8_t hour_to_show = cur_hour % 12;
  // Set word 'past' or 'to'?
  if (cur_min >= 35 && cur_min <= 59)
  {
    hour_to_show++;  
    updateHue(new_color,num_steps_update);
    setWord(clock_words.w_to,new_color);
  }
  else if(cur_min >= 5 && cur_min < 35)
  {
    updateHue(new_color,num_steps_update);
    setWord(clock_words.w_past,new_color);
  }  
  
  // Update hue and set hour
  updateHue(new_color,num_steps_update);
  hour_to_show = hour_to_show % 12;
  setWord(clock_words.hours[hour_to_show],new_color);
  
  // Set word 'o'clock'?
  if(cur_min < 5)
  {
    updateHue(new_color,num_steps_update);
    setWord(clock_words.w_o_clock,new_color);
  }
  pixels.show();
}

/*
 * This function updates the current color based on the HSV color space.
 * The hue of the color is increased, leading to a different color of the rainbow. 
 *  => hue = 0: red color
 *  => hue = 0.33: green color
 *  => hue = 0.66: blue color
 *  => hue = 1: red color
 * @param cur_min: Current color
 * @param num_color_steps: This parameter defines the number of steps that are needed 
 *  to go from hue 0 to hue 1
*/
void Wordclock::updateHue(Color& cur_color, double num_color_steps)
{
  RGBConverter conv;
  // Convert to HSV
  double hsv_value[3];
  conv.rgbToHsv(cur_color.r, cur_color.g, cur_color.b, hsv_value);

  double hue = hsv_value[0];
  // Calculate increment of hue
  double step_factor = 1/num_color_steps;
  // Increase/Decrease Hue
  hue += step_factor;
  
  uint8_t rgb_value[3];
  // Convert back to RGB
  conv.hsvToRgb(hue, hsv_value[1], hsv_value[2], rgb_value); 
  cur_color.r = rgb_value[0];
  cur_color.g = rgb_value[1];
  cur_color.b = rgb_value[2];
}
  
/*
 * This function updates the current color based on the HSV color space. 
 * The hue of the color is increased, leading to a different color of the rainbow. The hue is
 * bounded between 0 and 1. If the bound is reached, the hue is reduced, changing the color back into the original color.
 * With this function, the clock can be set to switch color e.g. between red and green. The static members of the 
 * Color class can be used in this function.
 * @param cur_min: Current color
 * @param num_color_steps: This parameter defines the number of steps that are needed 
 *  to go from hue 0 to hue 1
 * @param hue_min: Minimum hue value, e.g. HUE_RED, HUE_GREEN, HUE_BLUE or a floating point value
 * @param hue_max: Maximum hue value, e.g. HUE_GRREN, HUE_BLUE, HUE_RED_MAX or a floating point value
*/
void Wordclock::updateHueBounded(Color& cur_color, double num_color_steps, double hue_min = 0, double hue_max = 1.0)
{
  RGBConverter conv;
  double hsv_value[3];
  // Convert RGB to HSV
  conv.rgbToHsv(cur_color.r, cur_color.g, cur_color.b, hsv_value);
  // Calculate factor to increase hue
  double step_factor = 1.0/num_color_steps;
  
  double hue = hsv_value[0];
  // Check bounds of hue - select whether to increase or decrease hue
  if(hue + step_factor >= hue_max)
  {
    // Hue must be decreased
    color_rotation_factor = -1; 
  }
  else if(hue - step_factor <= hue_min)
  {
    // Hue must be increased
    color_rotation_factor = 1;
  }
  Serial.println("Color rotation:");
  Serial.println(color_rotation_factor);  
  // Increase/Decrease Hue
  hue += (color_rotation_factor * step_factor);
  
  uint8_t rgb_value[3];
  // Convert back to RGB
  conv.hsvToRgb(hue, hsv_value[1], hsv_value[2], rgb_value); 
  cur_color.r = rgb_value[0];
  cur_color.g = rgb_value[1];
  cur_color.b = rgb_value[2];
}

/*
 * This function sets the mode of the clock.
 * @param mode: mode definition 
 */
void Wordclock::setMode(uint8_t mode)
{
  if(mode <= MODE_RAINBOW_EACH_WORD_BOUNDED && mode >= MODE_FIXED)
    this->mode = mode;
}

/* Set number of color steps in rainbow. Used in all rainbow modes. 
 *  @param num_steps: number of steps
 */
void Wordclock::setNumberOfRainbowSteps(uint16_t num_steps)
{
  num_steps_rainbow = num_steps;
}

/* Set minimum hue for rainbow clock . Used in modes RAINBOW_BOUNDED and RAINBOW_EACH_WORD_BOUNDED.
 *  @param hue_min: minimum hue range [-1,1]
 */
void Wordclock::setRainbowHueMin(double hue_min)
{
  if( abs(hue_min) <= 1)
    rainbow_hue_min = hue_min;
}

/* Set maximum hue for rainbow clock . Used in modes RAINBOW_BOUNDED and RAINBOW_EACH_WORD_BOUNDED.
 *  @param hue_min: maximum hue range [-1,1]
 */
void Wordclock::setRainbowHueMax(double hue_max)
{
  if( abs(hue_max) <= 1)
    rainbow_hue_max = hue_max;
}

/* Set number of color steps in rainbow for different words. Used modes RAINBOW_EACH_WORD and RAINBOW_EACH_WORD_BOUNDED.
 *  @param num_steps: number of steps
 */
void Wordclock::setNumberOfRainbowStepsPerWord(uint16_t num_steps)
{
  num_steps_rainbow_per_word = num_steps;
}

/* Set color of word clock. Only use this in mode MODE_FIXED.
 * @param color: color to set
 */
void Wordclock::setColor(Color& color)
{
  cur_color = color;
}

/*
   This function updates the wordclock. The mode of the clock must be set beforehand with the function setMode.
   @param cur_hour: Current hour
   @param cur_min: Current minute
*/
void Wordclock::updateWordClock(uint8_t cur_hour, uint8_t cur_minute)
{
  // Check time for errors
  if(cur_hour < 24 && cur_minute < 60)
  {
    // Check wordclock modes
    if(mode == Wordclock::MODE_RAINBOW)
    {
      updateHue(cur_color, num_steps_rainbow);
      updateTime(cur_hour, cur_minute, cur_color);
    }
    else if(mode == Wordclock::MODE_RAINBOW_BOUNDED)
    {
      updateHueBounded(cur_color, num_steps_rainbow, rainbow_hue_min, rainbow_hue_max);
      updateTime(cur_hour,cur_minute, cur_color);
    }
    else if(mode == Wordclock::MODE_RAINBOW_EACH_WORD)
    {
      updateHue(cur_color, num_steps_rainbow);
      updateTimeAndColor(cur_hour,cur_minute,cur_color,num_steps_rainbow_per_word);
    }
    else if(mode == Wordclock::MODE_RAINBOW_EACH_WORD_BOUNDED)
    {
      updateHueBounded(cur_color, num_steps_rainbow, rainbow_hue_min, rainbow_hue_max);
      updateTimeAndColor(cur_hour,cur_minute,cur_color,num_steps_rainbow_per_word);
    }
    else // Fixed Color mode
       updateTime(cur_hour, cur_minute, cur_color);    
  }
}