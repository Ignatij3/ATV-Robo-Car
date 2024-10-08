MCU   := atmega328p
F_CPU := 16000000UL
BAUD  := 9600UL

CC      := avr-gcc
OBJCOPY := avr-objcopy
OBJDUMP := avr-objdump
AVRSIZE := avr-size
AVRDUDE := avrdude

PROGRAMMER_TYPE := xplainedmini
PROGRAMMER_ARGS := -v -p$(MCU) -c$(PROGRAMMER_TYPE) -b57600

SOURCES := main.c
BLDDIR  := build
OBJDIR  := obj
TARGET  := $(BLDDIR)/robocar_atv

ifeq ($(OS),Windows_NT)
SOURCES += $(wildcard libs/**/*.c)
SOURCES += $(wildcard libs/**/**/*.c)
clean:
	rm -rf $(OBJDIR)
	rm -rf $(BLDDIR)

else
SOURCES += $(shell find libs/ -type f -name '*.c')
clean:
	@cd $(OBJDIR) && rm -rf ./*
	@cd $(BLDDIR) && rm -f *.elf *.hex *.map
	@rmdir $(OBJDIR) $(BLDDIR)

endif

OBJECTS := $(addprefix $(OBJDIR)/,$(SOURCES:%.c=%.o))
HEADERS := $(SOURCES:.c=.h)

INCL_PATH := $(CURDIR)/libs
INCLUDES  := -I$(INCL_PATH)/bool -I$(INCL_PATH)/central_controller -I$(INCL_PATH)/speed_sensor -I$(INCL_PATH)/oled -I$(INCL_PATH)/global_constants -I$(INCL_PATH)/central_controller -I$(INCL_PATH)/engine_controller -I$(INCL_PATH)/ino_libs -I$(INCL_PATH)/serial_communication

CPPFLAGS    := -DF_CPU=$(F_CPU) -DBAUD=$(BAUD) $(INCLUDES)
# -Wconversion
CFLAGS      := -Os -g -Wall -Wextra -std=c11 -Wold-style-definition\
 -Wno-missing-braces -Wno-missing-field-initializers -Wwrite-strings \
 -Wformat=2 -Wswitch-default -Wswitch-enum -Wcast-align \
 -Wpointer-arith -Wbad-function-cast -Wstrict-overflow=4 \
 -Wstrict-prototypes -Winline -Wundef -Wnested-externs \
 -Wcast-qual -Wshadow -Wunreachable-code -Wlogical-op \
 -Wfloat-equal -Wstrict-aliasing=2 -Wredundant-decls \
 -fno-omit-frame-pointer -ffloat-store -fno-common -fstrict-aliasing
CFLAGS      += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
# Splits up object files per function
CFLAGS      += -ffunction-sections -fdata-sections
LDFLAGS     := -Wl,-Map,$(TARGET).map
# Optional, but often ends up with smaller code
LDFLAGS     += -Wl,--gc-sections
TARGET_ARCH := -mmcu=$(MCU)

all: $(TARGET).hex

$(OBJDIR) $(BLDDIR):
	@mkdir -p "$@"

$(OBJECTS): $(OBJDIR)/%.o: %.c | $(OBJDIR)
	@mkdir -p "$(dir $@)"
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c -o $@ $<

$(TARGET).elf: $(OBJECTS) | $(BLDDIR)
	@$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ $(LDLIBS) -o $@

%.hex: %.elf
	@$(OBJCOPY) -j .text -j .data -O ihex $< $@

.PHONY: all clean flash fuses

flash: $(TARGET).hex
	@$(AVRDUDE) $(PROGRAMMER_ARGS) -Uflash:w:$<:i

# Mega 48, 88, 168, 328 default values
LFUSE := 0xE0
HFUSE := 0xD6
EFUSE := 0xFC

# Generic
FUSE_STRING = -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m

fuses: 
	@$(AVRDUDE) $(PROGRAMMER_ARGS) $(FUSE_STRING)
show_fuses:
	@$(AVRDUDE) $(PROGRAMMER_ARGS) -nv
