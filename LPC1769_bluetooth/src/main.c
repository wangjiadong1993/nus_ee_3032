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

// ****************
int main(void) {
	

	initial_gpio();

//	}
	init_uart();
	char* moo= "U, 9600, N";
	uint8_t roo;
	uint32_t r= 0;
	moo="AT+PIN0011";
	uint32_t test = 0;
	UART_Send(LPC_UART3, (uint8_t *)moo , strlen(moo), BLOCKING);




	for(r = 0 ; r <= 8; r++)
	{
		UART_Receive(LPC_UART3,&roo,1, BLOCKING);
		printf("%c", roo);
	}



	//printf("%d", test);
	printf("dhaudhajs\n");
	if(moo[0] != 'A' || 1)
	{
		GPIO_ClearValue(0, 1<<9);
	}
	return 0 ;
}
