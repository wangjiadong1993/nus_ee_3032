// CMSIS headers required for setting up SysTick Timer
#include "LPC17xx.h"

// Lib_MCU header files
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_uart.h"
#include "string.h"
#include "stdio.h"

#define BAUD_RATE 9600
////private function
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
	uartCfg.Baud_rate = BAUD_RATE;  			 	//baud rate 115200
	uartCfg.Databits = UART_DATABIT_8;		//8bits signal
	uartCfg.Parity = UART_PARITY_NONE;		//none parity bit
	uartCfg.Stopbits = UART_STOPBIT_1;		//1 stop bit
	pinsel_uart3(); 						//pin select for uart3
	UART_Init(LPC_UART3, &uartCfg);			//supply power & setup working par.s for uart3
	UART_TxCmd(LPC_UART3,ENABLE);			//enable transmit for uart3
	LPC_UART3->FCR = (UART_FCR_FIFO_EN|UART_FCR_RX_RS|UART_FCR_TX_RS|UART_FCR_TRG_LEV0);//enable FIFO
	UART_IntConfig(LPC_UART3, UART_INTCFG_RBR, ENABLE); //enable RBR
}



/*public functions*/
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
	//printf("sent\n");
}


void init_bt(void)
{
	init_uart();
	NVIC_EnableIRQ(UART3_IRQn);

}

