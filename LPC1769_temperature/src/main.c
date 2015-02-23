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

void initial_gpio()
{
	PINSEL_CFG_Type PinCfg;
	PinCfg.Funcnum = 0;
	PinCfg.Pinnum = 9;
	PinCfg.Portnum = 0; //P0.0 fun2 is TXD3
	PINSEL_ConfigPin(&PinCfg);
	GPIO_SetDir(0, 1<<9, 1);
	GPIO_SetValue(0, 1<<9);
//	PinCfg.Pinnum = 0; //P0.1 fun2 is RXD3
//	PINSEL_ConfigPin(&PinCfg);
}

void pinsel_uart3(void)
{
	PINSEL_CFG_Type PinCfg;
	PinCfg.Funcnum = 2;
	PinCfg.Pinnum = 0;
	PinCfg.Portnum = 0; //P0.0 fun2 is TXD3
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 1; //P0.1 fun2 is RXD3
	PINSEL_ConfigPin(&PinCfg);
}

void init_uart(void)
{
	UART_CFG_Type uartCfg;
	uartCfg.Baud_rate = 9600;  			 	//baud rate 9600
	uartCfg.Databits = UART_DATABIT_8;		//8bits signal
	uartCfg.Parity = UART_PARITY_NONE;		//none parity bit
	uartCfg.Stopbits = UART_STOPBIT_1;		//1 stop bit
	pinsel_uart3(); 						//pin select for uart3
	UART_Init(LPC_UART3, &uartCfg);			//supply power & setup working par.s for uart3
	UART_TxCmd(LPC_UART3,ENABLE);			//enable transmit for uart3
	LPC_UART3->FCR = (UART_FCR_FIFO_EN|UART_FCR_RX_RS|UART_FCR_TX_RS|UART_FCR_TRG_LEV0);//enable FIFO
	UART_IntConfig(LPC_UART3, UART_INTCFG_RBR, ENABLE); //enable RBR
}

// Function to initialise GPIO to access LED2

void UART3_IRQHandler(void)
{
	//printf("interrupted\n");
	uint8_t roo = 0;
	UART_Receive(LPC_UART3,&roo,1, BLOCKING);
	printf("%c", roo);
	printf("\n");
}


// ****************
int main(void) {
	volatile static int i = 0 ;
	int adval, adval_64;
	// Turn on power to ADC block
		LPC_SC->PCONP |=  ADC_POWERON;

		// Turn on ADC peripheral clock
		LPC_SC->PCLKSEL0 &= ~(PCLK_ADC_MASK);
		LPC_SC->PCLKSEL0 |=  (3 << PCLK_ADC);

		// Set P0.23 to AD0.0 in PINSEL1
		LPC_PINCON->PINSEL1	|= SELECT_ADC0;

		// *****************************************
			// * Set up SysTick - used to provide delays
			// *****************************************

			// Setup SysTick Timer to interrupt at 1 msec intervals
			if (SysTick_Config(SystemCoreClock / 1000)) {
			    while (1);  // Capture error
			}


			// ****************************************************
			// * Enter main loop - reading ADC pot and updating LCD
			// ****************************************************
			while(1) {

				// Start A/D conversion for on AD0.0
				LPC_ADC->ADCR = START_ADC | OPERATIONAL_ADC | SEL_AD0 ;

				do {
					adval = LPC_ADC->ADGDR;            // Read A/D Data Register
				} while ((adval & ADC_DONE_BIT) == 0); // Wait for end of A/D Conversion

				// Stop A/D Conversion
				LPC_ADC->ADCR &= ~(START_ADC | OPERATIONAL_ADC | SEL_AD0) ;

				// Extract AD0.0 value - 12 bit result in bits [15:4]
				adval = (adval >> 4) & 0x0FFF ;
				//printf("\nprint start: ");
				printf("%d\n", adval);
				// Scale ADC value from range of 0-4095 down to 0-63
				// for use in drawing meter on LCD
				//adval_64 = adval >> 6;

			    //systick_delay (250); // wait a 1/4 of a second (250ms)

			    i++ ;	// increment loop counter

			}	//	end of while(1) infinite loop

	return 0 ;
}
