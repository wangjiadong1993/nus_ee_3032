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
	uint8_t roo = 0;
	UART_Receive(LPC_UART3,&roo,1, BLOCKING);
	str[num] = roo;
	num++;
	str[num] = '\0';
}
// ****************
int main(void) {
	

	initial_gpio();
	init_uart();
//	char* moo= "U, 9600, N";
//	uint8_t roo;
//	uint8_t i;
//	char str[100]="0";
//	uint32_t num=0;
//	moo="AT+PIN0011";
	NVIC_EnableIRQ(UART3_IRQn);
	if (SysTick_Config(SystemCoreClock / 1000)) {
		while (1);  // Capture error
	}
	while(1)
	{
		;
//		for
//		UART_Send(LPC_UART3, );
//		if(strlen(str) >=100)
//		{
//			str[0]="\0";
//			num=0;
//
//		}
//		systick_delay(200);
//		if(strlen(str) != 0)
//		{
//			printf("%s\n", str);
//			str[0]='\0';
//		}
////		while(LPC_UART3->LSR&0x1)
////		{
////			roo = LPC_UART3->RBR;
////			str[num] = roo;
////			num++;
//			//printf("%c", roo);
//		}
//		//str[num]='\0';
////		printf("%s", str);
//		//num=0;
////		for(i = 0; i<num; i++)
////			printf("%c", str[num]);
//		//num=0;
////		//printf("data finished\n");
	}
}
