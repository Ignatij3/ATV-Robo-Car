/**
 * -------------------------------------------------------------------------------------+
 * @desc        SSD1306 OLED Driver
 * -------------------------------------------------------------------------------------+
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @date        06.10.2020
 * @update      21.11.2022
 * @file        ssd1306.h
 * @version     2.0.0
 * @tested      AVR Atmega328p
 *
 * @depend      string.h, font.h, twi.h
 * -------------------------------------------------------------------------------------+
 * @descr       Version 1.0.0 -> applicable for 1 display
 *              Version 2.0.0 -> rebuild to 'cacheMemLcd' array
 *              Version 3.0.0 -> simplified alphanumeric version for 1 display
 * -------------------------------------------------------------------------------------+
 * @usage       Basic Setup for OLED Display
 */

#ifndef __SSD1306_H__
#define __SSD1306_H__

#include <stdio.h>

#define OLED_ADDRESS SSD1306_ADDR // oled display

// Success / Error
// ------------------------------------------------------------------------------------
#define SSD1306_SUCCESS 0
#define SSD1306_ERROR 1

// Address definition
// ------------------------------------------------------------------------------------
#define SSD1306_ADDR 0x3C

// Command definition
// ------------------------------------------------------------------------------------
#define SSD1306_COMMAND 0x80        // Continuation bit=1, D/C=0; 1000 0000
#define SSD1306_COMMAND_STREAM 0x00 // Continuation bit=0, D/C=0; 0000 0000
#define SSD1306_DATA 0xC0           // Continuation bit=1, D/C=1; 1100 0000
#define SSD1306_DATA_STREAM 0x40    // Continuation bit=0, D/C=1; 0100 0000

#define SSD1306_SET_MUX_RATIO 0xA8
#define SSD1306_DISPLAY_OFFSET 0xD3
#define SSD1306_DISPLAY_ON 0xAF
#define SSD1306_DISPLAY_OFF 0xAE
#define SSD1306_DIS_ENT_DISP_ON 0xA4
#define SSD1306_DIS_IGNORE_RAM 0xA5
#define SSD1306_DIS_NORMAL 0xA6
#define SSD1306_DIS_INVERSE 0xA7
#define SSD1306_DEACT_SCROLL 0x2E
#define SSD1306_ACTIVE_SCROLL 0x2F
#define SSD1306_SET_START_LINE 0x40
#define SSD1306_MEMORY_ADDR_MODE 0x20
#define SSD1306_SET_COLUMN_ADDR 0x21
#define SSD1306_SET_PAGE_ADDR 0x22
#define SSD1306_SEG_REMAP 0xA0
#define SSD1306_SEG_REMAP_OP 0xA1
#define SSD1306_COM_SCAN_DIR 0xC0
#define SSD1306_COM_SCAN_DIR_OP 0xC8
#define SSD1306_COM_PIN_CONF 0xDA
#define SSD1306_SET_CONTRAST 0x81
#define SSD1306_SET_OSC_FREQ 0xD5
#define SSD1306_SET_CHAR_REG 0x8D
#define SSD1306_SET_PRECHARGE 0xD9
#define SSD1306_VCOM_DESELECT 0xDB

// Clear Color
// ------------------------------------------------------------------------------------
#define CLEAR_COLOR 0x00

// Init Status
// ------------------------------------------------------------------------------------
#define INIT_STATUS 0xFF

// AREA definition
// ------------------------------------------------------------------------------------
#define START_PAGE_ADDR 0
#define END_PAGE_ADDR 7 // 7 for 128x64, 3 for 128x32 version
#define START_COLUMN_ADDR 0
#define END_COLUMN_ADDR 127
#define RAM_X_END END_COLUMN_ADDR + 1
#define RAM_Y_END END_PAGE_ADDR + 1

#define CACHE_SIZE_MEM (1 + END_PAGE_ADDR) * (1 + END_COLUMN_ADDR)

#define MAX_X END_COLUMN_ADDR
#define MAX_Y (END_PAGE_ADDR + 1) * 8

#define DIRECTION_X_COORD 72
#define DIRECTION_Y_COORD 0
#define DIRECTION_AREA 7

#define SPEED_X_COORD 42
#define SPEED_Y_COORD 2
#define SPEED_AREA 5

#define TIME_X_COORD 36
#define TIME_Y_COORD 4
#define TIME_AREA 7

#define DISTANCE_X_COORD 66
#define DISTANCE_Y_COORD 6
#define DISTANCE_AREA 5

#define POWER_X_COORD 96
#define POWER_Y_COORD 3
#define POWER_AREA 3

#define MAX__X 127
#define MAX__Y 63
#define MIN__X 0
#define MIN__Y 0
#define TEXT_SIZE 8
#define DEFAULT_MODE_POSITION 2
#define SIZE_OF_INVERSE 3 // 3pixels from text to the border
#define HEIGHT_OF_REGTANGLE 14

uint8_t SSD1306_Init(uint8_t);
uint8_t SSD1306_Send_StartAndSLAW(uint8_t);
uint8_t SSD1306_Send_Command(uint8_t);
void SSD1306_ClearScreen(void);
uint8_t SSD1306_NormalScreen(uint8_t);
uint8_t SSD1306_InverseScreen(uint8_t);
uint8_t SSD1306_UpdateScreen(uint8_t);
uint8_t SSD1306_UpdatePosition(void);
void SSD1306_SetPosition(uint8_t, uint8_t);
uint8_t SSD1306_DrawChar(char);
void SSD1306_DrawString(const char *);
uint8_t SSD1306_DrawPixel(uint8_t, uint8_t);
uint8_t SSD1306_DrawLine(uint8_t, uint8_t, uint8_t, uint8_t);
uint8_t SSD1306_ClearPixel(uint8_t x, uint8_t y);
uint8_t SSD1306_InvertRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
uint8_t SSD1306_InvertPixel(uint8_t x, uint8_t y);
void setDirection_OLED(const char *str);
void setSpeed_OLED(uint8_t n);
void setTime_OLED(uint16_t n);
void setDistance_OLED(uint8_t n);
void setPower_OLED(const char *str);
uint8_t modeMenu_OLED(void);
void infoMenu_OLED(void);
uint8_t setMode_OLED(uint8_t previous_mode, uint8_t vector);

#endif
