#ifndef DISPLAY_H_
#define DISPLAY_H_
#include <stdint.h>
#include "fs.h"

//display driver id
#define DISPLAY_DRIVER_ID 1
//display functions
#define DISPLAY_FUNCTION_INIT           0
#define DISPLAY_FUNCTION_INIT_DEFAULT   1
#define DISPLAY_FUNCTION_CLEAR_DISPLAY  2
#define DISPLAY_FUNCTION_DRAW_BITMAP    3
#define DISPLAY_FUNCTION_DRAW_RECTANGLE 4
#define DISPLAY_FUNCTION_SET_CLIP       5
#define DISPLAY_FUNCTION_CLEAR_CLIP     6

typedef struct {
  uint16_t width;
  uint16_t height;
  uint16_t * rgbData;
} RGBBitmap;

typedef struct {
  RGBBitmap * fontBmp;
  uint16_t charW, charH;
  uint16_t charsPerLine;
  uint16_t countLines;
  bool alternating;
} Font;

typedef struct {
  int x;
  int y;
  uint16_t w;
  uint16_t h;
} Rec;

/**
  load display driver kernel module.
*/
void loadDisplayDriver();

/**
  initialize the display for drawing, using the default pin assignment.
*/
void initializeDisplayDefaults();

/**
  clear the entire display to color (ignores clipping region setting)
*/
void clearDisplay(uint16_t color);

/**
  draw bitmap at the specified x,y with the specified scale
*/
void drawBitmap(RGBBitmap * b, int x, int y, float scale);

/**

*/
void drawRectangle(Rec * r, uint16_t color);


void setClipRegion(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void setClipRegionRect(Rec * r);

void clearClipRegion();

/**
  draws text starting at x / y (top left) untile end of string.
  @return the x positon of the end of the string on the screen. (so you can draw more text)
*/
int drawText(Font * font, char * text, int x, int y, float scale);

/**
  read font from file. not buffers must have been already allocated with enough
  size to hold the font bitmap
  @param charsPerLine the number of characters per line in the bitmap
  @param charWidth / charHeight the width / height of a single character
  @param countLines the number of character lines in the font bmp
  @param alter, true means alternating characters on each line Ex. AGBHCIDJEKF instead of ABCDEFGHIJK (so bitmap fonts are in
    this format, god knows why)
**/
void loadFont(Font * f, uint16_t charsPerLine, uint16_t charWidth, uint16_t charHeight, uint16_t countLines, bool alter, FileDescriptor * fd);

/**
  read a 24 bit RGB file and convert it to 16 bit RGB and place it in rgb16Buffer.
  @param count number of bytes to read from file note buffer will have [count - (count / 3)] bytes in it
*/
void readRGB24File(uint16_t * rgb16Buffer, uint32_t count, FileDescriptor * fd);
#endif /*DISPLAY_H_*/
