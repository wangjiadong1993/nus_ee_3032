// CMSIS headers required for setting up SysTick Timer
#include "LPC17xx.h"

// Lib_MCU header files
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_uart.h"
#include "string.h"
#include "stdio.h"


void pinsel_uart0(void);

void init_uart0(void);

void gsm_send(char* str);

void gsm_send_with_end(char* str);

void init_gsm(void);

int gsm_get_response();

int gsm_init_http(int (*gsm_get_response)(void));

int gsm_send_request(double la, double lon, int (*gsm_get_response)(void));

void gsm_set_baud();
