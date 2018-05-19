CC = arm-none-eabi-gcc
CFLAGS = --std=gnu11 -Ofast -nostartfiles -fPIC -mcpu=cortex-m3 -mthumb -fno-builtin  -ffreestanding   --specs=nosys.specs --specs=nano.specs -Tprog.ld -Wl,-N
OBYCP = arm-none-eabi-objcopy

INCLUDE = -I ../../src/
LIB=../../src/armboyAPI.a

test.bin : main.c prog.S
	$(CC) $(CFLAGS) -o main.elf main.c prog.S $(LIB) $(INCLUDE)
	$(OBYCP) -O binary main.elf main.bin
