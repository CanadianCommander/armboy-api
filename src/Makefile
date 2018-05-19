CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar
CFLAGS = -c --std=gnu11 -nostartfiles -fPIC -mcpu=cortex-m3 -mthumb -fno-builtin  -ffreestanding   --specs=nosys.specs --specs=nano.specs -Wl,-N
OBYCP = arm-none-eabi-objcopy

INCLUDE =
CFILES = $(wildcard *.c)
OBJFILES = $(CFILES:.c=.o)

.PHONY: all

all: armboyAPI.a

armboyAPI.a : $(OBJFILES)
	$(AR) rcs armboyAPI.a $(OBJFILES)

%.o : %.c
	$(CC) $(CFLAGS) $< $(INCLUDE)
