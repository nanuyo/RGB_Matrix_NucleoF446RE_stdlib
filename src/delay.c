/*
 * delay.c
 *
 *  Created on: 2016. 8. 16.
 *      Author: HJ Park
 */




//******************************************************************************

// From http://forums.arm.com/index.php?showtopic=13949

volatile unsigned int *DWT_CYCCNT   = (volatile unsigned int *)0xE0001004; //address of the register
volatile unsigned int *DWT_CONTROL  = (volatile unsigned int *)0xE0001000; //address of the register
volatile unsigned int *SCB_DEMCR        = (volatile unsigned int *)0xE000EDFC; //address of the register

//******************************************************************************

void EnableTiming(void)
{
    static int enabled = 0;

    if (!enabled)
    {
        *SCB_DEMCR = *SCB_DEMCR | 0x01000000;
        *DWT_CYCCNT = 0; // reset the counter
        *DWT_CONTROL = *DWT_CONTROL | 1 ; // enable the counter

        enabled = 1;
    }
}

//******************************************************************************

void TimingDelay(unsigned int tick)
{
    unsigned int start, current;

    EnableTiming();

    start = *DWT_CYCCNT;

    do
    {
        current = *DWT_CYCCNT;
    } while((current - start) < tick);
}

//******************************************************************************
