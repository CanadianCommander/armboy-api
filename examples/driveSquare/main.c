#include "display.h"
#include "input.h"
#include <string.h>

void main(void){
  loadDisplayDriver();
  initializeDisplayDefaults();

  loadInputDriver();
  initInputDefault();

  ControlState cState;

  Rec r1;
  r1.x = 0;
  r1.y = 0;
  r1.w = 40;
  r1.h = 40;

  Rec clear;
  clear.x =0;
  clear.y =0;
  clear.w =42;
  clear.h =42;
  for(;;){
    getControlState(&cState);

    if(cState.dPadUp){
      clear.x = r1.x;
      clear.y = r1.y + 39;
      drawRectangle(&clear,0x0000);
      r1.y --;
    }
    if(cState.dPadLeft){
      clear.x = r1.x + 39;
      clear.y = r1.y;
      drawRectangle(&clear,0x0000);
      r1.x --;
    }
    if(cState.dPadRight){
      clear.x = r1.x - 39;
      clear.y = r1.y;
      drawRectangle(&clear,0x0000);
      r1.x ++;
    }
    if(cState.dPadDown){
      clear.x = r1.x;
      clear.y = r1.y - 39;
      drawRectangle(&clear,0x0000);
      r1.y ++;
    }

    for(int i =0; i < 100000;i++){
      asm("");
    }

    drawRectangle(&r1,0x001F);


  }
}
