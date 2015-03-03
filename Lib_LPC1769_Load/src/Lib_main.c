//#include "Load_3032.h"
//
////timer
//volatile uint32_t msTicks; // counter for 1ms SysTicks
//void SysTick_Handler(void) {
//  msTicks++;
//}
//
//// ****************
//// systick_delay - creates a delay of the appropriate number of Systicks (happens every 1 ms)
//__INLINE static void systick_delay (uint32_t delayTicks) {
//  uint32_t currentTicks;
//
//  currentTicks = msTicks;	// read current tick counter
//  // Now loop until required number of ticks passes
//  while ((msTicks - currentTicks) < delayTicks);
//}
//int main(void) {
//	int adval;
//	init_load();
//	if (SysTick_Config(SystemCoreClock / 1000)) {
//		while (1);  // Capture error
//	}
//	while(1) {
//
//		adval=read_load();
//		printf("%.2f\n",adval/100.0- 25.4 );//(adval/10.0-250)/3.5);
//		systick_delay(500);
//		}
//
//	return 0 ;
//}
