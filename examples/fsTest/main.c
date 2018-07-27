#include "display.h"
#include "input.h"
#include "fs.h"
#include <string.h>

#define BLOCK_STRIP_W 76
#define BLOCK_STRIP_H 10

void main(void){
  loadFileSystemDriver();
  initSDandFAT();

  loadDisplayDriver();
  initializeDisplayDefaults();


  /** --- load IMAGE ---- **/
  FileDescriptor blockStripFile;
  RGBBitmap blockSpriteStrip;
  uint8_t blockSpriteImgData[BLOCK_STRIP_W*BLOCK_STRIP_H*2];
  blockSpriteStrip.width = BLOCK_STRIP_W;
  blockSpriteStrip.height = BLOCK_STRIP_H;
  blockSpriteStrip.rgbData = (uint16_t*)blockSpriteImgData;

  if(openFile("/blocks.data", &blockStripFile)){
    readRGB24File(blockSpriteStrip.rgbData, BLOCK_STRIP_W*BLOCK_STRIP_H*3, &blockStripFile);
    drawBitmap(&blockSpriteStrip, 100, 100, 1.0f);
  }
  else{
    //the magic error box
    Rec r1;
    r1.x = 0;
    r1.y = 0;
    r1.w = 40;
    r1.h = 40;
    drawRectangle(&r1,0xF800);
  }


  /** --- load system font ---- **/
  FileDescriptor fontFile;
  Font sysFont;
  RGBBitmap sysFontBmp;
  uint8_t fontData[96*33*2];
  sysFont.fontBmp = &sysFontBmp;
  sysFont.fontBmp->rgbData = (uint16_t *)fontData;

  if(openFile("/sysfonts/8x8font.data", &fontFile)){
    loadFont(&sysFont, 12, 8, 8, 4, true, &fontFile);
    drawText(&sysFont, "HELLO WORLD???" , 100,0, 1.0f);
  }
  else{
    //the magic error box
    Rec r1;
    r1.x = 0;
    r1.y = 0;
    r1.w = 40;
    r1.h = 40;
    drawRectangle(&r1,0xF800);
  }


}
