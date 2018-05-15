#include "display.h"
#include <string.h>

void main(void){
  loadDisplayDriver();
  initializeDisplayDefaults();

  int x = 2;
  int y = 2;
  int xInc = 1;
  int yInc = 1;

  Rec r1;
  r1.w = 40;
  r1.h = 40;
  for(;;){
    for(int i =0; i < 100000;i++){
      asm("");
    }

    x += xInc;
    y += yInc;
    if(x > 300 || x < -20){
      xInc = -xInc;
    }
    if(y > 220 || y < -20){
      yInc = -yInc;
    }

    // clear old rects
    if(xInc > 0){
      r1.x = x - xInc;
      r1.y = 100;
      r1.w = 1;
      drawRectangle(&r1,0x00);
      r1.w = 40;
    }
    else {
      r1.x = x - xInc + 39;
      r1.y = 100;
      r1.w = 1;
      drawRectangle(&r1,0x00);
      r1.w = 40;
    }

    if(yInc > 0){
      r1.x = 140;
      r1.y = y - yInc;
      r1.h = 1;
      drawRectangle(&r1,0x00);
      r1.h = 40;
    }
    else {
      r1.x = 140;
      r1.y = y - yInc + 39;
      r1.h = 1;
      drawRectangle(&r1,0x00);
      r1.h = 40;
    }

    //draw new rectangles
    r1.x = x;
    r1.y = 100;
    drawRectangle(&r1,0xF800);

    r1.x = 140;
    r1.y = y;
    drawRectangle(&r1,0x001F);


  }
}
