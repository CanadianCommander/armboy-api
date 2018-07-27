#include "display.h"
#include "core.h"
#include "util.h"
#include <memory.h>
#include "../drivers/ssd1289/src/ssd1289SysCall.h"

void loadDisplayDriver(){
  LOAD_KERNEL_MOD(DISPLAY_DRIVER_ID);
}

bool isDisplayDriverLoaded(){
  return isLoaded(DISPLAY_DRIVER_ID);
}

void initializeDisplayDefaults(){
  SYS_CALL(DISPLAY_DRIVER_ID, DISPLAY_FUNCTION_INIT_DEFAULT, NULL);
}

void clearDisplay(uint16_t color){
  SYS_CALL(DISPLAY_DRIVER_ID, DISPLAY_FUNCTION_CLEAR_DISPLAY, &color);
}

void drawBitmap(RGBBitmap * b, int x, int y, float scale){
  Bitmap driverBitmap;
  driverBitmap.w = b->width;
  driverBitmap.h = b->height;
  driverBitmap.rgb = b->rgbData;

  BitmapDraw driverArg;
  driverArg.b = &driverBitmap;
  driverArg.x = x;
  driverArg.y = y;
  driverArg.scale = scale;
  driverArg.mode = BITMAP_DRAW_MODE_NN;

  SYS_CALL(DISPLAY_DRIVER_ID,DISPLAY_FUNCTION_DRAW_BITMAP,&driverArg);
}

void drawRectangle(Rec * r, uint16_t color){
  RectangleDraw driverArg;
  Rectangle rec;
  rec.x = r->x;
  rec.y = r->y;
  rec.w = r->w;
  rec.h = r->h;
  driverArg.r = &rec;
  driverArg.color = color;

  SYS_CALL(DISPLAY_DRIVER_ID,DISPLAY_FUNCTION_DRAW_RECTANGLE,&driverArg);
}

void setClipRegion(uint16_t x, uint16_t y, uint16_t w, uint16_t h){
  SetClipRegionCall scrc;
  scrc.x = x;
  scrc.y = y;
  scrc.w = w;
  scrc.h = h;
  SYS_CALL(DISPLAY_DRIVER_ID, DISPLAY_FUNCTION_SET_CLIP, &scrc);
}

void setClipRegionRect(Rec * r){
  setClipRegion(r->x, r->y, r->w, r->h);
}

void clearClipRegion(){
  SYS_CALL(DISPLAY_DRIVER_ID, DISPLAY_FUNCTION_CLEAR_CLIP, NULL);
}


static void __drawChar(Font * font, char c, int x, int y, float scale);
static char __doCharacterConversion(char c);
int drawText(Font * font, char * text, int x, int y, float scale){
  int currX = x;
  for(int i =0; i < strlen(text); i++){
    __drawChar(font, __doCharacterConversion(text[i]), currX, y, scale);
    currX += font->charW*scale;
  }
  return currX;
}

static char __doCharacterConversion(char c){
  char character = c;
  if(character >= 65 && character <= 90){
    // capital character
    character -= 65;
  }
  else if(character >= 97 && character <= 122){
    //character
    character -= 97;
  }
  else if (character >= 49 && character <= 57){
    //number
    character -= 22;
  }
  else if (character == 48){
    //font bitmap goes "1234567890" so zero has special sauce!
    character -= 12;
  }
  else if (character == 45){
    // dash
    character -= 19;
  }
  else if(character == 32){
    //space (make character invalid)
    character = 0xFF;
  }

  return character;
}

static void __drawChar(Font * font, char c, int x, int y, float scale){
  if(c >= font->charsPerLine*font->countLines){
    //out of range
    return;
  }

  int maskX, maskY;
  if(font->alternating){
    if((c % font->charsPerLine) >= font->charsPerLine/2){
      maskX = font->charW + ((int)(c)
       % (font->charsPerLine/2))*font->charW*2;
    }
    else{
      maskX = ((int)(c) % (font->charsPerLine/2))*font->charW*2;
    }

    maskY = ((int)(c) / font->charsPerLine)*font->charH;
  }
  else{
    maskX = ((int)(c) % font->charsPerLine)*font->charW;
    maskY = ((int)(c) / font->charsPerLine)*font->charH;
  }

  Rec clipRec;
  clipRec.x = x;
  clipRec.y = y;
  clipRec.w = font->charW*scale;
  clipRec.h = font->charH*scale;
  setClipRegionRect(&clipRec);

  drawBitmap(font->fontBmp, x - maskX*scale, y - maskY*scale, scale);

}


void loadFont(Font * f, uint16_t charsPerLine, uint16_t charWidth, uint16_t charHeight, uint16_t countLines,
                bool alter, FileDescriptor * fd){

  readRGB24File(f->fontBmp->rgbData, (charsPerLine*charWidth*charHeight*countLines)*3, fd);
  f->fontBmp->width = charWidth*charsPerLine;
  f->fontBmp->height = charHeight*countLines;

  f->charW = charWidth;
  f->charH = charHeight;
  f->charsPerLine = charsPerLine;
  f->countLines = countLines;
  f->alternating = alter;
}

#define RGB24_CONVERT_BUFFER 510
static uint16_t __Rgb24toRgb16(uint8_t r, uint8_t g, uint8_t b);
void readRGB24File(uint16_t * rgb16Buffer, uint32_t count, FileDescriptor * fd){
  uint8_t conBuffer[RGB24_CONVERT_BUFFER];
  uint32_t rgb16Index = 0;

  while(count > 0){
    memset(conBuffer, 0, RGB24_CONVERT_BUFFER);
    uint32_t read = readFile(conBuffer, RGB24_CONVERT_BUFFER, fd);
    if(read == 0)break;
    count -= read;

    for(uint32_t i =0; i < read - 2; i+=3){
      rgb16Buffer[rgb16Index] = __Rgb24toRgb16(*(conBuffer + i), *(conBuffer + i + 1), *(conBuffer + i + 2));
      rgb16Index++;
    }
  }
}

static uint16_t __Rgb24toRgb16(uint8_t r, uint8_t g, uint8_t b){
  return ((uint16_t)((31.0f/255.0f)*r) << 11) | (0x07E0 & ((uint16_t)((63.0f/255.0f)*g) << 5)) | (0x001F & (uint16_t)((31.0f/255.0f)*b));
}
