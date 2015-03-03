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
	printf("interrupted\n");
	UART_Receive(LPC_UART3,&roo,1, BLOCKING);
	printf("%c ", roo);
	str[num] = roo;
	num++;
	str[num] = '\0';
}

void bt_send(char* str)
{
	int i = 0;
	uint8_t sn = '0';
	uint8_t rn = '\r';
	for(i=0; i<strlen(str); i++)
	{
		sn = (uint8_t)str[i];
		UART_Send(LPC_UART3, &(sn), 1, BLOCKING);
	}

	UART_Send(LPC_UART3, &rn, 1, BLOCKING);
	printf("sent\n");
}
// ****************
void init_bt(void)
{
	//initial_gpio();
	init_uart();
	NVIC_EnableIRQ(UART3_IRQn);
}

int main(void) {
	

	init_bt();

	if (SysTick_Config(SystemCoreClock / 1000)) {
		while (1);  // Capture error
	}
	while(1)
	{
		char str[100] = "AT+IPR=9600";
		//bt_send(str);
		systick_delay(1000);

	}
}
