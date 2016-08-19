/*
 * adafruit_gfx_test.c
 *
 *  Created on: 2016. 8. 17.
 *      Author: HJ Park
 */

#include "Adafruit_GFX.h"

#include "framebuffer.h"
#include "colorcorr.h"
#include "tm_stm32f4_delay.h"

GFX_Object Object;

#ifdef NANUYO
const char str[] = "www.nanuyo.com STM32 RGB LED Matrix";
#else
const char str[] = "Smartcore RGB LED Matrix";
#endif

int    textX   = MATRIX_WIDTH ,
       textMin = sizeof(str) * -12,
       hue     = 0;
int8_t ball[3][4] = {
  {  3,  0,  1,  1 }, // Initial X,Y pos & velocity for 3 bouncy balls
  { 17, 15,  1, -1 },
  { 27,  4, -1,  1 }
};

static const uint16_t ballcolor[3] = {
  0x00E0, // Green=1
  0x000f, // Blue=1
  0x1000  // Red=1
};


// 5/6/5 ->8/8/8
void adaDrawPixel(int32_t x, int32_t y, uint16_t color ) {
	int32_t offset=(x + (y*MATRIX_WIDTH))*3;
    uint8_t R8,G8,B8,R5,G6,B5;



    R5 = (color&0xf800)>>11;
    G6 = (color&0x07E0)>>5;
    B5 = color&0x1f;

#if 1
    R8 = ( R5 << 3) | (R5 >> 2);
    G8 = ( G6 << 3) | (G6 >> 2);
    B8 = ( B5 << 3) | (B5 >> 2);
#else
	R8 = ( R5 * 527 + 23 ) >> 6;
	G8 = ( G6 * 259 + 33 ) >> 6;
	B8 = ( B5 * 527 + 23 ) >> 6;
#endif
	framebuffer_write(offset,colorcorr_lookup(R8));
	framebuffer_write(offset+1,colorcorr_lookup(G8));
	framebuffer_write(offset+2,colorcorr_lookup(B8));
		/*
	framebuffer_write(offset,colorcorr_lookup((color&0xf800)>>8));
	framebuffer_write(offset+1,colorcorr_lookup((color&0x07E0)>>4));
	framebuffer_write(offset+2,colorcorr_lookup(color&0x1f));*/
}

#define pgm_read_word(addr) (*(const unsigned short *)(addr))
void adaTest(){

	Object.WIDTH    = MATRIX_WIDTH;
		Object.HEIGHT   = MATRIX_HEIGHT;
		Object.rotation  = 0;
		Object.cursor_y  = Object.cursor_x    = 0;
		Object.textsize  = 1;
		Object.textcolor = Object.textbgcolor = 0xFFFF;
		Object.wrap      = True;
		Object.drawPixel = (void(*)(int32_t,int32_t,uint16_t))adaDrawPixel;
	Adafruit_GFX_Init(&Object);

	//Adafruit_GFX_drawCircle(&Object, 0, 0, 5, 0xffff);
//	Adafruit_GFX_setTextSize(&Object, 2);
	//Adafruit_GFX_setTextColor(&Object, 0x07E0);
//	Adafruit_GFX_print(&Object, "Test");

	Adafruit_GFX_setTextSize(&Object, 1);
	Adafruit_GFX_setTextWrap(&Object, False); // Allow text to run off right edge

	for(int k=150;k>0;k--)
	{
	  uint8_t i;

	  // Clear background
	  Adafruit_GFX_fillScreen(&Object, 0);

	  // Bounce three balls around
	  for(i=0; i<3; i++) {
	    // Draw 'ball'
		  Adafruit_GFX_fillCircle(&Object, ball[i][0], ball[i][1], 5, pgm_read_word(&ballcolor[i]));
	    // Update X, Y position
	    ball[i][0] += ball[i][2];
	    ball[i][1] += ball[i][3];
	    // Bounce off edges
	    if((ball[i][0] == 0) || (ball[i][0] == (MATRIX_WIDTH - 1)))
	      ball[i][2] *= -1;
	    if((ball[i][1] == 0) || (ball[i][1] == (MATRIX_HEIGHT - 1)))
	      ball[i][3] *= -1;
	  }

	  // Draw big scrolly text on top
	  Adafruit_GFX_setTextColor(&Object, ColorHSV(hue, 255, 255, True));
	  Adafruit_GFX_setCursor(&Object, textX, 1);
	  Adafruit_GFX_print(&Object, str);


	  // Move text left (w/wrap), increase hue
	  if((--textX) < textMin) textX = MATRIX_WIDTH;
	  hue += 7;
	  if(hue >= 1536) hue -= 1536;

	  // Update display
	  		  framebuffer_swap();

	}

}

// Promote 3/3/3 RGB to Adafruit_GFX 5/6/5
uint16_t Color333(uint8_t r, uint8_t g, uint8_t b) {
  // RRRrrGGGgggBBBbb
  return ((r & 0x7) << 13) | ((r & 0x6) << 10) |
         ((g & 0x7) <<  8) | ((g & 0x7) <<  5) |
         ((b & 0x7) <<  2) | ((b & 0x6) >>  1);
}

void adaTestShape32x32() {
	Object.WIDTH    = MATRIX_WIDTH;
			Object.HEIGHT   = MATRIX_HEIGHT;
			Object.rotation  = 0;
			Object.cursor_y  = Object.cursor_x    = 0;
			Object.textsize  = 1;
			Object.textcolor = Object.textbgcolor = 0xFFFF;
			Object.wrap      = True;
			Object.drawPixel = (void(*)(int32_t,int32_t,uint16_t))adaDrawPixel;
		Adafruit_GFX_Init(&Object);
	//for(int k=500;k>0;k--)
		{

  // fill the screen with 'black'
  Adafruit_GFX_fillScreen(&Object, Color333(0, 0, 0));

  // draw some text!
  Adafruit_GFX_setCursor(&Object,1, 0);    // start at top left, with one pixel of spacing
  Adafruit_GFX_setTextSize(&Object, 1);     // size 1 == 8 pixels high
  Adafruit_GFX_setTextWrap(&Object, False);// Don't wrap at end of line - will do ourselves

  Adafruit_GFX_setTextColor(&Object,Color333(7,7,7));
#ifdef NANUYO
  Adafruit_GFX_println(&Object,"NA-NU-YO");
   Adafruit_GFX_println(&Object,"www.nanuyo.com");
#else
  Adafruit_GFX_println(&Object,"Smart");
  Adafruit_GFX_println(&Object,"core");
#endif

  // print each letter with a rainbow color
#ifndef _LED64_64_1
  Adafruit_GFX_setTextColor(&Object,Color333(7,0,0));
  Adafruit_GFX_write(&Object,'3');
  Adafruit_GFX_setTextColor(&Object,Color333(7,4,0));
  Adafruit_GFX_write(&Object,'2');
  Adafruit_GFX_setTextColor(&Object,Color333(7,7,0));
  Adafruit_GFX_write(&Object,'x');
  Adafruit_GFX_setTextColor(&Object,Color333(4,7,0));
  Adafruit_GFX_write(&Object,'3');
  Adafruit_GFX_setTextColor(&Object,Color333(0,7,0));
  Adafruit_GFX_write(&Object,'2');
#else
  Adafruit_GFX_setTextColor(&Object,Color333(7,0,0));
    Adafruit_GFX_write(&Object,'6');
    Adafruit_GFX_setTextColor(&Object,Color333(7,4,0));
    Adafruit_GFX_write(&Object,'4');
    Adafruit_GFX_setTextColor(&Object,Color333(7,7,0));
    Adafruit_GFX_write(&Object,'x');
    Adafruit_GFX_setTextColor(&Object,Color333(4,7,0));
    Adafruit_GFX_write(&Object,'6');
    Adafruit_GFX_setTextColor(&Object,Color333(0,7,0));
    Adafruit_GFX_write(&Object,'4');
#endif

  Adafruit_GFX_setTextColor(&Object,Color333(0,7,7));
  Adafruit_GFX_write(&Object,'*');
  Adafruit_GFX_setTextColor(&Object,Color333(0,4,7));
  Adafruit_GFX_write(&Object,'R');
  Adafruit_GFX_setTextColor(&Object,Color333(0,0,7));
  Adafruit_GFX_write(&Object,'G');
  Adafruit_GFX_setTextColor(&Object,Color333(4,0,7));
  Adafruit_GFX_write(&Object,'B');
  Adafruit_GFX_setTextColor(&Object,Color333(7,0,4));
  Adafruit_GFX_write(&Object,'*');


		}
	framebuffer_swap();
  // whew!
}
