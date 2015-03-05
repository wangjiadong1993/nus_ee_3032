// CMSIS headers required for setting up SysTick Timer
#include "LPC17xx.h"

// Lib_MCU header files
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_uart.h"
#include "Load_3032.h"
#include "SD_3032.h"
#include "string.h"


#define LOAD_FILE "/load.in"
#define LOCATE_FILE "/location.txt"
#define GPS_TIMER_LIMIT 180000
int load_write(double front, double middle, double rear, int date_l, int time_l)
{
	FL_FILE *load_write = fl_fopen(LOAD_FILE, "a");
	char data[200] ="1";
	sprintf(data, "%d %d %f %f %f\n", date_l, time_l, front, middle, rear);
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
	FL_FILE *load_read = fl_fopen(LOCATE_FILE, "r");
	char temp = NULL;
	if(load_read != NULL)
	{
		temp = fl_fgetc(load_read);
		while(temp != EOF && temp != -1 && temp != 256)
		{
			printf("%c", temp);
			temp=fl_fgetc(load_read);
		}
		//printf("done\n");
	}
}

void location_write(double latitude, double longitude, double velocity, int date, int time)
{
	FL_FILE *locate_write = fl_fopen(LOCATE_FILE, "a");
	char data[200] ="1";
	sprintf(data, "%f %f %f %d %d\n", latitude, longitude, velocity, date, time);
	if(locate_write != NULL)
	{
		fl_fputs(data, locate_write);
		fl_fclose(locate_write);
		return 1;
	}
	else{
		return 0;
	}
}

void location_read()
{

}
