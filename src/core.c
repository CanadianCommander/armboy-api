#include "core.h"


uint32_t rand(void){
  uint32_t val;
  SYS_CALL(CORE_FUNCTIONS_ID, CF_RANDOM, (void*)&val);
  return val;
}
