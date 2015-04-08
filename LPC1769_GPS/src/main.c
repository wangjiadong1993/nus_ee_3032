//*****************************************************************************
// LPC1769_CMSIS_MCU_blinky
// ==========
// CK Tham, ECE, NUS
// June 2011
// adapted from Code Red LPCXpresso1768_systick
// to use Lib_MCU : PINSEL and GPIO
// see readme.txt file
//*****************************************************************************

// CMSIS headers required for setting up SysTick Timer
#include "LPC17xx.h"

// Lib_MCU header files
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_uart.h"
#include "string.h"
#include "stdio.h"


volatile uint32_t msTicks; // counter for 1ms SysTicks
char str[1024] = "0";
int num = 0;
float time_g = 0.0;
// ****************
//  SysTick_Handler - just increment SysTick counter
void SysTick_Handler(void) {
  msTicks++;
}

// ****************
// systick_delay - creates a delay of the appropriate number of Systicks (happens every 1 ms)
__INLINE static void systick_delay (uint32_t delayTicks) {
  uint32_t currentTicks;

  currentTicks = msTicks;	// read current tick counter
  // Now loop until required number of ticks passes
  while ((msTicks - currentTicks) < delayTicks);
}

void initial_gpio()
{
	PINSEL_CFG_Type PinCfg;
	PinCfg.Funcnum = 0;
	PinCfg.Pinnum = 9;
	PinCfg.Portnum = 0; //P0.0 fun2 is TXD3
	PINSEL_ConfigPin(&PinCfg);
	GPIO_SetDir(0, 1<<9, 1);
	GPIO_SetValue(0, 1<<9);
}

void pinsel_uart2(void)
{
	PINSEL_CFG_Type PinCfg;
	PinCfg.Funcnum = 1;
	PinCfg.Pinnum = 10;
	PinCfg.Portnum = 0; //P0.10 fun2 is TXD2
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 11; //P0.11 fun2 is RXD2
	PINSEL_ConfigPin(&PinCfg);
}


// ****************
int main(void) {
	

	initial_gpio();
	//init_uart();
	//NVIC_EnableIRQ(UART2_IRQn);
	while(1)
	{
		//printf("come back");
		printf("the input is %d", GPIO_ReadValue(2)>>4&0x);
	}

	return 0 ;
}
