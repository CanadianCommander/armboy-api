#ifndef CORE_H_
#define CORE_H_
#include <stdint.h>
#include <stdbool.h>
#include "util.h"

#define CORE_FUNCTIONS_ID 0

/** functions list **/
#define CF_RANDOM     0
#define CF_SLEEP      1
#define CF_IS_LOADED  2

/** return random 32 bit number (from TRANG hardware) **/
uint32_t randomTRNG(void);

/** sleep for ms milliseconds **/
void sleep(uint32_t ms);

/** for some reason, the ARM provided itoa function returns incorrect results so, here is mine **/
char * armBoyItoa(int num, char * str, int base);

/** true if module with module id (mid) is loaded **/
bool isLoaded(uint32_t mid);

#endif /*CORE_H_*/
