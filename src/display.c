#include "display.h"
#include "util.h"
#include <memory.h>
#include "../drivers/ssd1289/src/ssd1289SysCall.h"

void loadDisplayDriver(){
  LOAD_KERNEL_MOD(DISPLAY_DRIVER_ID);
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
