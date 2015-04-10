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

int steps_num = 0;

//emergency count

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

//GSM Timer
int year_gsm =0;
int month_gsm =0;
int date_gsm =0;
int hour_gsm =0;
int minute_gsm =0;
int second_gsm =0;

//global load analyzed result
double avg_load = 0.0;
double max_load = 0.0;
double min_load = 0.0;
double std_load = 0.0;

//detection result
/*
 * away or empty load 0
 * probably standing or sitting 1
 * walking 2
 * running 3
 * undetectable 4
 */
int body_status =0;
double accuracy =0.0;
double weight =0.0;


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
    if(button_pressed ==1)
    {
    	if(emergency_counter >= 6000)
    	{
    		emergency_status = 0;
    		emergency_counter =0;
    	}
    	if(emergency_counter ==4000)
    	{
    		emergency_status = 2;
    		emergency_counter ++;

    	}
    	if(emergency_counter ==2000)
    	{
    		emergency_status = 1;
    		emergency_counter ++;
    		//emergency_counter =0;
    	}
    	else
    	{
    		emergency_counter ++;
    	}
    }
    else
    {
    	emergency_counter = 0;
    	emergency_status = 0;
    }

    //sleep detection
    if(sleep_counter >=3000000)
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
				if(response_last_gsm[0]=='A'||response_last_gsm[0]=='a')
					continue;
				response_num_gsm = 0;
				return 1;

			}
		}
		return 0;
}
//init SD card
void SD_init_3032()
{
	    if (SD_Init () == SD_FALSE)
	    {
	        while (1);
	    }
	    if (SD_ReadConfiguration () == SD_FALSE)
	    {
	        while (1);
	    }

	    //file system initialization
	    fl_init();
	    if (fl_attach_media(media_read, media_write) != FAT_INIT_OK)
	     {
	    	return;
	     }
	     fl_listdirectory("/");
}

void init_timer_3032()
{

}

void Buttons_LEDs_init()
{
	//buttons input
	PINSEL_CFG_Type PinCfg;

	PinCfg.Funcnum = 0;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 2;
    PinCfg.Pinnum = 7;

    PINSEL_ConfigPin (&PinCfg);
    GPIO_SetDir(2, 1<<7, 1);
    GPIO_ClearValue(2, 1<<7);

	//leds output
	PinCfg.Funcnum = 0;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 2;
    PinCfg.Pinnum = 6;

    PINSEL_ConfigPin (&PinCfg);
    GPIO_SetDir(2, 1<<6, 0);
   // GPIO_ClearValue(2, 1<<6);
//emergency buzzer
	PinCfg.Funcnum = 0;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 2;
    PinCfg.Pinnum = 5;

    PINSEL_ConfigPin (&PinCfg);
    GPIO_SetDir(2, 1<<5, 1);
    GPIO_ClearValue(2, 1<<5);
//emergency button
//	PinCfg.Funcnum = 0;
//    PinCfg.OpenDrain = 0;
//    PinCfg.Pinmode = 0;
//    PinCfg.Portnum = 2;
//    PinCfg.Pinnum = 4;
//    PINSEL_ConfigPin (&PinCfg);
//    GPIO_SetDir(2, 1<<4, 0);

//	PinCfg.Funcnum = 0;
//    PinCfg.OpenDrain = 0;
//    PinCfg.Pinmode = 0;
//    PinCfg.Portnum = 2;
//    PinCfg.Pinnum = 3;
//    PINSEL_ConfigPin (&PinCfg);
//    GPIO_SetDir(2, 1<<4, 0);
    //GPIO_ClearValue(2, 1<<4);
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
		printf("%s\n",substr);
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
			   printf("%f %f \n",lati_l,longi_l);
			   if(strstr(str, "A") != NULL && longi_l > 1)//make sure it is active
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
//q
//	}
}

void UART3_IRQHandler(void)
{
	uint8_t roo = 0;
	UART_Receive(LPC_UART3,&roo,1, BLOCKING);
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
			else if(str_bt[0] == 'C')
			{
				BT_CMD = 'C';
			}
		}
		num_bt =0;
	}
}


void UART0_IRQHandler(void)
{
	uint8_t roo = 0;
	UART_Receive((LPC_UART_TypeDef *)LPC_UART0,&roo,1, BLOCKING);
	str_gsm[num_gsm] = roo;
	num_gsm++;
	str_gsm[num_gsm] = '\0';
	if(roo == '\n')
	{
		num_gsm = 0;
		strcpy(response_last_gsm, str_gsm);
		response_num_gsm = 1;
		str_gsm[num_gsm] = '\0';
	}
}

void upload_location(double a, double b)
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
		char str_temp[100]="0";
		sprintf(str_temp, "L%s", str);
		bt_send(str_temp);
	}
	fl_fclose(load_file);
	fl_remove(LOAD_FILE);
}

void load_data()
{
	int i = 1;
	int j = 0;
		double temp =0.0;
		double *load_arr = NULL;

		load_arr = load_array;

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
			}
		avg = avg/94;
		float std = 0;
		for(i=3; i<=96; i++)
			{
				std= std+ (load_arr[i]-avg)*(load_arr[i]-avg);
			}
		std = std/94;
		std = sqrt(std);
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

}



void save_load_array()
{
	FL_FILE *locate_write = fl_fopen(LOAD_FILE, "a");
	char data[200] ="1";
	int i = 0;
	if(locate_write != NULL)
	{
		//for(i = 0; i<=99; i++)
		//{
			sprintf(data, "%.2f\n", (float)weight);
			fl_fputs(data, locate_write);
		//}
		//return 1;
	}
	fl_fclose(locate_write);
	return ;
}


void analyze_data()
{
	if(avg_load <=empty_value + 5*0.4)
	{
		if(std_load < 2)
		{
			body_status = 0;
			//accuracy = sqrt((2- std_load)/2);//it is an approxiamation
			//weight = 0;
			return ;
		}
	}
	if(avg_load > empty_value + 5*0.4 && avg_load <empty_value + 20*0.4)
	{
		if(std_load < 2)
		{
			body_status = 1;
			//accuracy = 0;//it is an approxiamation
			//weight = 0;
			return ;
		}
		else if(std_load > 8)
		{
			body_status = 2;
			accuracy = 0.7;//it is an approxiamation
			weight = (max_load-empty_value)*1.4/0.45;
			return ;
		}
		else
		{
			body_status = 4;
			//accuracy = 0;//it is an approxiamation
			//weight = 0;
			return ;
		}
	}
	if(avg_load >=empty_value)// + 20*0.4)
	{
		if(std_load < 2)
		{
				body_status = 1;
				accuracy = 0.80;//it is an approxiamation
				weight = (avg_load-empty_value)*1.4/0.4;
				return ;
		}
		else if (std_load >8)
		{
				body_status = 1;
				accuracy = 0.9;//it is an approxiamation
				weight = (max_load-empty_value)/0.45;
				return ;
		}
		else
		{
			body_status = 4;
			//accuracy = 0;//it is an approxiamation
			//weight = 0;
			return ;
		}


	}
}


void load_calibration()
{
	int i =0;
	double temp = 0.0;
	int inprogress = 1;
	int criteria = 0;

	while(inprogress)
	{
		for(i=0; i<=99; i++)
		{
			temp = (read_load_1()/20.0)+ (read_load_2()/13.0) + (read_load_3()/15.0);
			load_array[i] = temp;
			systick_delay(20);
		}
		load_data();

		if(std_load <=1)
		{
			if(empty_value<0.1 || (avg_load - empty_value <5))
			{
				criteria =1;
				empty_value = avg_load;
			}
			else
			{
				;
			}
		}
		else
		{
			;
		}

		if(criteria)
			inprogress =0;
	}
	load_array_position = 0;
}

void count_step()
{
	if(empty_value > 0 )
	{
		float limit = empty_value + 35 * 0.4;
		int i =0;
		for(i=0; i<= 98; i++)
		{
			if(load_array[i]<limit && load_array[i] > empty_value  && load_array[i+1] >= limit &&  load_array[i+1]< empty_value + 100*0.4)
				steps_num ++;
		}

	}
}


void sleep_load_detect()
{
	double temp = (read_load_1()/20.0)+ (read_load_2()/13.0) + (read_load_3()/15.0);
	load_array[load_array_position] =temp;
	load_array_position ++;
	if(load_array_position ==99)
	{
		load_array_position =0;
		count_step();
		load_data();
		analyze_data();
		if(!body_status)
		{
			SLEEP_STATUS = !SLEEP_STATUS;
		}
	}
}
void active_load_detect()
{
	double temp = (read_load_1()/20.0)+ (read_load_2()/13.0) + (read_load_3()/15.0);
	load_array[load_array_position] =temp;
	load_array_position++;
	if(load_array_position ==99)
	{
		load_array_position =0;
		count_step();
//		save_load_array();
		load_data();

		analyze_data();

		//printf("%f %f %f %f %d\n", avg_load, std_load, max_load, weight, steps_num);
		if(body_status ==1 || body_status == 4)
		{

		}
		else
		{

		}

		if(body_status)
		{
			sleep_counter =0;
		}
		save_load_array();
	}
}
void gsm_send_sms()
{
	//gsm_send("AT+CMGS=\"+6598595765\"");
	gsm_send("ATD+6598595765;");
	systick_delay(100);

	//gsm_send_with_end("Emergency from the elderly, please check\n");
}
void test_load()
{
	int a = 0;
	load_calibration();
	analyze_data();

	while(1)
	{

		int i = 0;
		for(i=0;i<=99;i++)
		{
			active_load_detect();
			systick_delay(10);
		}
	}
}
int main()
{
	int temp_1, temp_2, temp_3, temp_4;
	//char *temp_s;
	SysTick_Config(SystemCoreClock/1000 - 1);  /* Generate interrupt each 1 ms     */
	initialization_3032();

	//interrupts configuration
	NVIC_EnableIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART3_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);
	systick_delay(1000);
	load_calibration();
	//GSM Calibration
	//int temp_gsm_cali = 0;
	if(0)
	{
	gsm_send("AT");
	systick_delay(100);
	gsm_set_baud();
	systick_delay(1000);
	gsm_send("AT+COPS?");
	systick_delay(1000);
	gsm_send("AT+CSQ?");
	systick_delay(1000);
	gsm_send("AT+CREG?");
	systick_delay(1000);
	//gsm_send_sms();
	}
	//File system clean up
	clean_up_files();
	//test_load();
	while(1)
	{
		bt_send("S0\n");
		while(!SLEEP_STATUS)
		{
			if(BT_CMD == 'A') //read all the load data
			{

				clean_up_load_file();
				BT_CMD = 0;
			}
			if(BT_CMD == 'B')
			{
				BT_CMD = 0;
				char temp_str[100] = "0";
				sprintf(temp_str, "W%f", (float)weight);
				bt_send(temp_str);
			}
			if(BT_CMD == 'C')
			{
				BT_CMD= 0;
				char temp_str[100] = "0";
				sprintf(temp_str, "T%d", steps_num);
				bt_send(temp_str);
			}
			if(BT_CMD =='D')
			{
				BT_CMD = 0;
				SLEEP_STATUS = !SLEEP_STATUS;
			}


			if(GPIO_ReadValue(2)>>6 & 0x1)
			{
				button_pressed  = 0;
			}
			else
			{
				button_pressed = 1;
			}



			temp_4 = read_temp();
			if(temp_4 >= 3000)
				{
				//turn on heater
				GPIO_SetValue(2, 1<<7);
				bt_send("H1\n");
				}
			else if(temp_4<=2500)
				{
				//turn off heater
				GPIO_ClearValue(2, 1<<7);
				bt_send("H0\n");
				}
			else
				{
					;
				}
			if(1)
				{
					active_load_detect();
				}

			//storage
			if( GPS_timer >= GPS_TIMER_LIMIT)
			{

				if(longitude >=1)
				{
					location_write(latitude, longitude, velocity, time, date);
					upload_location((double)latitude, (double)longitude);
				}
				GPS_timer = 0;
			}

			//if emergency ?
			if(emergency_status ==1)
			{
				//send sms

				//update gps
				//led on for 3 seconds
				GPIO_SetValue(2, 1<<5);
			}
			else if(emergency_status == 2)
			{
				GPIO_ClearValue(2, 1<<5);
				gsm_send_sms();
				bt_send("E1\n");
			}
			else if(emergency_status == 0)
			{
				//GPIO_ClearValue(2, 1<<6);
				GPIO_ClearValue(2, 1<<5);
				bt_send("E0\n");
			}
			else
			{
				emergency_status = 0;
				GPIO_ClearValue(2, 1<<5);

			}
			//sending data
			systick_delay(20);
		}
		//global variables clean up
		load_array_position = 0;
		bt_send("S1\n");
		while(SLEEP_STATUS)
		{
			//bluetooth detection
			if(BT_CMD=='D')
			{
				BT_CMD = 0;
				SLEEP_STATUS = !SLEEP_STATUS;
			}
			//detect load;
			//location
			sleep_load_detect();
			if(emergency_status ==1)
			{
				//send sms

				//update gps
				//led on for 3 seconds
				GPIO_SetValue(2, 1<<5);
			}
			else if(emergency_status == 2)
			{
				GPIO_SetValue(2, 1<<4);
				gsm_send_sms();
			}
			else if(emergency_status == 0)
			{
				GPIO_ClearValue(2, 1<<4);
				GPIO_ClearValue(2, 1<<5);
			}
			else
			{
				emergency_status = 0;
				GPIO_ClearValue(2, 1<<4);
				GPIO_ClearValue(2, 1<<5);
			}
			systick_delay(20);
		}

		//globa; variable clean up
		load_array_position =0;
		//GPIO_ClearValue(2, 1<<6);
		//turn on devices
		bt_send("S0\n");
	}
	return 0 ;
}
