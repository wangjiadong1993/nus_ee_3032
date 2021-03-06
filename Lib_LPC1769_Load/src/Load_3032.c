#include "Load_3032.h"

void init_load(void)
{
	LPC_SC->PCONP |=  ADC_POWERON;

	// Turn on ADC peripheral clock
	LPC_SC->PCLKSEL0 &= ~(PCLK_ADC_MASK);
	LPC_SC->PCLKSEL0 |=  (3 << PCLK_ADC);

	// Set P0.23 to AD0.0 in PINSEL1
	LPC_PINCON->PINSEL1	|= SELECT_ADC1;
}

int read_load(void)
{
	int adval;
	// Start A/D conversion for on AD0.0
	LPC_ADC->ADCR = START_ADC | OPERATIONAL_ADC | SEL_AD1 ;

	do {
		adval = LPC_ADC->ADGDR;            // Read A/D Data Register
	} while ((adval & ADC_DONE_BIT) == 0); // Wait for end of A/D Conversion

	// Stop A/D Conversion
	LPC_ADC->ADCR &= ~(START_ADC | OPERATIONAL_ADC | SEL_AD1) ;

	// Extract AD0.0 value - 12 bit result in bits [15:4]
	adval = (adval >> 4) & 0x0FFF ;
	return adval;
}
