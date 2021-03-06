#include "GPS_3032.h"

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
void GPS_switch_init(void)
{

	PINSEL_CFG_Type PinCfg;
	PinCfg.Funcnum = 0;
	PinCfg.Pinnum = 10;
	PinCfg.Portnum = 2; //P2.10 fun0 is GPIO
	PINSEL_ConfigPin(&PinCfg);
	GPIO_SetDir(2, 1<<10, 1);
	GPIO_SetValue(2, 1<<10);//trun it on
}

void init_uart2(void)
{
	UART_CFG_Type uartCfg;
	uartCfg.Baud_rate = 9600;  			 	//baud rate 9600
	uartCfg.Databits = UART_DATABIT_8;		//8bits signal
	uartCfg.Parity = UART_PARITY_NONE;		//none parity bit
	uartCfg.Stopbits = UART_STOPBIT_1;		//1 stop bit
	pinsel_uart2(); 						//pin select for uart3
	GPS_switch_init();
	UART_Init(LPC_UART2, &uartCfg);			//supply power & setup working par.s for uart3
	UART_TxCmd(LPC_UART2,ENABLE);			//enable transmit for uart3
	LPC_UART2->FCR = (UART_FCR_FIFO_EN|UART_FCR_RX_RS|UART_FCR_TX_RS|UART_FCR_TRG_LEV0);//enable FIFO
	UART_IntConfig(LPC_UART2, UART_INTCFG_RBR, ENABLE); //enable RBR
}

void GPS_on()
{
	GPIO_SetValue(2, 1<<10);

}
void GPS_off()
{
	GPIO_ClearValue(2, 1<<10);
}
