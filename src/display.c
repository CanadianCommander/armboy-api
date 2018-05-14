#include "display.h"
#include "util.h"
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
