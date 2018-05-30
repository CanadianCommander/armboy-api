#include "display.h"
#include "input.h"
#include "fs.h"
#include <string.h>

void main(void){

  loadDisplayDriver();
  initializeDisplayDefaults();

  loadInputDriver();
  initInputDefault();

  loadFileSystemDriver();
  initSDandFAT();

  FileDescriptor img;
  openFile("/BOOT/armboy_logo.data", &img);

  RGBBitmap bmp;
  bmp.width = 128;
  bmp.height = 106;
  uint8_t rgbBuff[128*128*2];
  memset(rgbBuff, 0, 128*128*2);
  bmp.rgbData = (uint16_t*)rgbBuff;
  readRGB24File(bmp.rgbData, 128*106*3, &img);

  ControlState cState;

  uint16_t color = 0x001F;

  Rec r1;
  r1.x = 100;
  r1.y = 50;
  r1.w = 40;
  r1.h = 40;
  float scale = 1.0f;
  for(;;){
    getControlState(&cState);

    if(cState.button1){
      FileDescriptor img2;
      openFile("/BOOT/eth_large.data", &img2);
      bmp.width = 128;
      bmp.height = 128;
      memset(rgbBuff, 0, 128*128*2);
      readRGB24File(bmp.rgbData, 128*128*3, &img2);
    }

    if(cState.button4){
      scale += 0.1f;
      for(int i =0; i < 1000000; i++){
        asm("");
      }
      clearDisplay(0x00);
    }

    if(cState.button2){
      if(scale >= 1.0f){
        scale -= 0.1f;
      }
      for(int i =0; i < 1000000; i++){
        asm("");
      }
      clearDisplay(0x00);
    }

    if(cState.dPadUp){
      r1.y -=1;
    }
    if(cState.dPadLeft){
      r1.x -=1;
    }
    if(cState.dPadRight){
      r1.x +=1;
    }
    if(cState.dPadDown){
      r1.y +=1;
    }

    drawBitmap(&bmp, r1.x, r1.y,  scale);


  }
}
