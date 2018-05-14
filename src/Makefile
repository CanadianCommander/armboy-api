CC = arm-none-eabi-gcc
CFLAGS = -c --std=gnu11 -nostartfiles -fPIC -mcpu=cortex-m3 -mthumb -fno-builtin  -ffreestanding   --specs=nosys.specs --specs=nano.specs -Wl,-N
OBYCP = arm-none-eabi-objcopy

INCLUDE =
CFILES = ./*.c

test.bin : $(CFILES)
	$(CC) $(CFLAGS) -o armboyAPI.a $(CFILES) $(INCLUDE)
