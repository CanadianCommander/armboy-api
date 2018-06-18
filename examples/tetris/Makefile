CC = arm-none-eabi-gcc
CFLAGS = --std=gnu11 -Ofast -nostartfiles -fpic  -mcpu=cortex-m3 -mthumb --specs=nosys.specs -Tprog.ld -Wl,-N
OBYCP = arm-none-eabi-objcopy

INCLUDE = -I ../../src/
LIB=../../src/armboyAPI.a

tetris.bin : tetris.c prog.S
	$(CC) $(CFLAGS) -o tetris.elf tetris.c prog.S $(LIB) $(INCLUDE)
	$(OBYCP) -O binary tetris.elf tetris.bin
