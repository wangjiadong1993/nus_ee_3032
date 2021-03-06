#include "Temp_3032.h"

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

void init_temp(void)
{
	LPC_SC->PCONP |=  ADC_POWERON;

	// Turn on ADC peripheral clock
	LPC_SC->PCLKSEL0 &= ~(PCLK_ADC_MASK);
	LPC_SC->PCLKSEL0 |=  (3 << PCLK_ADC);

	// Set P0.23 to AD0.0 in PINSEL1
	LPC_PINCON->PINSEL1	|= SELECT_ADC0;

	init_gpio_2_8();
}

int read_temp(void)
{
	int adval;
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
