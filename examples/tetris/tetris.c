#include "display.h"
#include "input.h"
#include "fs.h"
#include "core.h"
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>

/** BLOCKS **/
#define BLOCK_NONE    0
#define BLOCK_YELLOW  1
#define BLOCK_BLUE    2
#define BLOCK_ORANGE  3
#define BLOCK_RED     4
#define BLOCK_PURPLE  5
#define BLOCK_GREEN   6
#define BLOCK_PINK    7

/** PEICES **/
#define PEICE_COUNT 7
#define BLOCKS_PER_SHAPE 4

/** block groups that spawn at top of game board **/
#define BLOCK_CUBE      0
#define BLOCK_L_LEFT    1
#define BLOCK_L_RIGHT   2
#define BLOCK_LINE      3
#define BLOCK_TRIANGLE  4
#define BLOCK_ZIG_LEFT  5
#define BLOCK_ZIG_RIGHT 6

/** PLAY FIELD **/
#define PLAY_AREA_UNIT_W_H 12 // cell size
#define PLAY_AREA_W 10 // width in cells
#define PLAY_AREA_H 20 // height in cells
#define PLAY_AREA_X_OFFSET 90;// not really centered on screen but "looks" more centered due to screen physical shape
#define PLAY_AREA_Y_OFFSET 0;

/** block sprite strip (blocks.raw) **/
#define BLOCK_STRIP_W 76
#define BLOCK_STRIP_H 10
#define BLOCK_W_H 10
#define BLOCK_SEPARATION 1
#define BLOCK_SCALE 1.2f

typedef struct {
  uint16_t activeBlocks[BLOCKS_PER_SHAPE];
  uint8_t activeBlockType;
  bool isActive;
} GameState;

void draw(RGBBitmap * blockSheet, uint8_t * playArea);
void logicInit(GameState * gs);
void update(RGBBitmap * blockSheet, uint8_t * playArea, GameState * gs);
bool checkControls(RGBBitmap * sStrip, uint8_t * playArea, GameState * gs);

void sysInit(){
  loadDisplayDriver();
  initializeDisplayDefaults();

  loadInputDriver();
  initInputDefault();

  loadFileSystemDriver();
  initSDandFAT();
}

void main(void){
  sysInit();

  /** load sprite strip for blocks **/
  RGBBitmap blockSpriteStrip;
  uint8_t blockSpriteImgData[BLOCK_STRIP_W*BLOCK_STRIP_H*2];
  blockSpriteStrip.width = BLOCK_STRIP_W;
  blockSpriteStrip.height = BLOCK_STRIP_H;
  blockSpriteStrip.rgbData = (uint16_t*)blockSpriteImgData;

  FileDescriptor blockStripFile;
  if(openFile("/blocks.data", &blockStripFile)){
    readRGB24File(blockSpriteStrip.rgbData, BLOCK_STRIP_W*BLOCK_STRIP_H*3, &blockStripFile);
  }
  else {
    //the magic error box
    Rec r1;
    r1.x = 100;
    r1.y = 50;
    r1.w = 40;
    r1.h = 40;
    drawRectangle(&r1,0xF800);
  }

  /** setup play area **/
  uint8_t playArea[PLAY_AREA_W*PLAY_AREA_H];
  memset(playArea, BLOCK_NONE, PLAY_AREA_W*PLAY_AREA_H);

  clearDisplay(0xA516);
  Rec playAreaR;
  playAreaR.x = PLAY_AREA_X_OFFSET;
  playAreaR.y = PLAY_AREA_Y_OFFSET;
  playAreaR.w = PLAY_AREA_W*PLAY_AREA_UNIT_W_H;
  playAreaR.h = PLAY_AREA_H*PLAY_AREA_UNIT_W_H;
  drawRectangle(&playAreaR, 0x0000);

  GameState gs;
  logicInit(&gs);
  for(;;){
    for(int i =0; i < 10;i++){
      if(checkControls(&blockSpriteStrip, playArea, &gs)){
        break;
      }
      for(int z = 0; z < 100000; z++)asm("");
      draw(&blockSpriteStrip, playArea);
    }

    update(&blockSpriteStrip, playArea, &gs);
    draw(&blockSpriteStrip, playArea);
  }
}


/**============= rendering stuff ================**/
void drawBlock(uint16_t gridX, uint16_t gridY, uint8_t blockType, RGBBitmap * blockSheet){
  uint16_t x = gridX*PLAY_AREA_UNIT_W_H + PLAY_AREA_X_OFFSET;
  uint16_t xTextureOffset = (blockType - 1)*(BLOCK_W_H + BLOCK_SEPARATION);
  uint16_t y = gridY*PLAY_AREA_UNIT_W_H + PLAY_AREA_Y_OFFSET;

  setClipRegion(x, y, BLOCK_W_H*BLOCK_SCALE, BLOCK_W_H*BLOCK_SCALE );

  if(blockType == BLOCK_NONE){
    Rec none;
    none.x = x;
    none.y = y;
    none.w = BLOCK_W_H*BLOCK_SCALE;
    none.h = BLOCK_W_H*BLOCK_SCALE;
    drawRectangle(&none, 0x0000);
  }
  else{
    drawBitmap(blockSheet, x - xTextureOffset*BLOCK_SCALE, y, BLOCK_SCALE);
  }
}

void draw(RGBBitmap * blockSheet, uint8_t * playArea){
  for(uint32_t i =0; i < (PLAY_AREA_W*PLAY_AREA_H); i++){
    drawBlock(i % PLAY_AREA_W, i / PLAY_AREA_W, playArea[i], blockSheet);
  }
  clearClipRegion();
}

/**============== game logic down here =====================**/
bool isCollide(uint16_t * indices, uint16_t count, uint8_t * playArea){
  //scan for problems
  for(uint16_t i =0; i < count; i++){
    if(playArea[indices[i]] != BLOCK_NONE){
      //space already in use
      return true;
    }
    else if (indices[i] > PLAY_AREA_W*PLAY_AREA_H){
      // off bottom
      return true;
    }
  }
  return false;
}

// true if shift will cause indices to "hit" the side of the screen
bool isCollideLeftRight(uint16_t * indices, uint16_t count, int shift, uint8_t * playArea){
  for(uint16_t i =0; i < count; i++){
    if(playArea[indices[i] + shift] != BLOCK_NONE){
      //space already in use
      bool isSelf = false;
      for(uint16_t z = 0; z < count; z++){
        if(indices[i] + shift == indices[z]){
          isSelf = true;
        }
      }
      if(!isSelf){
        return true;
      }
    }
    else if((uint16_t)((indices[i] + shift)/PLAY_AREA_W) != (uint16_t)(indices[i] / PLAY_AREA_W) ){
      return true;
    }
  }
  return false;
}

void setCells(uint16_t * indices, uint16_t count, uint16_t newType, uint8_t * playArea){
  for(uint16_t i =0; i < count; i++){
    playArea[indices[i]] = newType;
  }
}

void shiftCells(uint16_t * indices, uint16_t count, int shift, uint8_t * playArea){
  uint16_t old[count];
  for(uint16_t i =0; i < count; i++){
    old[i] = playArea[indices[i]];
  }

  for(uint16_t i =0; i < count; i++){
    playArea[indices[i]] = BLOCK_NONE;
  }

  for(uint16_t i =0; i < count; i++){
    playArea[indices[i] + shift] = old[i];
    indices[i] += shift;
  }
}

void logicInit(GameState * gs){
  gs->isActive = false;
  gs->activeBlockType = BLOCK_NONE;
  memset(gs->activeBlocks, 0, BLOCKS_PER_SHAPE*sizeof(uint16_t));
}

bool checkControls(RGBBitmap * sStrip, uint8_t * playArea, GameState * gs){
  ControlState cState;
  getControlState(&cState);

  if(gs->isActive){
    if(cState.dPadLeft){
      if(!isCollideLeftRight(gs->activeBlocks, BLOCKS_PER_SHAPE, -1, playArea)){
        shiftCells(gs->activeBlocks, BLOCKS_PER_SHAPE, -1, playArea);
      }
    }
    else if(cState.dPadRight){
      if(!isCollideLeftRight(gs->activeBlocks, BLOCKS_PER_SHAPE, 1, playArea)){
        shiftCells(gs->activeBlocks, BLOCKS_PER_SHAPE, 1, playArea);
      }
    }
    else if (cState.dPadDown){
        update(sStrip, playArea, gs);
    }

    return false;
  }
  else {
    return true;
  }
}

void shiftGameBoard(int line, uint8_t * playArea){
  for(int i =line; i >= 0; i-= PLAY_AREA_W){
    uint16_t lineIndices[PLAY_AREA_W];
    for(int z =0; z < PLAY_AREA_W; z++){
      lineIndices[z] = i + z;
    }
    shiftCells(lineIndices, PLAY_AREA_W, PLAY_AREA_W, playArea);
  }
}

#define BLINK_COUNT 5
#define BLINK_DELAY 100999
void removeFullLines(RGBBitmap * sStrip, uint8_t * playArea, GameState * gs){
  for(int i = 0; i < PLAY_AREA_W*PLAY_AREA_H; i += PLAY_AREA_W){
    int z;
    for(z =0; z < PLAY_AREA_W; z++){
      if(playArea[i + z] == BLOCK_NONE){
        break;
      }
    }

    if(z == PLAY_AREA_W){
      //blink line to remove.
      uint16_t lineIndices[PLAY_AREA_W];
      for(int k =0; k < PLAY_AREA_W; k++){
        lineIndices[k] = i + k;
      }

      for(int p = 0; p < BLINK_COUNT; p++){
        setCells(lineIndices, PLAY_AREA_W, BLOCK_NONE, playArea);
        draw(sStrip, playArea);
        for(int e =0; e < BLINK_DELAY; e++)asm("");
        setCells(lineIndices, PLAY_AREA_W, BLOCK_ORANGE, playArea);
        draw(sStrip, playArea);
      }

      shiftGameBoard(i, playArea);
      draw(sStrip, playArea);
    }
  }
}

void spawnSquare(uint8_t * playArea, GameState * gs);
void spawnLLBlock(uint8_t * playArea, GameState * gs);
void spawnLRBlock(uint8_t * playArea, GameState * gs);
void spawnLine(uint8_t * playArea, GameState * gs);
void spawnTriangle(uint8_t * playArea, GameState * gs);
void spawnZigLeft(uint8_t * playArea, GameState * gs);
void spawnZigRight(uint8_t * playArea, GameState * gs);

void update(RGBBitmap * sStrip, uint8_t * playArea, GameState * gs){

  if(gs->isActive){
    //move down
    uint16_t lookAhead[BLOCKS_PER_SHAPE];
    memcpy(lookAhead, gs->activeBlocks, BLOCKS_PER_SHAPE*sizeof(uint16_t));
    setCells(lookAhead, BLOCKS_PER_SHAPE, BLOCK_NONE, playArea);
    for(uint16_t i =0; i < BLOCKS_PER_SHAPE; i++){
      *(lookAhead + i) += PLAY_AREA_W;
    }

    if(!isCollide(lookAhead, BLOCKS_PER_SHAPE, playArea)){
      setCells(gs->activeBlocks, BLOCKS_PER_SHAPE, BLOCK_NONE, playArea);
      memcpy(gs->activeBlocks, lookAhead, BLOCKS_PER_SHAPE*sizeof(uint16_t));
      setCells(gs->activeBlocks, BLOCKS_PER_SHAPE, gs->activeBlockType, playArea);
    }
    else{
      //cannot move down. must have hit some thing!
      setCells(gs->activeBlocks, BLOCKS_PER_SHAPE, gs->activeBlockType, playArea);
      gs->isActive = false;
    }
  }
  else{
    removeFullLines(sStrip, playArea, gs);

    //drop new block
    gs->isActive = true;
    uint8_t rndBlock = rand()%5;

    switch(rndBlock){
      case BLOCK_CUBE:
        spawnSquare(playArea, gs);
        break;
      case BLOCK_L_LEFT:
        spawnLLBlock(playArea, gs);
        break;
      case BLOCK_L_RIGHT:
        spawnLRBlock(playArea, gs);
        break;
      case BLOCK_LINE:
        spawnLine(playArea, gs);
        break;
      case BLOCK_TRIANGLE:
        spawnTriangle(playArea, gs);
        break;
      default:
        return;//wtf
    }

    gs->activeBlockType = 1 + rand()%6;
    if(isCollide(gs->activeBlocks, BLOCKS_PER_SHAPE, playArea)){
      //game over!!! do some thing
      Rec r1;
      r1.x = PLAY_AREA_X_OFFSET;
      r1.y = 50;
      r1.w = PLAY_AREA_W*PLAY_AREA_UNIT_W_H;
      r1.h = 50;
      drawRectangle(&r1,0xF800);
      for(;;){// DEAD LOOP!
        asm("");
      }
    }
    else{
      setCells(gs->activeBlocks, BLOCKS_PER_SHAPE, gs->activeBlockType, playArea);
    }
  }
}

void spawnSquare(uint8_t * playArea, GameState * gs){
  uint16_t square[] = {PLAY_AREA_W/2, PLAY_AREA_W/2 + 1, (PLAY_AREA_W*3)/2, (PLAY_AREA_W*3)/2 + 1};
  memcpy(gs->activeBlocks, square, BLOCKS_PER_SHAPE*sizeof(uint16_t));
}

void spawnLLBlock(uint8_t * playArea, GameState * gs){
  uint16_t leftL[] = {PLAY_AREA_W/2, (PLAY_AREA_W*3)/2, (PLAY_AREA_W*5)/2, (PLAY_AREA_W*5)/2 - 1};
  memcpy(gs->activeBlocks, leftL, BLOCKS_PER_SHAPE*sizeof(uint16_t));
}

void spawnLRBlock(uint8_t * playArea, GameState * gs){
  uint16_t rightL[] = {PLAY_AREA_W/2, (PLAY_AREA_W*3)/2, (PLAY_AREA_W*5)/2, (PLAY_AREA_W*5)/2 + 1};
  memcpy(gs->activeBlocks, rightL, BLOCKS_PER_SHAPE*sizeof(uint16_t));
}

void spawnLine(uint8_t * playArea, GameState * gs){
  uint16_t line[] = {PLAY_AREA_W/2, (PLAY_AREA_W*3)/2, (PLAY_AREA_W*5)/2, (PLAY_AREA_W*7)/2};
  memcpy(gs->activeBlocks, line, BLOCKS_PER_SHAPE*sizeof(uint16_t));
}

void spawnTriangle(uint8_t * playArea, GameState * gs){
  uint16_t triangle[] = {PLAY_AREA_W/2, (PLAY_AREA_W*3)/2, (PLAY_AREA_W*3)/2 + 1, (PLAY_AREA_W*3)/2 - 1};
  memcpy(gs->activeBlocks, triangle, BLOCKS_PER_SHAPE*sizeof(uint16_t));
}

void spawnZigLeft(uint8_t * playArea, GameState * gs){
  uint16_t zigL[] = {PLAY_AREA_W/2, (PLAY_AREA_W)/2 - 1, (PLAY_AREA_W*3)/2 , (PLAY_AREA_W*3)/2 + 1};
  memcpy(gs->activeBlocks, zigL, BLOCKS_PER_SHAPE*sizeof(uint16_t));
}

void spawnZigRight(uint8_t * playArea, GameState * gs){
  uint16_t zigR[] = {PLAY_AREA_W/2, (PLAY_AREA_W)/2 + 1, (PLAY_AREA_W*3)/2 , (PLAY_AREA_W*3)/2 - 1};
  memcpy(gs->activeBlocks, zigR, BLOCKS_PER_SHAPE*sizeof(uint16_t));
}
