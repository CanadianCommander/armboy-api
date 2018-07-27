#include "core.h"


uint32_t randomTRNG(void){
  uint32_t val;
  SYS_CALL(CORE_FUNCTIONS_ID, CF_RANDOM, (void*)&val);
  return val;
}

void sleep(uint32_t ms){
  SYS_CALL(CORE_FUNCTIONS_ID, CF_SLEEP, (void *)&ms);
}

char * armBoyItoa(int num, char * str, int base){
  int numCurr = num;
  int i =0;
  for(i =0; ; i++){
    str[i] = (char)((numCurr % base) + 48);
    numCurr /= base;

    if(numCurr == 0)break;
  }
  str[i+1] = 0x0;

  int count = i + 1;
  for(int z = 0; z < count/2; z++){
    char tmp = str[z];
    str[z] = str[i];
    str[i] = tmp;
    i --;
  }
  return str;
}

bool isLoaded(uint32_t mid){
  SYS_CALL(CORE_FUNCTIONS_ID, CF_IS_LOADED, (void *)&mid);
  return (bool)mid;
}
