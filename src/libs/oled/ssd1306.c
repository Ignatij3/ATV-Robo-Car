
// @includes
#include "ssd1306.h"
#include "../global_constants/global_constants.h"
#include "font6x8.h"
#include "twi.h"
#include <stdio.h>
#include <string.h> // memset function

#define str(s) #s

static uint16_t _counter;

// @array Init command
const uint8_t INIT_SSD1306[] PROGMEM = {
    18,                                                             // number of initializers
    0, SSD1306_DISPLAY_OFF,                                         // 0xAE = Set Display OFF
    1, SSD1306_SET_MUX_RATIO, 63,                                   // 0xA8 - 64MUX for 128 x 64 version
                                                                    //      - 32MUX for 128 x 32 version
    1, SSD1306_MEMORY_ADDR_MODE, 0x00,                              // 0x20 = Set Memory Addressing Mode
                                                                    // 0x00 - Horizontal Addressing Mode
                                                                    // 0x01 - Vertical Addressing Mode
                                                                    // 0x02 - Page Addressing Mode (RESET)
    2, SSD1306_SET_COLUMN_ADDR, START_COLUMN_ADDR, END_COLUMN_ADDR, // 0x21 = Set Column Address, 0 - 127
    2, SSD1306_SET_PAGE_ADDR, START_PAGE_ADDR, END_PAGE_ADDR,       // 0x22 = Set Page Address, 0 - 7
    0, SSD1306_SET_START_LINE,                                      // 0x40
    1, SSD1306_DISPLAY_OFFSET, 0x00,                                // 0xD3
    0, SSD1306_SEG_REMAP_OP,                                        // 0xA0 / remap 0xA1
    0, SSD1306_COM_SCAN_DIR_OP,                                     // 0xC0 / remap 0xC8
    1, SSD1306_COM_PIN_CONF, 0x12,  // 0xDA, 0x12 - Disable COM Left/Right remap, Alternative COM pin configuration
                                    //       0x12 - for 128 x 64 version
                                    //       0x02 - for 128 x 32 version
    1, SSD1306_SET_CONTRAST, 0x7F,  // 0x81, 0x7F - reset value (max 0xFF)
    0, SSD1306_DIS_ENT_DISP_ON,     // 0xA4
    0, SSD1306_DIS_NORMAL,          // 0xA6
    1, SSD1306_SET_OSC_FREQ, 0x80,  // 0xD5, 0x80 => D=1; DCLK = Fosc / D <=> DCLK = Fosc
    1, SSD1306_SET_PRECHARGE, 0xc2, // 0xD9, higher value less blinking
                                    // 0xC2, 1st phase = 2 DCLK,  2nd phase = 13 DCLK
    1, SSD1306_VCOM_DESELECT, 0x20, // Set V COMH Deselect, reset value 0x22 = 0,77xUcc
    1, SSD1306_SET_CHAR_REG, 0x14,  // 0x8D, Enable charge pump during display on
    0, SSD1306_DISPLAY_ON           // 0xAF = Set Display ON
};

static char cacheMemLcd[CACHE_SIZE_MEM];
uint8_t SSD1306_Init(uint8_t address) {
    // variables
    const uint8_t *commands = INIT_SSD1306;
    // number of commands
    unsigned short int no_of_commands = pgm_read_byte(commands++);
    // argument
    uint8_t no_of_arguments;
    // command
    uint8_t command;
    // init status
    uint8_t status = INIT_STATUS;

    // TWI: Init
    // -------------------------------------------------------------------------------------
    TWI_Init();

    // TWI: start & SLAW
    // -------------------------------------------------------------------------------------
    status = SSD1306_Send_StartAndSLAW(address);
    // request - start TWI
    if (SSD1306_SUCCESS != status) {
        // error
        return status;
    }

    // loop through commands
    while (no_of_commands) {

        // number of arguments
        no_of_arguments = pgm_read_byte(commands++);
        // command
        command = pgm_read_byte(commands++);

        // send command
        // -------------------------------------------------------------------------------------
        status = SSD1306_Send_Command(command);
        // request - start TWI
        if (SSD1306_SUCCESS != status) {
            // error
            return status;
        }

        // send arguments
        // -------------------------------------------------------------------------------------
        while (no_of_arguments--) {
            // send command
            status = SSD1306_Send_Command(pgm_read_byte(commands++));
            // request - start TWI
            if (SSD1306_SUCCESS != status) {
                // error
                return status;
            }
        }
        // decrement
        no_of_commands--;
    }

    // TWI: Stop
    // -------------------------------------------------------------------------------------
    TWI_Stop();

    // success
    return SSD1306_SUCCESS;
}

uint8_t SSD1306_Send_StartAndSLAW(uint8_t address) {
    // init status
    uint8_t status = INIT_STATUS;

    // TWI: start
    // -------------------------------------------------------------------------------------
    status = TWI_MT_Start();
    // request - start TWI
    if (SSD1306_SUCCESS != status) {
        // error
        return status;
    }

    // TWI: send SLAW
    // -------------------------------------------------------------------------------------
    status = TWI_MT_Send_SLAW(address);
    // request - send SLAW
    if (SSD1306_SUCCESS != status) {
        // error
        return status;
    }

    // success
    return SSD1306_SUCCESS;
}

uint8_t SSD1306_Send_Command(uint8_t command) {
    // init status
    uint8_t status = INIT_STATUS;

    // send control byte
    // -------------------------------------------------------------------------------------
    status = TWI_MT_Send_Data(SSD1306_COMMAND);
    // request - start TWI
    if (SSD1306_SUCCESS != status) {
        // error
        return status;
    }

    // send command
    // -------------------------------------------------------------------------------------
    status = TWI_MT_Send_Data(command);
    // request - start TWI
    if (SSD1306_SUCCESS != status) {
        // error
        return status;
    }

    // success
    return SSD1306_SUCCESS;
}

uint8_t SSD1306_NormalScreen(uint8_t address) {
    // init status
    uint8_t status = INIT_STATUS;

    // TWI: start & SLAW
    // -------------------------------------------------------------------------------------
    status = SSD1306_Send_StartAndSLAW(address);
    // request succesfull
    if (SSD1306_SUCCESS != status) {
        // error
        return status;
    }

    // send command
    // -------------------------------------------------------------------------------------
    status = SSD1306_Send_Command(SSD1306_DIS_NORMAL);
    // request succesfull
    if (SSD1306_SUCCESS != status) {
        // error
        return status;
    }

    // success
    return SSD1306_SUCCESS;
}

uint8_t SSD1306_InverseScreen(uint8_t address) {
    // init status
    uint8_t status = INIT_STATUS;

    // TWI: start & SLAW
    // -------------------------------------------------------------------------------------
    status = SSD1306_Send_StartAndSLAW(address);
    // request succesfull
    if (SSD1306_SUCCESS != status) {
        // error
        return status;
    }

    // send command
    // -------------------------------------------------------------------------------------
    status = SSD1306_Send_Command(SSD1306_DIS_INVERSE);
    // request succesfull
    if (SSD1306_SUCCESS != status) {
        // error
        return status;
    }

    // success
    return SSD1306_SUCCESS;
}

/**
 * @desc    SSD1306 Update screen
 *
 * @param   uint8_t address
 *
 * @return  uint8_t
 */
uint8_t SSD1306_UpdateScreen(uint8_t address) {
    // init status
    uint8_t status = INIT_STATUS;
    // init i
    uint16_t i = 0;

    // TWI: start & SLAW
    // -------------------------------------------------------------------------------------
    status = SSD1306_Send_StartAndSLAW(address);
    // request succesfull
    if (SSD1306_SUCCESS != status) {
        // error
        return status;
    }

    // control byte data stream
    // -------------------------------------------------------------------------------------
    status = TWI_MT_Send_Data(SSD1306_DATA_STREAM);
    // request succesfull
    if (SSD1306_SUCCESS != status) {
        // error
        return status;
    }

    //  send cache memory lcd
    // -------------------------------------------------------------------------------------
    while (i < CACHE_SIZE_MEM) {
        // send data
        status = TWI_MT_Send_Data(cacheMemLcd[i]);
        // request succesfull
        if (SSD1306_SUCCESS != status) {
            // error
            return status;
        }
        // increment
        i++;
    }

    // stop TWI
    TWI_Stop();

    // success
    return SSD1306_SUCCESS;
}

/**
 * @desc    SSD1306 Clear screen
 *
 * @param   void
 *
 * @return  void
 */
void SSD1306_ClearScreen(void) {
    // null cache memory lcd
    memset(cacheMemLcd, 0x00, CACHE_SIZE_MEM);
}

/**
 * @desc    SSD1306 Set position
 *
 * @param   uint8_t column -> 0 ... 127
 * @param   uint8_t page -> 0 ... 7 or 3
 *
 * @return  void
 */
void SSD1306_SetPosition(uint8_t x, uint8_t y) {
    // calculate counter
    _counter = x + (y << 7);
}

/**
 * @desc    SSD1306 Update text poisition - this ensure that character will not be divided at the end of row,
 *          the whole character will be depicted on the new row
 *
 * @param   void
 *
 * @return  uint8_t
 */
uint8_t SSD1306_UpdatePosition(void) {
    // y / 8
    uint8_t y = _counter >> 7;
    // y % 8
    uint8_t x = _counter - (y << 7);
    // x + character length + 1
    uint8_t x_new = x + CHARS_COLS_LENGTH + 1;

    // check position
    if (x_new > END_COLUMN_ADDR) {
        // if more than allowable number of pages
        if (y > END_PAGE_ADDR) {
            // return out of range
            return SSD1306_ERROR;
            // if x reach the end but page in range
        } else if (y < (END_PAGE_ADDR - 1)) {
            // update
            _counter = ((++y) << 7);
        }
    }

    // success
    return SSD1306_SUCCESS;
}

uint8_t SSD1306_DrawChar(char character) {
    // variables
    uint8_t i = 0;

    // update text position
    // this ensure that character will not be divided at the end of row, the whole character will be depicted on the new
    // row
    if (SSD1306_UpdatePosition() == SSD1306_ERROR) {
        // error
        return SSD1306_ERROR;
    }

    // loop through 5 bits
    while (i < CHARS_COLS_LENGTH) {
        // read byte
        cacheMemLcd[_counter++] = pgm_read_byte(&FONTS[character - 32][i++]);
    }

    // update position
    _counter++;

    // success
    return SSD1306_SUCCESS;
}

void SSD1306_DrawString(const char *str) {
    // init
    int i = 0;
    // loop through character of string
    while (str[i] != '\0') {
        // draw string
        SSD1306_DrawChar(str[i++]);
    }
}

uint8_t SSD1306_DrawPixel(uint8_t x, uint8_t y) {
    uint8_t page = 0;
    uint8_t pixel = 0;

    // if out of range
    if ((x > MAX_X) || (y > MAX_Y)) {
        // out of range
        return SSD1306_ERROR;
    }
    // find page (y / 8)
    page = y >> 3;
    // which pixel (y % 8)
    pixel = 1 << (y - (page << 3));
    // update counter
    _counter = x + (page << 7);
    // save pixel
    cacheMemLcd[_counter++] |= pixel;

    // success
    return SSD1306_SUCCESS;
}

uint8_t SSD1306_DrawLine(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2) {
    // determinant
    int16_t D;
    // deltas
    int16_t delta_x, delta_y;
    // steps
    int16_t trace_x = 1, trace_y = 1;

    // delta x
    delta_x = x2 - x1;
    // delta y
    delta_y = y2 - y1;

    // check if x2 > x1
    if (delta_x < 0) {
        // negate delta x
        delta_x = -delta_x;
        // negate step x
        trace_x = -trace_x;
    }

    // check if y2 > y1
    if (delta_y < 0) {
        // negate detla y
        delta_y = -delta_y;
        // negate step y
        trace_y = -trace_y;
    }

    // Bresenham condition for m < 1 (dy < dx)
    if (delta_y < delta_x) {
        // calculate determinant
        D = (delta_y << 1) - delta_x;
        // draw first pixel
        SSD1306_DrawPixel(x1, y1);
        // check if x1 equal x2
        while (x1 != x2) {
            // update x1
            x1 += trace_x;
            // check if determinant is positive
            if (D >= 0) {
                // update y1
                y1 += trace_y;
                // update determinant
                D -= 2 * delta_x;
            }
            // update deteminant
            D += 2 * delta_y;
            // draw next pixel
            SSD1306_DrawPixel(x1, y1);
        }
        // for m > 1 (dy > dx)
    } else {
        // calculate determinant
        D = delta_y - (delta_x << 1);
        // draw first pixel
        SSD1306_DrawPixel(x1, y1);
        // check if y2 equal y1
        while (y1 != y2) {
            // update y1
            y1 += trace_y;
            // check if determinant is positive
            if (D <= 0) {
                // update y1
                x1 += trace_x;
                // update determinant
                D += 2 * delta_y;
            }
            // update deteminant
            D -= 2 * delta_x;
            // draw next pixel
            SSD1306_DrawPixel(x1, y1);
        }
    }
    // success return
    return SSD1306_SUCCESS;
}

uint8_t SSD1306_ClearPixel(uint8_t x, uint8_t y) {
    // Ensure the coordinates are within bounds
    if (x >= MAX_X || y >= MAX_Y) {
        return SSD1306_ERROR; // Coordinates are out of bounds
    }

    // Calculate the page and bit within the page for the specified pixel
    uint8_t page = y >> 3;
    uint8_t bit = 1 << (y - (page << 3));

    // Update counter
    _counter = x + (page << 7);

    // Clear the corresponding bit (invert it) in the cache memory
    cacheMemLcd[_counter] &= ~bit;

    // Success
    return SSD1306_SUCCESS;
}

uint8_t SSD1306_InvertRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    // Ensure the coordinates are within bounds
    if (x >= MAX_X || y >= MAX_Y || (x + width) > MAX_X || (y + height) > MAX_Y) {
        return SSD1306_ERROR; // Coordinates are out of bounds
    }

    // Calculate the end coordinates of the rectangle
    uint8_t endX = x + width - 1;
    uint8_t endY = y + height - 1;

    // Loop through the specified rectangle area
    for (uint8_t i = x; i <= endX; i++) {
        for (uint8_t j = y; j <= endY; j++) {
            // Invert the pixel at coordinates (i, j)
            SSD1306_InvertPixel(i, j);
        }
    }

    // Update the display
    SSD1306_UpdateScreen(OLED_ADDRESS);

    // Success
    return SSD1306_SUCCESS;
}

uint8_t SSD1306_InvertPixel(uint8_t x, uint8_t y) {
    // Ensure the coordinates are within bounds
    if (x >= MAX_X || y >= MAX_Y) {
        return SSD1306_ERROR; // Coordinates are out of bounds
    }

    // Calculate the page and bit within the page for the specified pixel
    uint8_t page = y >> 3;
    uint8_t bit = 1 << (y - (page << 3));

    // Update counter
    _counter = x + (page << 7);

    // Invert the corresponding bit (XOR operation)
    cacheMemLcd[_counter] ^= bit;

    // Success
    return SSD1306_SUCCESS;
}

// The setDirection_OLED function is responsible for setting the direction
// information to be displayed on an OLED screen.
// It ensures that the direction information is correctly positioned and rendered on the screen.
void setDirection_OLED(const char *str) {
    SSD1306_SetPosition(DIRECTION_X_COORD, DIRECTION_Y_COORD);
    uint8_t i = 0;
    while (str[i] != '\0') {
        SSD1306_DrawChar(str[i++]);
    }
    while (i++ < DIRECTION_AREA) {
        SSD1306_DrawChar(' ');
    }
    SSD1306_UpdateScreen(OLED_ADDRESS);
}

// The setSpeed_OLED function is responsible for setting the speed
// information to be displayed on an OLED screen with unit of measurement(m/s).
// It ensures that the speed information is correctly positioned and rendered on the screen.
void setSpeed_OLED(uint8_t n) {
    char str[4];
    snprintf(str, 4, "%u", n);
    SSD1306_SetPosition(SPEED_X_COORD, SPEED_Y_COORD);
    uint8_t i = 0;
    while (str[i] != '\0') {
        SSD1306_DrawChar(str[i++]);
    }
    SSD1306_DrawString("m/s");
    i += 3;
    while (i++ < SPEED_AREA) {
        SSD1306_DrawChar(' ');
    }
    SSD1306_UpdateScreen(OLED_ADDRESS);
}

// The setTime_OLED function is responsible for setting the time
// information to be displayed on an OLED screen with unit of measurement(s).
// It ensures that the time information is correctly positioned and rendered on the screen.
void setTime_OLED(uint16_t n) {
    char str[6];
    snprintf(str, 6, "%u", n);
    SSD1306_SetPosition(TIME_X_COORD, TIME_Y_COORD);
    uint8_t i = 0;
    while (str[i] != '\0') {
        SSD1306_DrawChar(str[i++]);
    }
    SSD1306_DrawChar('s');
    i++;
    while (i++ < TIME_AREA) {
        SSD1306_DrawChar(' ');
    }
    SSD1306_UpdateScreen(OLED_ADDRESS);
}

// The setDistance_OLED function is responsible for setting the distance
// information to be displayed on an OLED screen with unit of measurement(cm).
// It ensures that the distance information is correctly positioned and rendered on the screen.
void setDistance_OLED(uint8_t n) {
    SSD1306_SetPosition(DIRECTION_X_COORD, DISTANCE_Y_COORD);

    char str[4]; // max 3 digits + null terminator
    sprintf(str, "%u", n);

    uint8_t i = 0;
    while (str[i] != '\0') {
        SSD1306_DrawChar(str[i++]);
    }
    SSD1306_DrawString("cm");
    i += 2;
    while (i++ < DISTANCE_AREA) {
        SSD1306_DrawChar(' ');
    }
    SSD1306_UpdateScreen(OLED_ADDRESS);
}

// The setPower_OLED function is responsible for setting the power
// information to be displayed on an OLED screen.
// It ensures that the power information is correctly positioned and rendered on the screen.
void setPower_OLED(const char *str) {
    SSD1306_SetPosition(POWER_X_COORD, POWER_Y_COORD);
    uint8_t i = 0;
    while (str[i] != '\0') {
        SSD1306_DrawChar(str[i++]);
    }
    while (i++ < POWER_AREA) {
        SSD1306_DrawChar(' ');
    }
    SSD1306_UpdateScreen(OLED_ADDRESS);
}

// The modeMenu function is responsible for displaying a menu of modes on an OLED screen
// and allowing the user to select one of them.
// It sets the SLAVE mode as default and return its value(1)
uint8_t modeMenu_OLED(void) {
    SSD1306_ClearScreen();
    SSD1306_SetPosition(0, 0);
    SSD1306_DrawString("SELECT MODE");
    SSD1306_SetPosition(15, 2);
    SSD1306_DrawString(str(SLAVE));
    SSD1306_SetPosition(15, 4);
    SSD1306_DrawString(str(CONTROLLED));
    SSD1306_SetPosition(15, 6);
    SSD1306_DrawString(str(AUTOMATIC));

    SSD1306_SetPosition(100, DEFAULT_MODE_POSITION);
    SSD1306_DrawChar('<');
    SSD1306_InvertRectangle(MIN__X, TEXT_SIZE * DEFAULT_MODE_POSITION - SIZE_OF_INVERSE, MAX__X, HEIGHT_OF_REGTANGLE);
    SSD1306_UpdateScreen(OLED_ADDRESS);
    return 1;
}

// The infoMenu function is responsible for displaying an information menu on an OLED screen.
// It presents information related to various parameters such as direction, speed, time, distance and power.
void infoMenu_OLED(void) {
    SSD1306_ClearScreen();
    // do not set position to 0,8 - results in overwrite of poweredOn variable (for some reason?)
    SSD1306_SetPosition(0, 0);
    SSD1306_DrawString("Direction:");
    SSD1306_SetPosition(0, 2);
    SSD1306_DrawString("Speed:");
    SSD1306_SetPosition(0, 4);
    SSD1306_DrawString("Time:");
    SSD1306_SetPosition(0, 6);
    SSD1306_DrawString("Distance:");
    setPower_OLED("OFF");
    SSD1306_UpdateScreen(OLED_ADDRESS);
}

// The setMode function is designed to change the operational mode on an OLED screen and
// return the updated mode value. It clears the previous mode and updates the screen with the latest one.
uint8_t setMode_OLED(uint8_t previous_mode, uint8_t vector) {
    uint8_t current_Mode = previous_mode + vector;
    SSD1306_InvertRectangle(MIN__X, TEXT_SIZE * 2 * previous_mode - SIZE_OF_INVERSE, MAX__X, HEIGHT_OF_REGTANGLE);
    SSD1306_SetPosition(100, 2 * previous_mode);
    SSD1306_DrawChar(' ');
    SSD1306_UpdateScreen(OLED_ADDRESS);
    if (current_Mode < SLAVE) {
        current_Mode = AUTOMATIC;
    } else if (current_Mode > AUTOMATIC) {
        current_Mode = SLAVE;
    }
    SSD1306_SetPosition(100, 2 * current_Mode);
    SSD1306_DrawChar('<');
    SSD1306_InvertRectangle(MIN__X, TEXT_SIZE * 2 * current_Mode - SIZE_OF_INVERSE, MAX__X, HEIGHT_OF_REGTANGLE);
    SSD1306_UpdateScreen(OLED_ADDRESS);
    return current_Mode;
}
