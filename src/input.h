#ifndef INPUT_H_
#define INPUT_H_
#include <stdint.h>
#include <stdbool.h>

//display driver id
#define INPUT_DRIVER_ID 2
//display functions
#define INPUT_FUNCTION_INIT           0
#define INPUT_FUNCTION_DEFAULT_INIT   1
#define INPUT_FUNCTION_GET_STATE      2

/**
  state of the user input controls (buttons)
*/
typedef struct {
  uint8_t button1, button2, button3, button4;

  uint8_t dPadUp, dPadDown, dPadLeft, dPadRight;

  uint8_t home, start;
} ControlState;

void loadInputDriver();


/**
  true if input driver is loaded
*/
bool isInputDriverLoaded();

/**
  initialize user input with default pin mappings
*/
void initInputDefault();

/**
  fills out the cs structure with the current control state.
*/
void getControlState(ControlState * cs);

#endif /*INPUT_H_*/
