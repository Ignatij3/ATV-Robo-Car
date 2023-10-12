MCU   := atmega328p
F_CPU := 16000000UL
BAUD  := 9600UL

CC      := avr-gcc
OBJCOPY := avr-objcopy
OBJDUMP := avr-objdump
AVRSIZE := avr-size
AVRDUDE := avrdude

PROGRAMMER_TYPE := xplainedmini
<<<<<<< HEAD
PROGRAMMER_ARGS := -v -p $(MCU) -c $(PROGRAMMER_TYPE) -b 57600 
=======
PROGRAMMER_ARGS := -C/etc/avrdude.conf -v -p$(MCU) -c$(PROGRAMMER_TYPE) -b57600
>>>>>>> e27875f (Temporary fixed)

INCL_PATH := $(CURDIR)/libs
INCLUDES  := -I$(INCL_PATH)/bool -I$(INCL_PATH)/central_controller -I$(INCL_PATH)/engine_controller -I$(INCL_PATH)/ino_libs -I$(INCL_PATH)/serial_communication

<<<<<<< HEAD
INCL_PATH = libs
INCLUDES  = -I$(INCL_PATH)/bool -I$(INCL_PATH)/central_controller -I$(INCL_PATH)/oled -I$(INCL_PATH)/engine_controller -I$(INCL_PATH)/ino_libs -I$(INCL_PATH)/serial_communication
=======
BLDDIR := build
OBJDIR := obj
TARGET := $(BLDDIR)/robocar_atv
>>>>>>> e27875f (Temporary fixed)

SOURCES := main.c
SOURCES += $(shell find libs/ -type f -name '*.c')
OBJECTS := $(addprefix $(OBJDIR)/,$(SOURCES:%.c=%.o))
HEADERS := $(SOURCES:.c=.h)

SOURCES = main.c
SOURCES += $(wildcard libs/**/*.c)
SOURCES += $(wildcard libs/**/**/*.c)
$(info LIB_C_FILES: $(LIB_C_FILES))

OBJECTS = $(addprefix $(OBJDIR)/,$(SOURCES:%.c=%.o))
HEADERS = $(SOURCES:.c=.h)


CPPFLAGS    = -DF_CPU=$(F_CPU) -DBAUD=$(BAUD) $(INCLUDES)
CFLAGS      = -Os -g -std=gnu99 -Wall
CFLAGS      += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
# Splits up object files per function
CFLAGS      += -ffunction-sections -fdata-sections
LDFLAGS     := -Wl,-Map,$(TARGET).map
# Optional, but often ends up with smaller code
LDFLAGS     += -Wl,--gc-sections
TARGET_ARCH := -mmcu=$(MCU)

all: $(TARGET).hex

$(OBJDIR) $(BLDDIR):
	mkdir -p "$@"

$(OBJECTS): $(OBJDIR)/%.o: %.c | $(OBJDIR)
	mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c -o $@ $<

$(TARGET).elf: $(OBJECTS) | $(BLDDIR)
	$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ $(LDLIBS) -o $@

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

.PHONY: all debug clean flash fuses

debug:
	@echo
	@echo "Source files:"   $(SOURCES)
	@echo "Object files:"   $(OBJECTS)
	@echo "MCU, F_CPU, BAUD:"  $(MCU), $(F_CPU), $(BAUD)
	@echo	

clean:
<<<<<<< HEAD
	rm -rf $(OBJDIR)
	rm -rf $(BLDDIR)
=======
	cd $(BLDDIR) && rm -f *.elf *.hex *.map
	cd $(OBJDIR) && rm -rf ./*
	rmdir $(BLDDIR) $(OBJDIR)
>>>>>>> e27875f (Temporary fixed)

flash: $(TARGET).hex
	$(AVRDUDE) $(PROGRAMMER_ARGS) -Uflash:w:$<:i

# Mega 48, 88, 168, 328 default values
LFUSE = 0x62
HFUSE = 0xdf
EFUSE = 0x00

# Generic
FUSE_STRING = -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m

fuses: 
	$(AVRDUDE) $(PROGRAMMER_ARGS) $(FUSE_STRING)
show_fuses:
	$(AVRDUDE) $(PROGRAMMER_ARGS) -nv