// Lib_MCU header files
#ifndef LOAD_3032_H
#define LOAD_3032_h
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_uart.h"
#include "string.h"
#include "stdio.h"
#include "LPC17xx.h"

//defines
#define ADC_POWERON (1 << 12)
#define PCLK_ADC 24
#define PCLK_ADC_MASK (3 << 24)
#define SELECT_ADC1 (0x1<<16)
#define SELECT_ADC2 (0x1<<18)
#define SELECT_ADC3 (0x1<<20)
#define START_ADC (1<<24)
#define OPERATIONAL_ADC (1 << 21)
#define SEL_AD1 (1 <<1)
#define SEL_AD2 (1 <<2)
#define SEL_AD3 (1 <<3)
#define ADC_DONE_BIT	(1 << 31)

//functions declare
void init_load(void);

int read_load_1(void);
int read_load_2(void);
int read_load_3(void);

#endif


