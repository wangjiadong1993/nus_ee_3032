#include "Bluetooth_3032.h"



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




void UART3_IRQHandler(void)
{
	uint8_t roo = 0;
	printf("interrupted\n");
	UART_Receive(LPC_UART3,&roo,1, BLOCKING);
	str[num] = roo;
	num++;
	str[num] = '\0';
}

int main(void) {
	

	init_bt();
//	char* moo= "U, 9600, N";
//	uint8_t roo;
//	uint8_t i;
//	char str[100]="0";
//	uint32_t num=0;
//	moo="AT+PIN0011";

	if (SysTick_Config(SystemCoreClock / 1000)) {
		while (1);  // Capture error
	}
	while(1)
	{
		char str[100] = "1234567890123456789012345678901234567890";
		bt_send(str);
		systick_delay(1000);
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
