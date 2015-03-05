#include "Temp_3032.h"
#include "Bluetooth_3032.h"
#include "GPS_3032.h"
#include "Cons_3032.h"
#include "Load_3032.h"
#include "SD_3032.h"
#include "Temp_3032.h"

volatile uint32_t Timer = 0;
/* data buffer */
uint8_t *buf = (uint8_t *)0x2007C000; // 16KB

void SysTick_Handler (void) /* SysTick Interrupt Handler (1ms)   */
{
    static uint32_t ticks;
    /* LED blink at frequency 1Hz */
    if (ticks++ >=10)
    {
        disk_timerproc();
        ticks = 0;
    }
    Timer++;
}
//init SD card
void SD_init_3032()
{
		uint32_t i;
	    printf("\nAccess SDC/MMC via SPI on NXP LPC1700. "__DATE__" "__TIME__"\n\n");
	    if (SD_Init () == SD_FALSE)
	    {
	        printf("Failed to init the card, pls check the card.\n");
	        while (1);
	    }
	    if (SD_ReadConfiguration () == SD_FALSE)
	    {
	        printf("Failed to read card CID or CSD.\n");
	        while (1);
	    }

	    printf("Card init OK.\n\n");
	    //file system initialization
	    fl_init();
	    if (fl_attach_media(media_read, media_write) != FAT_INIT_OK)
	     {
	    	printf("ERROR: Media attach failed\n");
	    	return 0;
	     }
	     printf("hello, finished \n");
	     fl_listdirectory("/");
}
//finished time handler
int initialization_3032()
{
	//GPS	//GPS switch
	init_uart2();
	//SD card
	SD_init_3032();
	//load sensor

	//temp sensor	& heater

	//GSM
	//Bluetooth
}



int main()
{
	SysTick_Config(SystemCoreClock/1000 - 1);  /* Generate interrupt each 1 ms      */
	initialization_3032();
	printf("finished initialization\n");
	return 0 ;
}
