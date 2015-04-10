// CMSIS headers required for setting up SysTick Timer
#include "LPC17xx.h"

// Lib_MCU header files
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_uart.h"
#include "Load_3032.h"
#include "SD_3032.h"
#include "string.h"


#define LOAD_FILE "/load.txt"
#define LOCATE_FILE "/location.txt"
#define LOAD_FILE_TEST "/load.txt"
#define GPS_TIMER_LIMIT 6000
//#define LOAD_TIMER_LIMIT
int load_write(int front, int middle, int rear, int temp)
{
	FL_FILE *load_write = fl_fopen(LOAD_FILE, "a");
	char data[200] ="1";
	sprintf(data, "%s %d %d %d %d\n", __TIME__, front, middle, rear, temp);
	if(load_write != NULL)
	{
		fl_fputs(data, load_write);
		fl_fclose(load_write);

		return 1;

	}
	else{

		return 0;

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

			temp=fl_fgetc(load_read);
		}
	}
}

char* load_read_line(FL_FILE *file, char* str)
{
	return fl_fgets(str, 100, file);
}

int location_write(double latitude, double longitude, double velocity, int date, int time)
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
	FL_FILE *locate_write = fl_fopen(LOCATE_FILE, "r");
	char data[200] ="1";

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
void clean_up_files()
{
	fl_remove(LOAD_FILE);
}

