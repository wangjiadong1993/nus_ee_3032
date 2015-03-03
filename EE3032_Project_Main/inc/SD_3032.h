#include <stdio.h>
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "sd.h"
#include "fat_filelib.h"

/* Macro definitions for single/multiple sector read/write */
#define S_SECTOR_INDEX 103
#define S_FILL_VALUE 0xAA

#define M_SECTOR_INDEX  203
#define M_SECTOR_NUM    2
#define M_FILL_VALUE    0x55

/* The number of bytes to display in terminal */
#define DISPLAY_SIZE    32

/* data buffer */
uint8_t *buf = (uint8_t *)0x2007C000; // 16KB

volatile uint32_t Timer = 0;
