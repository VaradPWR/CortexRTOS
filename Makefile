CC = arm-none-eabi-gcc

CFLAGS = -mcpu=cortex-m3 -mthumb -g -O0 -Wall \
-Icore \
-Iinclude

LDFLAGS = -Tstartup/STM32F103XX_FLASH.ld --specs=nosys.specs

SRC = main.c \
core/system.c \
rtos/task.c \
rtos/systick.c \
startup/startup_stm32f103xb.s

OBJ = $(SRC:.c=.o)
OBJ := $(OBJ:.s=.o)

TARGET = build/main.elf

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/*.o build/*.elf *.o startup/*.o
