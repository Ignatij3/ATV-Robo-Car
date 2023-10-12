MCU   = atmega328p
F_CPU = 16000000UL
BAUD  = 9600UL

PROGRAMMER_TYPE := xplainedmini
PROGRAMMER_ARGS := -v -p $(MCU) -c $(PROGRAMMER_TYPE) -b 57600

CC      = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
AVRSIZE = avr-size
AVRDUDE = avrdude

INCL_PATH = libs
INCLUDES  = -IC:\msys64\mingw64\include -I$(INCL_PATH)/central_controller -I$(INCL_PATH)/engine_controller -I$(INCL_PATH)/ino_libs -I$(INCL_PATH)/serial_communication

BLDDIR = build
OBJDIR = obj
TARGET = $(BLDDIR)/robocar_atv

# Compilation options, type man avr-gcc if you're curious.
CPPFLAGS    := -DF_CPU=$(F_CPU) -DBAUD=$(BAUD) $(INCLUDES)
CFLAGS      := -Os -g -std=gnu99 -Wall
# Use short (8-bit) data types
CFLAGS      += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS      += -ffunction-sections -fdata-sections
LDFLAGS     = -Wl,-Map,$(TARGET).map
LDFLAGS     += -Wl,--gc-sections
TARGET_ARCH = -mmcu=$(MCU)

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
	rmdir /s /q $(BLDDIR)
	rmdir /s /q $(OBJDIR)

flash: $(TARGET).hex
	$(AVRDUDE) $(PROGRAMMER_ARGS) -U flash:w:$<:i

# Mega 48, 88, 168, 328 default values
LFUSE = 0xE0
HFUSE = 0xDE
EFUSE = 0xFC

# Generic
FUSE_STRING = -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m

fuses: 
	$(AVRDUDE) $(PROGRAMMER_ARGS) $(FUSE_STRING)
show_fuses:
	$(AVRDUDE) $(PROGRAMMER_ARGS) -nv