#include "input.h"
#include "util.h"
#include "../drivers/ArmboyInputDriver/src/inputSyscall.h"

void loadInputDriver(){
  LOAD_KERNEL_MOD(INPUT_DRIVER_ID);
}

void initInputDefault(){
  SYS_CALL(INPUT_DRIVER_ID, INPUT_FUNCTION_DEFAULT_INIT, NULL);
}


void getControlState(ControlState * cs){
  SYS_CALL(INPUT_DRIVER_ID, INPUT_FUNCTION_GET_STATE, (InputState *)cs);
}
