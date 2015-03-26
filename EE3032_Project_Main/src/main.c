#include "Temp_3032.h"
#include "Bluetooth_3032.h"
#include "GPS_3032.h"
#include "Cons_3032.h"
#include "Load_3032.h"
#include "Temp_3032.h"
#include "GSM_3032.h"
#include "math.h"
volatile uint32_t Timer = 0;
/* data buffer */
uint8_t *buf = (uint8_t *)0x2007C000; // 16KB

//GPS buffer, for update and storage;
double latitude = 0.0;
double longitude = 0.0;
double velocity = 0.0;
int date =0;
int  time =0;
float empty_value =0;

//GPS string
char str[200] = "0";
int num = 0;

//system level timer
int time_sys;
int date_sys;

//gsm variables
char str_gsm[1024] = "0";
int num_gsm = 0;
char response_last_gsm[1024]="0";
int response_num_gsm = 0;

//BT command
int BT_CMD = 0;
int BUTTON_CMD = 0;

int SLEEP_CMD = 0;
int SLEEP_STATUS = 0;
int sleep_counter = 0;

int emergency_counter = 0; //it should be pressed for three seconds
int button_pressed = 0;
int emergency_status = 0;

double load_array[100] ={0.0};
int load_array_position = 0;

//global load analyzed result
double avg_load = 0.0;
double max_load = 0.0;
double min_load = 0.0;
double std_load = 0.0;

//detection result
int body_status =0;
double accuracy =0.0;
double weight =0.0;
/*
 * away or empty load 0
 * probably standing or sitting 1
 * walking 2
 * running 3
 * undetectable 4
 */

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


    //emergency detection
    if(button_pressed ==1 && emergency_status == 0)
    {
    	if(emergency_counter ==3000)
    	{
    		emergency_status = 1;
    		emergency_counter =0;
    	}
    	else
    	{
    		emergency_counter ++;
    	}
    }
    else
    {
    	emergency_counter = 0;
    }

    //sleep detection
    if(sleep_counter >=30000)
    {
    	SLEEP_STATUS = 1;
    }
    else
    {
    	sleep_counter ++;
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

int get_response_gsm_main(void)
{
		int i = 0;
		for(i = 0 ; i<= 10; i++)
		{
			systick_delay(100);
			if(response_num_gsm != 0 && strlen(response_last_gsm) <= 20 &&strlen(response_last_gsm) >0)
			{
				printf("%s", response_last_gsm);
				if(response_last_gsm[0]=='A'||response_last_gsm[0]=='a')
					continue;
				response_num_gsm = 0;
				return 1;

			}
			//printf("failed for %d\n", i);
		}
		//printf("\nfailed\n");
		return 0;
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

void Buttons_LEDs_init()
{
	//buttons input

	//leds output

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
	init_gsm();
	//buttons and leds
	Buttons_LEDs_init();

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


void UART0_IRQHandler(void)
{
	uint8_t roo = 0;
	//printf("interrupted\n");
	UART_Receive((LPC_UART_TypeDef *)LPC_UART0,&roo,1, BLOCKING);
	//printf("%c", roo);
	str_gsm[num_gsm] = roo;
	num_gsm++;
	str_gsm[num_gsm] = '\0';
	if(roo == '\n')
	{
		printf("%s", str_gsm);
		num_gsm = 0;
		strcpy(response_last_gsm, str_gsm);
		response_num_gsm = 1;
		str_gsm[num_gsm] = '\0';
		//printf("%s");
	}
}

void upload_location(float a, float b)
{
	gsm_init_http(&get_response_gsm_main);
	systick_delay(1000);
	gsm_send_request(a, b, &get_response_gsm_main);
	systick_delay(1000);
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
	//fl_remove(LOAD_FILE);
}

void load_data()
{
	int i = 1;
	int j = 0;
	while(1)
	{
	float temp =0.0;
	float *load_arr = NULL;
	load_arr = load_array;
//	for(i=1; i<=100; i++)
//	{
//		temp = (read_load_1()/12.0)+ (read_load_2()/13.0) + (read_load_3()/15.0);
//		//printf("%f\n", temp);
//		load_arr[i-1] = temp;
//		systick_delay(50);
//	}
	for(i=0; i<=98; i++)
		for(j=i+1;j<=99; j++)
			if(load_arr[i] > load_arr[j])
				{
					temp = load_arr[i];
					load_arr[i]= load_arr[j];
					load_arr[j] = temp;
				}
	float avg = 0;
	for(i=3; i<=96; i++)
		{
			avg += load_arr[i];
			//printf("%f\n", (load_arr[i]-empty_value)/0.43);
		}
	avg = avg/94;
	//printf("the average: %f\n", avg);
	float std = 0;
	for(i=3; i<=96; i++)
		{
			std= std+ (load_arr[i]-avg)*(load_arr[i]-avg);
		}
	std = std/94;
	std = sqrt(std);
	//printf("the sqrt is %f\n", std);
	int start=3, end=96;
	int start_flag =0;
	int end_flag = 0;
	for(i=3; i <=96; i++)
	{
		if(load_arr[i] >= avg - 3* std && start_flag ==0)
		{
			start = i;
			start_flag = 1;
		}
		if(load_arr[i] >= avg + 3* std && end_flag ==0)
		{
			end  = i-1;
			end_flag = 1;
		}
	}
//	if(end < start)
//	{
//		printf("error\n");
//		//return 0;
//	}
	//printf("the start and end will be %d  %d", start, end);
	avg = 0;
	for(i=start; i<=end; i++)
	{
		avg += load_arr[i];
	}
	avg = avg /(end-start + 1);

	//set all global variables
	avg_load = avg;
	std_load = std;
	min_load = load_arr[start];
	max_load = load_arr[end];
//	finished setting
//
//	printf("the average is %f\n", avg);
//	return avg;
//	printf("type in a number and carry on\n");
//	if(empty_value<=0.1 || ((avg-empty_value)/0.43 <=5 && std<=2.5))
//	{
//		empty_value =avg;
//		printf("the init load is reset now.\n");
//	}
//	else
//	{
//		printf("your load is approximately %f\n", (avg-empty_value)/0.43);
//	}
//	scanf("%d", &i);
//	}
}

void save_load_array()
{

}

void analyze_data()
{

}

void load_calibration()
{
	double temp = (read_load_1()/12.0)+ (read_load_2()/13.0) + (read_load_3()/15.0);
	load_array[load_array_position] =temp;
	if(load_array_position ==99)
	{
		load_array_position =0;
		load_data();
	}
}

}
void sleep_load_detect()
{
	double temp = (read_load_1()/12.0)+ (read_load_2()/13.0) + (read_load_3()/15.0);
	load_array[load_array_position] =temp;
	if(load_array_position ==99)
	{
		load_array_position =0;
		load_data();
	}
}
void active_load_detect()
{
	double temp = (read_load_1()/12.0)+ (read_load_2()/13.0) + (read_load_3()/15.0);
	load_array[load_array_position] =temp;
	if(load_array_position ==99)
	{
		load_array_position =0;
		load_data();
	}
}

int main()
{
	int temp_1, temp_2, temp_3, temp_4;
	char *temp_s;
	SysTick_Config(SystemCoreClock/1000 - 1);  /* Generate interrupt each 1 ms      */
	initialization_3032();

	//interrupts configuration
	NVIC_EnableIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART3_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);
	printf("finished initialization, our smart shoe pad is going to work now\n");
	systick_delay(1000);
	printf("finished timer testing\n");
	printf("gsm baud rate set and test on\n");

	//GSM Calibration
	//gsm_set_baud();
	systick_delay(500);
	//gsm_send("AT+COPS?");
	systick_delay(500);
	//upload_location(107,3);

	//File system clean up
	clean_up_files();

	load_calibration();
	while(1)
	{
		while(!SLEEP_STATUS)
		{
			//check the bt
			if(BT_CMD == 'A') //read all the load data
			{
				//bt_send("we get the read file command\n");
				clean_up_load_file();
				BT_CMD = 0;
				printf("finished send data\n");
			}

			//check for interrupts
			if(BUTTON_CMD == 'A')
			{
				//something
			}

			else if(BUTTON_CMD == 'E')
			{

				//emergency
			}

//			activate SLEEP
//					if(SLEEP_CMD =='A')
//					{
//						//activate sleep mode
//						SLEEP_STATUS = 1;
//						SLEEP_CMD
//
//					}
//
//			read data from from sensors
//			temp_1 = read_load_1()-1000;
//			temp_2 = read_load_2()-1000;
//			temp_3 = read_load_3()-1000;
//			printf("%d %d %d %d\n", temp_1, temp_2, temp_3, (temp_1/13+temp_2/12+temp_3/15)*10);
//			float temp_temp = (temp_1/13+temp_2/12+temp_3/15)*10-2120+1.75*11;
//			printf("%.2f\n", temp_temp);
			temp_4 = read_temp();

			if(0)
			{
				//sscanf(__DATE__, );
				//load_write((int)temp_1, (int)temp_2, (int)temp_3, (int)temp_4);
				active_load_detect();
			}

			//storage
			if(0)//(latitude > 100)// && GPS_timer >= GPS_TIMER_LIMIT)
			{
				printf("\n%f %f %f %d %d\n", latitude, longitude, velocity, time, date);
				location_write(latitude, longitude, velocity, time, date);
				upload_location(latitude, longitude);
				GPS_timer = 0;
			}

			//if emergency ?
			if(emergency_status)
			{
				//send sms
				//update gps
				//buz for 3 seconds
				//led on for 3 seconds
				//off
			}
			//sending data
			systick_delay(200);
		}
		//global variables clean up
		load_array_position = 0;
		//turn off devices

		while(SLEEP_STATUS)
		{

			//detect load;
			sleep_load_detect();
			if(emergency_status)
						{
							//send sms
							//update gps
							//buz for 3 seconds
							//led on for 3 seconds
							//off
						}
			sleep(100);
		}

		//globa; variable clean up
		load_array_position =0;
		//turn on devices

	}

	return 0 ;
}
