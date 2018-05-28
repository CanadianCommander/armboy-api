#ifndef UTIL_H_
#define UITL_H_

#ifndef NULL
#define NULL 0
#endif 

#define SYS_CALL(id, func, arg) \
  asm(                        \
    "mov r0, %[arg0] \n"      \
    "mov r1, %[arg1] \n"      \
    "svc #3 \n"               \
    :                         \
    : [arg0] "r" ((id << 16) | (0x0000FFFF & func)), [arg1] "r" (arg) \
    : "r0", "r1"              \
  )                           \


#define LOAD_KERNEL_MOD(id)   \
  asm(                        \
    "mov r0, %[modId] \n"     \
    "svc #4 \n"               \
    :                         \
    : [modId] "r" (id)        \
    :  "r0"                   \
  )                           \



#endif /*UTIL_H_*/
