/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _ADAFRUIT_GFX_H
#define _ADAFRUIT_GFX_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
/*
#if defined(STM32F40_41xxx)
#include "stm32f4xx.h"
#elif defined(STM32F10X_MD) || defined(STM32F10X_MD_VL)
#include "stm32f10x.h"
#endif
*/
/* Defines -------------------------------------------------------------------*/
#define swap(a, b) { int32_t t = a; a = b; b = t; }

/* Typedefs ------------------------------------------------------------------*/
typedef enum
{ False = 0,
  True = !False
} boolean;

typedef struct
{
	int32_t WIDTH, HEIGHT;			/* This is the 'raw' display w/h - never changes */
	int32_t _width, _height;		/* Display w/h as modified by current rotation */
	int32_t cursor_x, cursor_y;
	uint16_t textcolor, textbgcolor;
	uint8_t textsize, rotation;
	boolean wrap;					/* If set, 'wrap' text at right edge of display */

	void (*drawPixel)(int32_t, int32_t, uint16_t);	/* This MUST be set by the creator of a GFX_Object */
} GFX_Object;

/* Function prototypes -------------------------------------------------------*/
void Adafruit_GFX_Init(GFX_Object *Object);

void Adafruit_GFX_drawCircle(GFX_Object *Object, int32_t x0, int32_t y0, int32_t r, uint16_t color);
void Adafruit_GFX_drawCircleHelper(GFX_Object *Object, int32_t x0, int32_t y0, int32_t r, uint8_t cornername, uint16_t color);
void Adafruit_GFX_fillCircle(GFX_Object *Object, int32_t x0, int32_t y0, int32_t r, uint16_t color);
void Adafruit_GFX_fillCircleHelper(GFX_Object *Object, int32_t x0, int32_t y0, int32_t r, uint8_t cornername, int32_t delta, uint16_t color);

void Adafruit_GFX_drawLine(GFX_Object *Object, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t color);
void Adafruit_GFX_drawRect(GFX_Object *Object, int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color);

void Adafruit_GFX_drawFastVLine(GFX_Object *Object, int32_t x, int32_t y, int32_t h, uint16_t color);
void Adafruit_GFX_drawFastHLine(GFX_Object *Object, int32_t x, int32_t y, int32_t w, uint16_t color);

void Adafruit_GFX_fillRect(GFX_Object *Object, int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color);
void Adafruit_GFX_fillScreen(GFX_Object *Object, uint16_t color);

void Adafruit_GFX_drawRoundRect(GFX_Object *Object, int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint16_t color);
void Adafruit_GFX_fillRoundRect(GFX_Object *Object, int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint16_t color);

void Adafruit_GFX_drawTriangle(GFX_Object *Object, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint16_t color);
void Adafruit_GFX_fillTriangle(GFX_Object *Object, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint16_t color);

void Adafruit_GFX_print(GFX_Object *Object, const char *String);
void Adafruit_GFX_println(GFX_Object *Object, const char *String);
void Adafruit_GFX_printDigit(GFX_Object *Object, uint8_t Digit);
void Adafruit_GFX_printDigitln(GFX_Object *Object, uint8_t Digit);
void Adafruit_GFX_printHex(GFX_Object *Object, uint8_t Byte, boolean Prefix);
void Adafruit_GFX_printHexln(GFX_Object *Object, uint8_t Byte, boolean Prefix);
void Adafruit_GFX_printNumber(GFX_Object *Object, uint32_t Number, boolean Spaces);
void Adafruit_GFX_printNumberln(GFX_Object *Object, uint32_t Number, boolean Spaces);
void Adafruit_GFX_write(GFX_Object *Object, uint8_t c);
void Adafruit_GFX_drawChar(GFX_Object *Object, int32_t x, int32_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);

void Adafruit_GFX_setCursor(GFX_Object *Object, int32_t x, int32_t y);
void Adafruit_GFX_setTextSize(GFX_Object *Object, uint8_t s);
void Adafruit_GFX_setTextColor(GFX_Object *Object, uint16_t c);
void Adafruit_GFX_setTextColorAndBackground(GFX_Object *Object, uint16_t c, uint16_t b);
void Adafruit_GFX_setTextWrap(GFX_Object *Object, boolean w);

uint8_t Adafruit_GFX_getRotation(GFX_Object *Object);
void Adafruit_GFX_setRotation(GFX_Object *Object, uint8_t x);

int32_t Adafruit_GFX_width(GFX_Object *Object);
int32_t Adafruit_GFX_height(GFX_Object *Object);

void Adafruit_GFX_invertDisplay(GFX_Object *Object, boolean i);

#endif /* _ADAFRUIT_GFX_H */
