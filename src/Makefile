MCU   = atmega328p
F_CPU = 16000000UL
BAUD  = 9600UL

PROGRAMMER_TYPE = arduino
PROGRAMMER_ARGS = -C/etc/avrdude.conf -v -p$(MCU) -c$(PROGRAMMER_TYPE) -P/dev/ttyACM0 -b57600

CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
AVRSIZE = avr-size
AVRDUDE = avrdude

TARGET = build/$(lastword $(subst /, ,$(CURDIR)))
INCL_PATH = $(CURDIR)/libs
INCLUDES = -I$(INCL_PATH)/bool -I$(INCL_PATH)/central_controller -I$(INCL_PATH)/engine_controller -I$(INCL_PATH)/ino_libs -I$(INCL_PATH)/serial_communication

SOURCES=main.c
SOURCES+=$(wildcard libs/**/*.c)
OBJECTS=$(SOURCES:.c=.o)
HEADERS=$(SOURCES:.c=.h)

# Compilation options, type man avr-gcc if you're curious.
CPPFLAGS = -DF_CPU=$(F_CPU) -DBAUD=$(BAUD) $(INCLUDES)
CFLAGS = -Os -g -std=gnu99 -Wall
# Use short (8-bit) data types 
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
# Splits up object files per function
CFLAGS += -ffunction-sections -fdata-sections 
LDFLAGS = -Wl,-Map,$(TARGET).map 
# Optional, but often ends up with smaller code
LDFLAGS += -Wl,--gc-sections
TARGET_ARCH = -mmcu=$(MCU)

#* Explicit pattern rules:
# To make .o files from .c files
%.o: %.c $(HEADERS) Makefile
	 $(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c -o $@ $<;

$(TARGET).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ $(LDLIBS) -o $@

%.hex: %.elf
	 $(OBJCOPY) -j .text -j .data -O ihex $< $@

%.eeprom: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

%.lst: %.elf
	$(OBJDUMP) -S $< > $@

.PHONY: all eeprom depend size clean flash fuses

all: $(TARGET).hex

debug:
	@echo
	@echo "Source files:"   $(SOURCES)
	@echo "MCU, F_CPU, BAUD:"  $(MCU), $(F_CPU), $(BAUD)
	@echo	

depend: $(SRCS)
	makedepend $(INCLUDES) $^\


# Optionally show how big the resulting program is 
size:  $(TARGET).elf
	$(AVRSIZE) -C --mcu=$(MCU) $(TARGET).elf

clean:
	rm -rf *.elf *.hex *.obj ./**/*.o *.d *.eep *.lst *.lss *.sym *.map *~ *.eeprom

flash: $(TARGET).hex
	$(AVRDUDE) $(PROGRAMMER_ARGS) -Uflash:w:$<:i

flash_eeprom: $(TARGET).eeprom
	$(AVRDUDE) $(PROGRAMMER_ARGS) -Ueeprom:w:$<:i

avrdude_terminal:
	$(AVRDUDE) $(PROGRAMMER_ARGS) -nt

## Mega 48, 88, 168, 328 default values
LFUSE = 0x62
HFUSE = 0xdf
EFUSE = 0x00

## Generic
FUSE_STRING = -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m

fuses: 
	$(AVRDUDE) $(PROGRAMMER_ARGS) $(FUSE_STRING)
show_fuses:
	$(AVRDUDE) $(PROGRAMMER_ARGS) -nv