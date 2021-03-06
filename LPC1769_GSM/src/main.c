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

#define BAUD_RATE_GSM 9600
#define BAUD_RATE_GSM_CHAR "9600"
#define INTERNET_ACCESS_POINT "INTERNET"
#define HTTP_URL "http://128.199.213.135/locations/post_new"

uint32_t msTicks;
//volatile uint32_t msTicks; // counter for 1ms SysTicks
char str[1024] = "0";
int num = 0;
char response_last[1024]="0";
int response_num = 0;
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


void pinsel_uart0(void)
{
	PINSEL_CFG_Type PinCfg;
	PinCfg.Funcnum = 1;
	PinCfg.Pinnum = 2;
	PinCfg.Portnum = 0; //P0.0 fun2 is TXD3
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 3; //P0.1 fun2 is RXD3
	PINSEL_ConfigPin(&PinCfg);
}

void init_uart0(void)
{
	UART_CFG_Type uartCfg;
	uartCfg.Baud_rate = BAUD_RATE_GSM;  			 	//baud rate 9600
	uartCfg.Databits = UART_DATABIT_8;		//8bits signal
	uartCfg.Parity = UART_PARITY_NONE;		//none parity bit
	uartCfg.Stopbits = UART_STOPBIT_1;		//1 stop bit
	pinsel_uart0(); 						//pin select for uart0
	UART_Init((LPC_UART_TypeDef *)LPC_UART0, &uartCfg);			//supply power & setup working par.s for uart0
	UART_TxCmd((LPC_UART_TypeDef *)LPC_UART0,ENABLE);			//enable transmit for uart0
	LPC_UART0->FCR = (UART_FCR_FIFO_EN|UART_FCR_RX_RS|UART_FCR_TX_RS|UART_FCR_TRG_LEV0);//enable FIFO
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_RBR, ENABLE); //enable RBR
}

// Function to initialise GPIO to access LED2

void UART0_IRQHandler(void)
{
	uint8_t roo = 0;
	//printf("interrupted\n");
	UART_Receive((LPC_UART_TypeDef *)LPC_UART0,&roo,1, BLOCKING);
	//printf("%c", roo);
	str[num] = roo;
	num++;
	str[num] = '\0';
	if(roo == '\n')
	{
		num = 0;
		strcpy(response_last, str);
		response_num = 1;
		str[num] = '\0';
	}
}

void gsm_send(char* str)
{
	int i = 0;
	uint8_t sn = '0';
	uint8_t rn = '\r';
	for(i=0; i<strlen(str); i++)
	{
		sn = (uint8_t)str[i];
		UART_Send((LPC_UART_TypeDef *)LPC_UART0, &(sn), 1, BLOCKING);
	}

	UART_Send((LPC_UART_TypeDef *)LPC_UART0, &rn, 1, BLOCKING);
	//printf("sent\n");
}
// ****************
void init_bt(void)
{
	//initial_gpio();
	init_uart0();
	NVIC_EnableIRQ(UART0_IRQn);
}
int gsm_get_response()
{
	int i = 0;
	for(i = 0 ; i<= 10; i++)
	{
		systick_delay(200);
		if(response_num != 0 && strlen(response_last) <= 20 &&strlen(response_last) >0)
		{
			//printf("print this success %s", str );
			printf("%s", response_last);
			if(response_last[0]=='A'||response_last[0]=='a')
				continue;
			response_num = 0;
			return 1;
		}

	}
	printf("\nfailed\n");
	return 0;
}
int gsm_init_http()
{
	char temp[100]="0";
	gsm_send("AT+QIFGCNT=0\n");
	gsm_get_response();
	sprintf(temp, "AT+QICSGP=1, \"%s\"\n", INTERNET_ACCESS_POINT);
	gsm_send(temp);
	gsm_get_response();
	gsm_send("AT+QIREGAPP\n");
	gsm_get_response();
	gsm_send("AT+QIACT\n");
	gsm_get_response();
	return 0;
}
int gsm_send_request(float la, float lon)
{
	char temp[100]="0";
	char temp_1[100]="0";
	sprintf(temp, "%s?latitude=%f&longitude=%f", HTTP_URL, la, lon);
	printf("%s\n", temp);
	sprintf(temp_1, "AT+QHTTPURL=%d,10\n", strlen(temp));
	gsm_send(temp_1);
	gsm_get_response();
	gsm_send(temp);
	gsm_get_response();
	gsm_send("AT+QHTTPGET=20\n");
	gsm_get_response();
	systick_delay(200);
	printf("sent\n");
	gsm_send("AT+QHTTPREAD=10\n");
	//gsm_get_response();
	gsm_send("AT+QIDEACT\n");
	gsm_get_response();
	return 0;
}

void gsm_set_baud()
{
	char temp[50]  ="0";
	//int time = 10;
	sprintf(temp, "AT+IPR=%d\n", BAUD_RATE_GSM);
	//printf("the string is now %s", temp);
	gsm_send(temp);
	gsm_get_response();
}

int main(void) {
	

	init_bt();

	if (SysTick_Config(SystemCoreClock / 1000)) {
		while (1);  // Capture error
	}

	//gsm_send("AT\n");
	//systick_delay(100);
	//char str_temp[100] = "AT+IPR=9600\n";
	//gsm_send("AT+IPR=9600");
	gsm_set_baud();
	systick_delay(100);
	gsm_init_http();
	systick_delay(100);
	gsm_send_request(12.0, 15.0);
	while(1)
	{

		//gsm_send("AT\n");
		//gsm_get_response();

		//printf("in the loop\n");
		//systick_delay(100);

	}
}
