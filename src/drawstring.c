/*
 * drawstring.c
 *
 *  Created on: 2016. 8. 16.
 *      Author: HJ Park
 */


#include "framebuffer.h"
#include "colorcorr.h"
#include "fonts.h"
#include "tm_stm32f4_delay.h"

void drawPixelRGB(unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b);

void fillScreen(uint32_t color)
{
	for (int x=0; x<MATRIX_WIDTH; x++) {
			for (int y=0; y<MATRIX_HEIGHT; y++) {
				drawPixelRGB(x,y, (color&0xff0000)>>16,(color&0x00ff00)>>8, (color&0xff));
			}
	}
}

void drawPixelRGB(unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b) {
	unsigned int offset=(x + (y*MATRIX_WIDTH))*3;
	framebuffer_write(offset,colorcorr_lookup(r));
	framebuffer_write(offset+1,colorcorr_lookup(g));
	framebuffer_write(offset+2,colorcorr_lookup(b));
}

void drawPixelRGBBuff(unsigned int x, unsigned int y, uint8_t* rgb) {
	unsigned int offset=(x + (y*MATRIX_WIDTH))*3;
	framebuffer_write(offset,colorcorr_lookup(rgb[0]));
	framebuffer_write(offset+1,colorcorr_lookup(rgb[1]));
	framebuffer_write(offset+2,colorcorr_lookup(rgb[2]));
}

struct color foreground_color;
struct color background_color;
struct color{
	uint8_t R;
	uint8_t G;
	uint8_t B;
};
void calculateLEDString(char *s, int32_t x, int32_t y, sFONT font)
{
	int32_t i,l,j;

	while(*s)
	{
		for(i=0;i<font.Height;i++)
		{
			l=font.table[((*s)-32)*(font.Height)+i];

			for(j=0;j<font.Width;j++)
			{
				if(MATRIX_WIDTH>x+(font.Width-1-j) && x+(font.Width-1-j)>=0)
				{//check width
					if((l>>j)&0x1)
					{
						drawPixelRGB(x+(font.Width-1-j),y+i, foreground_color.R, foreground_color.G, foreground_color.B);
					}
					else
					{
						drawPixelRGB(x+(font.Width-1-j),y+i, background_color.R, background_color.G, background_color.B);
					}
				}
			}
		}
		x+=font.Width;
		s++;
	}
}

void calculateScrollText(char *s, sFONT font, uint8_t textlength, uint8_t y, int32_t *x)
{
	if((*x)<textlength*-8)
	{
		(*x)=MATRIX_WIDTH+8*textlength;
		for(;textlength>0;textlength--)
		{
			calculateLEDString(" ",textlength*8,y,font);

			//Delayms(10);
		}
		calculateLEDString(" ",textlength*-8,y,font);
		//Delayms(10);
	}
	calculateLEDString(s,(*x),y,font);
	//Delayms(10);
	(*x)--;
}

void drawTestImage() {
	foreground_color.R=0xff;
	  foreground_color.G=0xff;
	  foreground_color.B=0xff;

	  background_color.R=0x0;
	  background_color.G=0x0;
	  background_color.B=0x0;

	fillScreen(0xffffff);
	//calculateLEDString("SMART", 0, 0,Font8x8);
	framebuffer_swap();
}

void drawStringLoop() {
	 int32_t scroll1=0,scroll2=0;

	 fillScreen(0);

	for(int i = 0; i<300; i++)
	   {
		 foreground_color.R=0x00;
	 	 foreground_color.G=0xff;
	 	 foreground_color.B=0x00;

	 	 background_color.R=0xff;
	 	 background_color.G=0x0;
	 	 background_color.B=0xff;
if(MATRIX_WIDTH>32)
	 calculateLEDString("12345678",0,0,Font8x8);
else
	 calculateLEDString("SMART",0,0,Font8x8);

		 foreground_color.R=0x00;
		 	 foreground_color.G=0x00;
		 	 foreground_color.B=0xff;

		 	 background_color.R=0xff;
		 	 background_color.G=0xff;
		 	 background_color.B=0x0;

if(MATRIX_WIDTH>32)
	 calculateLEDString("ABCDEFGH",0,8,Font8x8);
else
		 calculateLEDString("CORE",0,8,Font8x8);


	     foreground_color.R=0xff;
	     	 foreground_color.G=0x00;
	     	 foreground_color.B=0x00;

	     	 background_color.R=0x0;
	     	 background_color.G=0xff;
	     	 background_color.B=0xff;

if(MATRIX_WIDTH>32)
	 calculateLEDString("abcdefgh",0,16,Font8x8);
else
		 calculateLEDString("CORE",0,16,Font8x8);

	 	 foreground_color.R=0xff;
	 	 	 foreground_color.G=0xff;
	 	 	 foreground_color.B=0xff;

	 	 	 background_color.R=0x0;
	 	 	 background_color.G=0x0;
	 	 	 background_color.B=0x0;

	 	  calculateScrollText("SMARTCORE",Font8x8,10,24,&scroll1);
	 	  //calculateScrollText("Smartcore",Font8x8,10,8,&scroll1);

	 	//  calculateScrollText("www.smartcoreinc.com",Font8x8,20,16,&scroll2);
	 	  //calculateScrollText("Imaging technology",Font8x8,20,24,&scroll2);

	 	  //calculateScrollText("Smart Core",Font8x8,10,8,&scroll1);
	 	 if(MATRIX_HEIGHT>32)
	 	 {
	 	    calculateLEDString("abcdefgh",0,32,Font8x8);
	 	   calculateLEDString("abcdefgh",0,32+8,Font8x8);
	 	  calculateLEDString("abcdefgh",0,32+16,Font8x8);
	 	  calculateLEDString("abcdefgh",0,32+24,Font8x8);
	 	 }

	 	 framebuffer_swap();
	   }
}

