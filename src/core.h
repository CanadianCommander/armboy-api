#ifndef CORE_H_
#define CORE_H_
#include <stdint.h>
#include "util.h"

#define CORE_FUNCTIONS_ID 0

/** functions list **/
#define CF_RANDOM 0
#define CF_SLEEP  1

/** return random 32 bit number (from TRANG hardware) **/
uint32_t randomTRNG(void);

/** sleep for ms milliseconds **/
void sleep(uint32_t ms);

/** for some reason, the ARM provided itoa function returns incorrect results so, here is mine **/
char * armBoyItoa(int num, char * str, int base);



#endif /*CORE_H_*/
