
#include "Load_3032.h"

void init_load(void)
{
	LPC_SC->PCONP |=  ADC_POWERON;

	// Turn on ADC peripheral clock
	LPC_SC->PCLKSEL0 &= ~(PCLK_ADC_MASK);
	LPC_SC->PCLKSEL0 |=  (3 << PCLK_ADC);

	// Set P0.23 to AD0.1 in PINSEL1
	LPC_PINCON->PINSEL1	|= SELECT_ADC1;
	//set P		 to AD0.2 in PINSEL2
	LPC_PINCON->PINSEL1	|= SELECT_ADC2;
	//set P		 to AD0.2 in PINSEL3
	LPC_PINCON->PINSEL1	|= SELECT_ADC3;
}

int read_load_1(void)
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
int read_load_2(void)
{
	int adval;
	// Start A/D conversion for on AD0.0
	LPC_ADC->ADCR = START_ADC | OPERATIONAL_ADC | SEL_AD2 ;

	do {
		adval = LPC_ADC->ADGDR;            // Read A/D Data Register
	} while ((adval & ADC_DONE_BIT) == 0); // Wait for end of A/D Conversion

	// Stop A/D Conversion
	LPC_ADC->ADCR &= ~(START_ADC | OPERATIONAL_ADC | SEL_AD2) ;

	// Extract AD0.0 value - 12 bit result in bits [15:4]
	adval = (adval >> 4) & 0x0FFF ;
	return adval;
}
int read_load_3(void)
{
	int adval;
	// Start A/D conversion for on AD0.0
	LPC_ADC->ADCR = START_ADC | OPERATIONAL_ADC | SEL_AD3 ;

	do {
		adval = LPC_ADC->ADGDR;            // Read A/D Data Register
	} while ((adval & ADC_DONE_BIT) == 0); // Wait for end of A/D Conversion

	// Stop A/D Conversion
	LPC_ADC->ADCR &= ~(START_ADC | OPERATIONAL_ADC | SEL_AD3) ;

	// Extract AD0.0 value - 12 bit result in bits [15:4]
	adval = (adval >> 4) & 0x0FFF ;
	return adval;
}
