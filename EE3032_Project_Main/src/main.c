#include "Temp_3032.h"
#include "Bluetooth_3032.h"
#include "GPS_3032.h"
#include "Cons_3032.h"
#include "Load_3032.h"
#include "Temp_3032.h"

volatile uint32_t Timer = 0;
/* data buffer */
uint8_t *buf = (uint8_t *)0x2007C000; // 16KB


//GPS buffer, for update and storage;
double latitude = 0.0;
double longitude = 0.0;
double velocity = 0.0;
int date =0;
int  time =0;
//GPS string
char str[200] = "0";
int num = 0;
//system level timer
int time_sys;
int date_sys;

//BT command
int BT_CMD = 0;
int num_bt =0;
char str_bt[200]="0";
//GPS timer, every 1 minutes, it will update once
uint32_t GPS_timer =0;

__INLINE static void systick_delay (uint32_t delayTicks) {
  uint32_t currentTicks;

  currentTicks = Timer;	// read current tick counter
  // Now loop until required number of ticks passes
  while ((Timer - currentTicks) < delayTicks);
}

void SysTick_Handler (void) /* SysTick Interrupt Handler (1ms)   */
{
    static uint32_t ticks;
    static uint32_t tick_sys;
    /* LED blink at frequency 1Hz */
    if (ticks++ >=10)
    {
        disk_timerproc();
        ticks = 0;
    }
    //system time update
//    if (tick_sys++ >=1000)
//    {
//    	if(time_sys%100 <=58)
//    		time_sys++;
//    	else
//    	{
//    		if((time_sys/100)%100<=58)
//    		{
//    			time_sys
//    		}
//    		else
//    		{
//
//    		}
//    	}
//    }

    Timer++;
    GPS_timer++;
}
//init SD card
void SD_init_3032()
{
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
	    	return;
	     }
	     printf("hello, finished \n");
	     fl_listdirectory("/");
}

void init_timer_3032()
{

}

void initialization_3032()
{
	//GPS	//GPS switch
	init_uart2();
	//SD card
	SD_init_3032();
	//load sensor
	init_load();
	//temp sensor	& heater
	init_temp();
	//GSM

	//timer
	init_timer_3032();
	//Bluetooth
	init_bt();
}

//GPS interrupt
void UART2_IRQHandler(void)
{
	uint8_t roo = 100;
	char* substr =NULL;
	float time_l = 0.0, date_l = 0.0;
	float longi_l=0.0, lati_l=0.0,  velo_l= 0.0;
	int i =0;
	int j=0;
	UART_Receive(LPC_UART2,&roo,1, BLOCKING);
	str[num]=roo;
	num++;
	if(roo=='\n')
	{
		str[num]='\0';
		substr = strstr(str, "$GPRMC,");
		//printf("%s\n",substr);
		if(substr != NULL)
		{
		   for(i=0; i<=strlen(substr); i++)
		   {
			   if(substr[i]==',')
				   j++;
		   }
		   if(j==12)
		   {
			   substr = strstr(substr, ",")+1;
			   sscanf(substr, "%f",&time_l);
			   substr = strstr(substr, ",")+1;
			   substr = strstr(substr, ",")+1;
			   sscanf(substr, "%f",&lati_l);
			   substr = strstr(substr, ",")+1;
			   substr = strstr(substr, ",")+1;
			   sscanf(substr, "%f",&longi_l);
			   substr = strstr(substr, ",")+1;
			   substr = strstr(substr, ",")+1;
			   sscanf(substr, "%f",&velo_l);
			   substr = strstr(substr, ",")+1;
			   substr = strstr(substr, ",")+1;
			   sscanf(substr, "%f",&date_l);
			   //printf("\n date: %f time: %f latitude: %f longitude: %f velocity: %f", date_l, time_l, lati_l, longi_l,velo_l);
			   if(strstr(str, "A") != NULL)//make sure it is active
			   {
				   if(strstr(str, "E") != NULL)
			   			longitude = longi_l;
				   else
			   			longitude = longi_l*-1;
				   if(strstr(str, "N") != NULL)
			   			latitude = lati_l;
				   else
			   			latitude = lati_l*-1;
				   velocity = velo_l;
				   time = time_l;
				   date = date_l;
			   }
		   }
		   j=0;

		}
		num=0;
	}
//	if(lati_l >=0.01 || longi_l >= 0.01 || strstr(str, "A") != NULL)
//	{
//
//	}
}

void UART3_IRQHandler(void)
{
	uint8_t roo = 0;
	UART_Receive(LPC_UART3,&roo,1, BLOCKING);
	//printf("this is %c \n", roo);
	str_bt[num_bt] = roo;

	num_bt++;
	str_bt[num_bt] = '\0';
	if(roo == '\n')
	{
		if(num_bt == 2)
		{
			if(str_bt[0] == 'A')
			{
				BT_CMD = 'A';
			}
			else if (str_bt[0] == 'B')
			{
				BT_CMD = 'B';
			}
		}
		num_bt =0;
	}
}

void clean_up_load_file()
{
	char str[100]="0";
	FL_FILE *load_file = fl_fopen(LOAD_FILE_TEST, "r");
	while(load_read_line(load_file, str) != NULL)
	{
		bt_send(str);
	}
	fl_fclose(load_file);
}

int main()
{
	int temp_1, temp_2, temp_3, temp_4;
	char *temp_s;
	SysTick_Config(SystemCoreClock/1000 - 1);  /* Generate interrupt each 1 ms      */
	initialization_3032();

	NVIC_EnableIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART3_IRQn);
	printf("finished initialization, our smart shoe pad is going to work now\n");
	systick_delay(1000);
	printf("finished timer testing\n");

	while(1)
	{
		//check the bt
		if(BT_CMD == 'A') //read all the load data
		{
			//bt_send("we get the read file command\n");
			clean_up_load_file();
			BT_CMD = 0;
		}







		//check for interrupts






		//read data from from sensors
		temp_1 = read_load_1()-1400;
		temp_2 = read_load_2()-800;
		temp_3 = read_load_3()-800;
		printf("%d %d %d %d\n", temp_1, temp_2, temp_3, temp_1+temp_2+temp_3);
		temp_4 = read_temp();
		if(1)
		{
			//sscanf(__DATE__, );
			load_write((int)temp_1, (int)temp_2, (int)temp_3, (int)temp_4);
		}




		//storage
		if(latitude > 100 && GPS_timer >= GPS_TIMER_LIMIT)
		{
			printf("\n%f %f %f %d %d\n", latitude, longitude, velocity, time, date);
			location_write(latitude, longitude, velocity, time, date);
			GPS_timer = 0;
		}
		//printf("round finish\n");
		//sending data
		systick_delay(200);
	}
	return 0 ;
}
