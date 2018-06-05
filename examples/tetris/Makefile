CC = arm-none-eabi-gcc
CFLAGS = --std=gnu11 -Ofast -nostartfiles -fPIC -mcpu=cortex-m3 -mthumb -fno-builtin  -ffreestanding   --specs=nosys.specs --specs=nano.specs -Tprog.ld -Wl,-N
OBYCP = arm-none-eabi-objcopy

INCLUDE = -I ../../src/
LIB=../../src/armboyAPI.a

tetris.bin : tetris.c prog.S
	$(CC) $(CFLAGS) -o tetris.elf tetris.c prog.S $(LIB) $(INCLUDE)
	$(OBYCP) -O binary tetris.elf tetris.bin
