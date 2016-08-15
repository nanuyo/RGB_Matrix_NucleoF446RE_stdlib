/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <math.h>
#include "config.h"
#include "framebuffer.h"
#include "colorcorr.h"
#include "control_uart.h"
#include "config.h"
#include "testimage.h"
#include "matrix.h"

int main(void)
{
	framebuffer_init();
	colorcorr_init();
	control_uart_init();
	testimage_init();
	matrix_init();

	control_uart_loop();
	return 0;
}

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/