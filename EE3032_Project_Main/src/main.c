#include "Temp_3032.h"
#include "Bluetooth_3032.h"
#include "GPS_3032.h"
#include "Cons_3032.h"
#include "Load_3032.h"
#include "SD_3032.h"
#include "Temp_3032.h"

/*time handler*/
volatile uint32_t msTicks; // counter for 1ms SysTicks
void SysTick_Handler(void) {
  msTicks++;
}
__INLINE static void systick_delay (uint32_t delayTicks) {
  uint32_t currentTicks;

  currentTicks = msTicks;	// read current tick counter
  // Now loop until required number of ticks passes
  while ((msTicks - currentTicks) < delayTicks);
}
//finished time handler


int main(void) {
	int adval;
	init_temp();
	init_gpio_2_8();
	if (SysTick_Config(SystemCoreClock / 1000)) {
		while (1);  // Capture error
	}
	while(1) {

		adval=read_temp();
		printf("%d\n", adval);
		if(adval>2300)
		{
			GPIO_SetValue(2, 1<<8);
		}else if(adval<2250)
		{
			GPIO_ClearValue(2, 1<<8);
		}
		systick_delay(200);
		}
	return 0 ;
}
