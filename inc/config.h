/*
 * config.h
 *
 *  Created on: 11 dec. 2014
 *      Author: Frans-Willem
 */

#ifndef CONFIG_H_
#define CONFIG_H_

//#define NANUYO
//#define _LED32_32_1  // 1 of 32x32
//#define _LED32_32_2 // 2 of 32x32
#define _LED64_64_1 // 1 of 64x64



#if defined (_LED32_32_2) || (_LED32_32_1)

//Information about individual panels
//How many pixels in width?
#define MATRIX_PANEL_WIDTH			32
//Number of scanrows. Should be a power of two.
#define MATRIX_PANEL_SCANROWS		16
#define MATRIX_MINIMUM_DISPLAY_TIME		1//50
/* HJ : 7 fixes flickering */
#define FRAMEBUFFER_MAXBITDEPTH			11


#elif defined (_LED64_64_1)

#define MATRIX_PANEL_WIDTH			64
#define MATRIX_PANEL_SCANROWS		32
#define MATRIX_MINIMUM_DISPLAY_TIME		50
/* HJ : 7 fixes flickering */
#define FRAMEBUFFER_MAXBITDEPTH			7//11


#else

#define MATRIX_PANEL_WIDTH			32
//The panels I use (16 pixels high) are 1/8th scan.
#define MATRIX_PANEL_SCANROWS		8
#define MATRIX_MINIMUM_DISPLAY_TIME		50
#endif

//How many buses and channels there are on the panel. For example on mine there's R1 G1 B1, R2 G2 B2, which means 3 channels (RGB), 2 buses (1,2)
#define MATRIX_PANEL_CHANNELS		3
#define MATRIX_PANEL_BUSES			2

//How many panels stuck together?
#if defined (_LED32_32_2)
#define MATRIX_PANELSW	2
#else
#define MATRIX_PANELSW	1
#endif

#define MATRIX_PANELSH	1/*3*/

//Output stuff

#define FRAMEBUFFER_BUFFERS				2


//Gamma & color correction
#define COLORCORR_GAMMA_MIN				1.0
#define COLORCORR_GAMMA_STEP			0.05
#define COLORCORR_GAMMA_COUNT			40
#define COLORCORR_GAMMA_DEFAULT			10

//Output
//GPIO for data and control
#define GPIO_DATA			GPIOC //GPIOD
#define GPIO_DATA_CLOCKCMD(x) 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, x)
//#define GPIO_DATA_CLOCKCMD(x) 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, x)

#define GPIO_CONTROL		GPIOB //GPIOE
#define GPIO_CONTROL_CLOCKCMD(x) 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, x)
//#define GPIO_CONTROL_CLOCKCMD(x) 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, x)

//Pins to use on control GPIO
#define GPIO_CONTROL_RowShift	0//2 // 2,3,4,5 for 16 scanrows, 2,3,4 for 8
#define GPIO_Pin_STB			GPIO_Pin_5//GPIO_Pin_4//GPIO_Pin_6

//Calculated, don't change this!
#define MATRIX_PANEL_HEIGHT (MATRIX_PANEL_SCANROWS * MATRIX_PANEL_BUSES)
#define MATRIX_WIDTH		(MATRIX_PANEL_WIDTH*MATRIX_PANELSW)
#define MATRIX_HEIGHT		(MATRIX_PANEL_HEIGHT*MATRIX_PANELSH)
#endif /* CONFIG_H_ */
