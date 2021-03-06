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

// Function to initialise GPIO to access LED2
void led2_init (void)
{
	PINSEL_CFG_Type PinCfg;

	/* Set P0_22 to 00 - GPIO */
	/*9 is the led*/
	PinCfg.Funcnum = 0;
	PinCfg.Pinnum = 9;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	/*8 is the light sensor*/
	PinCfg.Funcnum = 0;
	PinCfg.Pinnum = 8;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);

	// Set GPIO - P0_22 - to be output
	GPIO_SetDir(0,(1<<9),1);
	GPIO_SetDir(0, (1<<8),0);
}

// Function to turn LED2 on
void led2_on (void)
{
	GPIO_SetValue(0,(1<<9));
}

// Function to turn LED2 off
void led2_off (void)
{
	GPIO_ClearValue(0,(1<<9));
}

uint32_t comp ()
{
	uint32_t input = GPIO_ReadValue(0) & 0x80;
	if (input != 0)
	{
		return 1;
	}else{
		return 0;
	}
}

// Function to invert current state of LED2
void led2_invert (void)
{
	int ledstate;

	// Read current state of GPIO P0_0..31, which includes LED2
	ledstate = GPIO_ReadValue(0);

	// Turn off LED2 if it is on
	// (ANDing to ensure we only affect the LED output)
	GPIO_ClearValue(0,(ledstate & (1 << 9)));
	// Turn on LED2 if it is off
	// (ANDing to ensure we only affect the LED output)
	GPIO_SetValue(0,((~ledstate) & (1 << 9)));
}

// ****************
int main(void) {
	
	led2_init();	// Setup GPIO for LED2
	led2_on();		// Turn LED2 on

	// Setup SysTick Timer to interrupt at 1 msec intervals
//	if (SysTick_Config(SystemCoreClock / 1000)) {
//	    while (1);  // Capture error
//	}
	
	// Enter an infinite loop, just incrementing a counter and toggling LEDs every 2 seconds
	//volatile static int i = 0 ;
	while(1) {
		//i++;
	    //systick_delay(2000); // wait 2 seconds (2000ms)
//		printf("hello world\n");
		comp() == 0 ? led2_on() : led2_off();
	    //led2_invert();	// Toggle state of LED2
	}
	return 0 ;
}
