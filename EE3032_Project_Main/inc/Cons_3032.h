// CMSIS headers required for setting up SysTick Timer
#include "LPC17xx.h"

// Lib_MCU header files
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_uart.h"
#include "Load_3032.h"
#include "SD_3032.h"
#include "string.h"


#define LOAD_FILE "load.in"

int load_write(double front, double middle, double rear, long int date_l, long int time_l)
{
	FL_FILE *load_write = fl_fopen("/load.in", "a");
	char data[200] ="1";
	sprintf(data, "%ld %ld %f %f %f\n", date_l, time_l, front, middle, rear);
	if(load_write != NULL)
	{
		fl_fputs(data, load_write);
		fl_fclose(load_write);
		return 1;
		//printf("done\n");
	}
	else{
		return 0;
		//printf("write laod failed.\n");
	}
}

void load_read()
{
	FL_FILE *load_read = fl_fopen("/load.in", "r");
	char data[200] ="1";
	//sprintf(data, "%ld %ld %f %f %f\n", date_l, time_l, front, middle, rear);
	if(load_write != NULL)
	{
		fl_fputs(data, load_write);
		fl_fclose(load_write);
		return 1;
		//printf("done\n");
	}
	else{
		return 0;
		//printf("write laod failed.\n");
	}
}

void location_write(double latiude, double longitude, double velocity, double date, double time)
{

}

void location_read()
{

}
