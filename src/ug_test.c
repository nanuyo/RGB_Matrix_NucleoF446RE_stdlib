/*
 * ug_test.c
 *
 *  Created on: 2016. 8. 17.
 *      Author: HJ Park
 */

#include "ugui.h"
#include "framebuffer.h"
#include "colorcorr.h"
#include "image565.h"
/* GUI structure */
UG_GUI gui;

void ugDrawPixelRGB(UG_S16 x, UG_S16 y, UG_COLOR color) {
	UG_S16 offset=(x + (y*MATRIX_WIDTH))*3;

	framebuffer_write(offset,colorcorr_lookup((color&0xff0000)>>16));
	framebuffer_write(offset+1,colorcorr_lookup((color&0x00ff00)>>8));
	framebuffer_write(offset+2,colorcorr_lookup(color&0xff));
}
#include "tm_stm32f4_delay.h"
#define DELAYMIL 2000
void ugTest(){

	UG_Init(&gui,(void(*)(UG_S16,UG_S16,UG_COLOR))ugDrawPixelRGB,MATRIX_WIDTH,MATRIX_HEIGHT);
	UG_FillScreen( C_WHITE );
	UG_FontSelect( &FONT_6X8 );
	UG_FillFrame( 0, 0, 10, 10, C_RED);
	UG_FillRoundFrame( 11, 0, 20,10, 5, C_YELLOW);
    UG_DrawMesh( 21, 0, 30, 10, C_GREEN);
	UG_DrawFrame( 0, 11, 10, 20, C_MAGENTA );
	UG_DrawRoundFrame( 11, 11, 20, 20, 5, C_BLUE );
    UG_DrawPixel(11 , 11, C_BLACK );

	UG_DrawCircle( 21, 11, 5, C_CYAN );
	UG_FillCircle( 0, 21, 5, C_VIOLET );

	UG_DrawArc( 11, 21, 5, 2, C_LIME );

    UG_DrawLine( 0, 0/*31*/, 31, 31, C_GOLD);

	UG_PutString(0, 23, "Test" );

	UG_PutChar( 'T', 0, 23, C_BROWN, C_ORANGE );

	if(MATRIX_WIDTH>32){
	UG_DrawBMP( MATRIX_PANEL_WIDTH, 0, &smartcorelogo );
	}
	else{
	framebuffer_swap();
	Delayms(DELAYMIL);
	UG_DrawBMP( 0, 0, &smartcorelogo );
	}

	framebuffer_swap();
}
