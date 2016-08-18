/*
This is the core graphics library for all our displays, providing a common
set of graphics primitives (points, lines, circles, etc.).  It needs to be
paired with a hardware-specific library for each display device we carry
(to handle the lower-level functions).

Adafruit invests time and resources providing this open source code, please
support Adafruit & open-source hardware by purchasing products from Adafruit!
 
Copyright (c) 2013 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#include "Adafruit_GFX.h"
#include "glcdfont.c"

#include <stdlib.h>		/* Needed for: abs() */


void Adafruit_GFX_Init(GFX_Object *Object)
{
	Object->_width    = Object->WIDTH;
	Object->_height   = Object->HEIGHT;
	Object->rotation  = 0;
	Object->cursor_y  = Object->cursor_x    = 0;
	Object->textsize  = 1;
	Object->textcolor = Object->textbgcolor = 0xFFFF;
	Object->wrap      = True;
}

/* Draw a circle outline */
void Adafruit_GFX_drawCircle(GFX_Object *Object, int32_t x0, int32_t y0, int32_t r, uint16_t color)
{
	int32_t f = 1 - r;
	int32_t ddF_x = 1;
	int32_t ddF_y = -2 * r;
	int32_t x = 0;
	int32_t y = r;

	Object->drawPixel(x0  , y0+r, color);
	Object->drawPixel(x0  , y0-r, color);
	Object->drawPixel(x0+r, y0  , color);
	Object->drawPixel(x0-r, y0  , color);

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		Object->drawPixel(x0 + x, y0 + y, color);
		Object->drawPixel(x0 - x, y0 + y, color);
		Object->drawPixel(x0 + x, y0 - y, color);
		Object->drawPixel(x0 - x, y0 - y, color);
		Object->drawPixel(x0 + y, y0 + x, color);
		Object->drawPixel(x0 - y, y0 + x, color);
		Object->drawPixel(x0 + y, y0 - x, color);
		Object->drawPixel(x0 - y, y0 - x, color);
	}
}

void Adafruit_GFX_drawCircleHelper(GFX_Object *Object, int32_t x0, int32_t y0, int32_t r, uint8_t cornername, uint16_t color)
{
	int32_t f     = 1 - r;
	int32_t ddF_x = 1;
	int32_t ddF_y = -2 * r;
	int32_t x     = 0;
	int32_t y     = r;

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;
		if (cornername & 0x4)
		{
			Object->drawPixel(x0 + x, y0 + y, color);
			Object->drawPixel(x0 + y, y0 + x, color);
		}
		if (cornername & 0x2)
		{
			Object->drawPixel(x0 + x, y0 - y, color);
			Object->drawPixel(x0 + y, y0 - x, color);
		}
		if (cornername & 0x8)
		{
			Object->drawPixel(x0 - y, y0 + x, color);
			Object->drawPixel(x0 - x, y0 + y, color);
		}
		if (cornername & 0x1)
		{
			Object->drawPixel(x0 - y, y0 - x, color);
			Object->drawPixel(x0 - x, y0 - y, color);
		}
	}
}

void Adafruit_GFX_fillCircle(GFX_Object *Object, int32_t x0, int32_t y0, int32_t r, uint16_t color)
{
	Adafruit_GFX_drawFastVLine(Object, x0, y0-r, 2*r+1, color);
	Adafruit_GFX_fillCircleHelper(Object, x0, y0, r, 3, 0, color);
}

/* Used to do circles and roundrects */
void Adafruit_GFX_fillCircleHelper(GFX_Object *Object, int32_t x0, int32_t y0, int32_t r, uint8_t cornername, int32_t delta, uint16_t color)
{
	int32_t f     = 1 - r;
	int32_t ddF_x = 1;
	int32_t ddF_y = -2 * r;
	int32_t x     = 0;
	int32_t y     = r;

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;

		if (cornername & 0x1)
		{
			Adafruit_GFX_drawFastVLine(Object, x0+x, y0-y, 2*y+1+delta, color);
			Adafruit_GFX_drawFastVLine(Object, x0+y, y0-x, 2*x+1+delta, color);
		}
		if (cornername & 0x2)
		{
			Adafruit_GFX_drawFastVLine(Object, x0-x, y0-y, 2*y+1+delta, color);
			Adafruit_GFX_drawFastVLine(Object, x0-y, y0-x, 2*x+1+delta, color);
		}
	}
}

/* Bresenham's algorithm - thx wikpedia */
void Adafruit_GFX_drawLine(GFX_Object *Object, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t color)
{
	int32_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep)
	{
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1)
	{
		swap(x0, x1);
		swap(y0, y1);
	}

	int32_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int32_t err = dx / 2;
	int32_t ystep;

	if (y0 < y1)
	{
		ystep = 1;
	}
	else
	{
		ystep = -1;
	}

	for (; x0<=x1; x0++)
	{
		if (steep)
		{
			Object->drawPixel(y0, x0, color);
		}
		else
		{
			Object->drawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0)
		{
			y0 += ystep;
			err += dx;
		}
	}
}

/* Draw a rectangle */
void Adafruit_GFX_drawRect(GFX_Object *Object, int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color)
{
	Adafruit_GFX_drawFastHLine(Object, x, y, w, color);
	Adafruit_GFX_drawFastHLine(Object, x, y+h-1, w, color);
	Adafruit_GFX_drawFastVLine(Object, x, y, h, color);
	Adafruit_GFX_drawFastVLine(Object, x+w-1, y, h, color);
}

void Adafruit_GFX_drawFastVLine(GFX_Object *Object, int32_t x, int32_t y, int32_t h, uint16_t color)
{
	// TODO: Subclassing???
	// Update in subclasses if desired!
	Adafruit_GFX_drawLine(Object, x, y, x, y+h-1, color);
}

void Adafruit_GFX_drawFastHLine(GFX_Object *Object, int32_t x, int32_t y, int32_t w, uint16_t color)
{
	// TODO: Subclassing???
	// Update in subclasses if desired!
	Adafruit_GFX_drawLine(Object, x, y, x+w-1, y, color);
}

void Adafruit_GFX_fillRect(GFX_Object *Object, int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color)
{
	// TODO: Subclassing???
	// Update in subclasses if desired!
	for (int32_t i=x; i<x+w; i++)
	{
		Adafruit_GFX_drawFastVLine(Object, i, y, h, color);
	}
}

void Adafruit_GFX_fillScreen(GFX_Object *Object, uint16_t color)
{
	Adafruit_GFX_fillRect(Object, 0, 0, Object->_width, Object->_height, color);
}

/* Draw a rounded rectangle */
void Adafruit_GFX_drawRoundRect(GFX_Object *Object, int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint16_t color)
{
	/* smarter version */
	Adafruit_GFX_drawFastHLine(Object, x+r  , y    , w-2*r, color); // Top
	Adafruit_GFX_drawFastHLine(Object, x+r  , y+h-1, w-2*r, color); // Bottom
	Adafruit_GFX_drawFastVLine(Object, x    , y+r  , h-2*r, color); // Left
	Adafruit_GFX_drawFastVLine(Object, x+w-1, y+r  , h-2*r, color); // Right
	/* draw four corners */
	Adafruit_GFX_drawCircleHelper(Object, x+r    , y+r    , r, 1, color);
	Adafruit_GFX_drawCircleHelper(Object, x+w-r-1, y+r    , r, 2, color);
	Adafruit_GFX_drawCircleHelper(Object, x+w-r-1, y+h-r-1, r, 4, color);
	Adafruit_GFX_drawCircleHelper(Object, x+r    , y+h-r-1, r, 8, color);
}

/* Fill a rounded rectangle */
void Adafruit_GFX_fillRoundRect(GFX_Object *Object, int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint16_t color)
{
	// smarter version
	Adafruit_GFX_fillRect(Object, x+r, y, w-2*r, h, color);

	// draw four corners
	Adafruit_GFX_fillCircleHelper(Object, x+w-r-1, y+r, r, 1, h-2*r-1, color);
	Adafruit_GFX_fillCircleHelper(Object, x+r    , y+r, r, 2, h-2*r-1, color);
}

/* Draw a triangle */
void Adafruit_GFX_drawTriangle(GFX_Object *Object, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint16_t color)
{
	Adafruit_GFX_drawLine(Object, x0, y0, x1, y1, color);
	Adafruit_GFX_drawLine(Object, x1, y1, x2, y2, color);
	Adafruit_GFX_drawLine(Object, x2, y2, x0, y0, color);
}

/* Fill a triangle */
void Adafruit_GFX_fillTriangle(GFX_Object *Object, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint16_t color)
{
	int32_t a, b, y, last;

	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1) {
		swap(y0, y1); swap(x0, x1);
	}
	if (y1 > y2) {
		swap(y2, y1); swap(x2, x1);
	}
	if (y0 > y1) {
		swap(y0, y1); swap(x0, x1);
	}

	if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
		a = b = x0;
		if(x1 < a)      a = x1;
		else if(x1 > b) b = x1;
		if(x2 < a)      a = x2;
		else if(x2 > b) b = x2;
		Adafruit_GFX_drawFastHLine(Object, a, y0, b-a+1, color);
		return;
	}

	int32_t
	dx01 = x1 - x0,
	dy01 = y1 - y0,
	dx02 = x2 - x0,
	dy02 = y2 - y0,
	dx12 = x2 - x1,
	dy12 = y2 - y1,
	sa   = 0,
	sb   = 0;

	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).
	if (y1 == y2) last = y1;   // Include y1 scanline
	else         last = y1-1; // Skip it

	for (y=y0; y<=last; y++) {
		a   = x0 + sa / dy01;
		b   = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		/* longhand:
		a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
		b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		*/
		if (a > b) swap(a,b);
		Adafruit_GFX_drawFastHLine(Object, a, y, b-a+1, color);
	}

	// For lower part of triangle, find scanline crossings for segments
	// 0-2 and 1-2.  This loop is skipped if y1=y2.
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for (; y<=y2; y++) {
		a   = x1 + sa / dy12;
		b   = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		/* longhand:
		a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
		b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		*/
		if (a > b) swap(a,b);
		Adafruit_GFX_drawFastHLine(Object, a, y, b-a+1, color);
	}
}

/* TODO: Not implemented yet */
//void Adafruit_GFX_drawBitmap(int32_t x, int32_t y, const uint8_t *bitmap, int32_t w, int32_t h, uint16_t color)
//{
//  int32_t i, j, byteWidth = (w + 7) / 8;
//
//	for(j=0; j<h; j++) {
//		for(i=0; i<w; i++ ) {
//		if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
//			drawPixel(x+i, y+j, color);
//		}
//		}
//	}
//}

void Adafruit_GFX_print(GFX_Object *Object, const char *String)
{
	while (*String != 0x00)
	{
		Adafruit_GFX_write(Object, *String);
		String++;
	}
}

void Adafruit_GFX_println(GFX_Object *Object, const char *String)
{
	Adafruit_GFX_print(Object, String);
	Adafruit_GFX_print(Object, "\n");
}

/**
 * @brief	Write a single digit as a character
 * @param	Object: Pointer to the object
 * @param	Digit: The digit to write. Can be 0 to 15 -> 0-F as characters
 * @retval	None
 */
void Adafruit_GFX_printDigit(GFX_Object *Object, uint8_t Digit)
{
	if (Digit < 10)
	{
		uint8_t character = 48 + Digit;
		Adafruit_GFX_write(Object, character);
	}
	else if (Digit < 16)
	{
		uint8_t character = 55 + Digit;
		Adafruit_GFX_write(Object, character);
	}
}

void Adafruit_GFX_printDigitln(GFX_Object *Object, uint8_t Digit)
{
	Adafruit_GFX_printDigit(Object, Digit);
	Adafruit_GFX_print(Object, "\n");
}

void Adafruit_GFX_printHex(GFX_Object *Object, uint8_t Byte, boolean Prefix)
{
	if (Prefix) Adafruit_GFX_print(Object, "0x");
	Adafruit_GFX_printDigit(Object, (Byte >> 4) & 0xF);
	Adafruit_GFX_printDigit(Object, Byte & 0xF);
}

void Adafruit_GFX_printHexln(GFX_Object *Object, uint8_t Byte, boolean Prefix)
{
	Adafruit_GFX_printHex(Object, Byte, Prefix);
	Adafruit_GFX_print(Object, "\n");
}

void Adafruit_GFX_printNumber(GFX_Object *Object, uint32_t Number, boolean Spaces)
{
	uint8_t ones, tens, hundreds, thousands, tenThousands, hundredThousands, millions, tenMillions, hundredMillions, billions;

	billions = Number / 1000000000;
	Number = Number % 1000000000;

	hundredMillions = Number / 100000000;
	Number = Number % 100000000;

	tenMillions = Number / 10000000;
	Number = Number % 10000000;

	millions = Number / 1000000;
	Number = Number % 1000000;

	hundredThousands = Number / 100000;
	Number = Number % 100000;

	tenThousands = Number / 10000;
	Number = Number % 10000;

	thousands = Number / 1000;
	Number = Number % 1000;

	hundreds = Number / 100;
	Number = Number % 100;

	tens = Number / 10;
	Number = Number % 10;

	ones = Number;

	if (billions)
	{
		Adafruit_GFX_printDigit(Object, (uint8_t)billions);
		if (Spaces) Adafruit_GFX_print(Object, " ");
	}
	if (hundredMillions || billions)
	{
		Adafruit_GFX_printDigit(Object, (uint8_t)hundredMillions);
	}
	if (tenMillions || hundredMillions || billions)
	{
		Adafruit_GFX_printDigit(Object, (uint8_t)tenMillions);
	}
	if (millions || tenMillions || hundredMillions || billions)
	{
		Adafruit_GFX_printDigit(Object, (uint8_t)millions);
		if (Spaces) Adafruit_GFX_print(Object, " ");
	}
	if (hundredThousands || millions || tenMillions || hundredMillions || billions)
	{
		Adafruit_GFX_printDigit(Object, (uint8_t)hundredThousands);
	}
	if (tenThousands || hundredThousands || millions || tenMillions || hundredMillions || billions)
	{
		Adafruit_GFX_printDigit(Object, (uint8_t)tenThousands);
	}
	if (thousands || tenThousands || hundredThousands || millions || tenMillions || hundredMillions || billions)
	{
		Adafruit_GFX_printDigit(Object, (uint8_t)thousands);
		if (Spaces) Adafruit_GFX_print(Object, " ");
	}
	if (hundreds || thousands || tenThousands || hundredThousands || millions || tenMillions || hundredMillions || billions)
	{
		Adafruit_GFX_printDigit(Object, (uint8_t)hundreds);
	}
	if (tens || hundreds || thousands || tenThousands || hundredThousands || millions || tenMillions || hundredMillions || billions)
	{
		Adafruit_GFX_printDigit(Object, (uint8_t)tens);
	}
	Adafruit_GFX_printDigit(Object, (uint8_t)ones);
}

void Adafruit_GFX_printNumberln(GFX_Object *Object, uint32_t Number, boolean Spaces)
{
	Adafruit_GFX_printNumber(Object, Number, Spaces);
	Adafruit_GFX_print(Object, "\n");
}

void Adafruit_GFX_write(GFX_Object *Object, uint8_t c)
{
	if (c == '\n')
	{
		Object->cursor_y += Object->textsize*8;
		Object->cursor_x  = 0;
	}
	else if (c == '\r') {
	// skip em
	}
	else
	{
		Adafruit_GFX_drawChar(Object, Object->cursor_x, Object->cursor_y, c, Object->textcolor, Object->textbgcolor, Object->textsize);
		Object->cursor_x += Object->textsize*6;
		if (Object->wrap && (Object->cursor_x > (Object->_width - Object->textsize*6)))
		{
			Object->cursor_y += Object->textsize*8;
			Object->cursor_x = 0;
		}
	}
}

/* Draw a character */
void Adafruit_GFX_drawChar(GFX_Object *Object, int32_t x, int32_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size)
{
	if ((x >= Object->_width)		||	// Clip right
	   (y >= Object->_height)		||	// Clip bottom
	   ((x + 6 * size - 1) < 0)		||	// Clip left
	   ((y + 8 * size - 1) < 0))		// Clip top
		return;

	for (int8_t i=0; i<6; i++ )
	{
		uint8_t line;
		if (i == 5)
			line = 0x0;
		else
			// TODO: was progmem here before so test if this actually works!
			line = font[(c*5)+i];
		for (int8_t j = 0; j<8; j++)
		{
			if (line & 0x1)
			{
				if (size == 1) // default size
					Object->drawPixel(x+i, y+j, color);
				else {  // big size
					Adafruit_GFX_fillRect(Object, x+(i*size), y+(j*size), size, size, color);
				}
			}
			else if (bg != color)
			{
				if (size == 1) // default size
					Object->drawPixel(x+i, y+j, bg);
				else {  // big size
					Adafruit_GFX_fillRect(Object, x+i*size, y+j*size, size, size, bg);
				}
			}
			line >>= 1;
		}
	}
}

void Adafruit_GFX_setCursor(GFX_Object *Object, int32_t x, int32_t y)
{
	Object->cursor_x = x;
	Object->cursor_y = y;
}

void Adafruit_GFX_setTextSize(GFX_Object *Object, uint8_t s)
{
	Object->textsize = (s > 0) ? s : 1;
}

void Adafruit_GFX_setTextColor(GFX_Object *Object, uint16_t c)
{
	/*
	 * For 'transparent' background, we'll set the bg
	 * to the same as fg instead of using a flag
	 */
	Object->textcolor = Object->textbgcolor = c;
}

void Adafruit_GFX_setTextColorAndBackground(GFX_Object *Object, uint16_t c, uint16_t b)
{
	Object->textcolor   = c;
	Object->textbgcolor = b;
}

void Adafruit_GFX_setTextWrap(GFX_Object *Object, boolean w)
{
	Object->wrap = w;
}

uint8_t Adafruit_GFX_getRotation(GFX_Object *Object)
{
	return Object->rotation;
}

void Adafruit_GFX_setRotation(GFX_Object *Object, uint8_t x)
{
	Object->rotation = (x & 3);
	switch (Object->rotation) {
		case 0:
		case 2:
			Object->_width  = Object->WIDTH;
			Object->_height = Object->HEIGHT;
			break;
		case 1:
		case 3:
			Object->_width  = Object->HEIGHT;
			Object->_height = Object->WIDTH;
			break;
	}
}

/* Return the size of the display (per current rotation) */
int32_t Adafruit_GFX_width(GFX_Object *Object)
{
	return Object->_width;
}

int32_t Adafruit_GFX_height(GFX_Object *Object)
{
	return Object->_height;
}

void Adafruit_GFX_invertDisplay(GFX_Object *Object, boolean i)
{
  // Do nothing, must be subclassed if supported
}

