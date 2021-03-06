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
#define ADC_POWERON (1 << 12)
#define PCLK_ADC 24
#define PCLK_ADC_MASK (3 << 24)
#define SELECT_ADC0 (0x1<<14)
#define START_ADC (1<<24)
#define OPERATIONAL_ADC (1 << 21)
#define SEL_AD0 (1 <<0)
#define ADC_DONE_BIT	(1 << 31)
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

void init_temp(void)
{
	LPC_SC->PCONP |=  ADC_POWERON;

	// Turn on ADC peripheral clock
	LPC_SC->PCLKSEL0 &= ~(PCLK_ADC_MASK);
	LPC_SC->PCLKSEL0 |=  (3 << PCLK_ADC);

	// Set P0.23 to AD0.0 in PINSEL1
	LPC_PINCON->PINSEL1	|= SELECT_ADC0;
}
void init_gpio_2_8(void)
{
	PINSEL_CFG_Type PinCfg;
	PinCfg.Funcnum = 0;
	PinCfg.Pinnum = 8;
	PinCfg.Portnum = 2; //P2.8 fun2 is TXD3
	PINSEL_ConfigPin(&PinCfg);
	GPIO_SetDir(2, 1<<8, 1);
	GPIO_SetValue(2, 1<<8);
}
int read_temp(void)
{
	int adval, adval_64;
	// Start A/D conversion for on AD0.0
	LPC_ADC->ADCR = START_ADC | OPERATIONAL_ADC | SEL_AD0 ;

	do {
		adval = LPC_ADC->ADGDR;            // Read A/D Data Register
	} while ((adval & ADC_DONE_BIT) == 0); // Wait for end of A/D Conversion

	// Stop A/D Conversion
	LPC_ADC->ADCR &= ~(START_ADC | OPERATIONAL_ADC | SEL_AD0) ;

	// Extract AD0.0 value - 12 bit result in bits [15:4]
	adval = (adval >> 4) & 0x0FFF ;
	return adval;
}

// ****************
int main(void) {
	int adval;
	int low = 0;
	init_temp();
	init_gpio_2_8();
	if (SysTick_Config(SystemCoreClock / 1000)) {
		while (1);  // Capture error
	}
	while(1) {

		adval=read_temp();
		printf("%d\n", adval);
		if(adval>2300)
		{
			GPIO_SetValue(2, 1<<8);
		}else if(adval<2250)
		{
			GPIO_ClearValue(2, 1<<8);
		}
		systick_delay(200);
		}
	return 0 ;
}
