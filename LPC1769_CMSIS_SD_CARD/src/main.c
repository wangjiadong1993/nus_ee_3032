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

/*----------------------------------------------------------------------------
  Test R/W of single sector
 *----------------------------------------------------------------------------*/
void SingleSector_RW_Test()
{
    uint32_t i;

    printf("\n>Single sector read/write test ...\n\n");

    printf("Read sector #%d:\n", S_SECTOR_INDEX);
    if (SD_ReadSector(S_SECTOR_INDEX, buf, 1) == SD_FALSE)
    {
        printf("Failed to read sector %d.\n",  S_SECTOR_INDEX);
        while (1);
    }
    printf("(Only display the first %d bytes to avoid too many content in terminal).\n", DISPLAY_SIZE);
    for (i=0;i<DISPLAY_SIZE;)
    {
        printf("%2x ", buf[i++]);
        if ((i%16)==0) printf("\n");
    }

    printf("Fill sector #%d with 0x%x.\n", S_SECTOR_INDEX, S_FILL_VALUE);
    for (i=0;i<512;i++) buf[i] = S_FILL_VALUE;
    if (SD_WriteSector(S_SECTOR_INDEX, buf, 1) == SD_FALSE)
    {
        printf("Failed to write sector %d.\n", S_SECTOR_INDEX);
        while (1);
    }

    printf("Read sector #%d\n", S_SECTOR_INDEX);
    if (SD_ReadSector(S_SECTOR_INDEX, buf, 1) == SD_FALSE)
    {
        printf("Failed to read sector %d.\n",  S_SECTOR_INDEX);
        while (1);
    }
    printf("(Only display the first %d bytes to avoid too many content in terminal).\n", DISPLAY_SIZE);
    for (i=0;i<DISPLAY_SIZE;)
    {
        printf("%2x ", buf[i++]);
        if ((i%16)==0) printf("\n");
    }
}

/*----------------------------------------------------------------------------
  Test R/W of multiple sectors
 *----------------------------------------------------------------------------*/
void MultiSector_RW_Test ()
{
    uint32_t i;

    printf("\n>Multiple sector read/write test ...\n\n");

    printf("Read %d sectors from #%d:\n", M_SECTOR_NUM, M_SECTOR_INDEX);
    if (SD_ReadSector(M_SECTOR_INDEX, buf, M_SECTOR_NUM) == SD_FALSE)
    {
        printf("Failed to read sectors from %d.\n",  M_SECTOR_INDEX);
        while (1);
    }
    printf("(Only display the first %d bytes to avoid too many content in terminal).\n", DISPLAY_SIZE);
    for (i=0;i<DISPLAY_SIZE;)
    {
        printf("%2x ", buf[i++]);
        if ((i%16)==0) printf("\n");
    }

    printf("Fill %d sectors from #%d with 0x%x.\n", M_SECTOR_NUM, M_SECTOR_INDEX, M_FILL_VALUE);
    for (i=0;i<512*M_SECTOR_NUM;i++) buf[i] = M_FILL_VALUE;
    if (SD_WriteSector(M_SECTOR_INDEX, buf, M_SECTOR_NUM) == SD_FALSE)
    {
        printf("Failed to write sectors from %d.\n", M_SECTOR_INDEX);
        while (1);
    }

    printf("Read %d sectors from #%d:\n", M_SECTOR_NUM, M_SECTOR_INDEX);
    if (SD_ReadSector(M_SECTOR_INDEX, buf, M_SECTOR_NUM) == SD_FALSE)
    {
        printf("Failed to read sectors from %d.\n",  M_SECTOR_INDEX);
        while (1);
    }
    printf("Only display the first %d bytes to avoid too many content in terminal.\n", DISPLAY_SIZE);
    for (i=0;i<DISPLAY_SIZE;)
    {
        printf("%2x ", buf[i++]);
        if ((i%16)==0) printf("\n");
    }
}

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int media_read(unsigned long sect, unsigned char *buf, unsigned long cnt)
{
	return SD_ReadSector((uint32_t)sect, (uint8_t *)buf, (uint32_t)cnt) ? 1 : 0;
};

int media_write(unsigned long sect, unsigned char *buf, unsigned long cnt)
{
	return SD_WriteSector((uint32_t)sect, (uint8_t *)buf, (uint32_t)cnt) ? 1 : 0;
};

int main (void)
{
    uint32_t i;

    SysTick_Config(SystemCoreClock/1000 - 1);  /* Generate interrupt each 1 ms      */

    //SER_init(1);                          /* UART#1 Initialization              */

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
    printf("Card type: ");
    switch (CardType)
    {
        case CARDTYPE_MMC:
            printf("MMC\n");
            break;
        case CARDTYPE_SDV1:
            printf("Version 1.x Standard Capacity SD card.\n");
            break;
        case CARDTYPE_SDV2_SC:
            printf("Version 2.0 or later Standard Capacity SD card.\n");
            break;
        case CARDTYPE_SDV2_HC:
            printf("Version 2.0 or later High/eXtended Capacity SD card.\n");
            break;
        default:
            break;
    }
    printf("Sector size: %d bytes\n", CardConfig.sectorsize);
    printf("Sector count: %d\n", CardConfig.sectorcnt);
    printf("Block size: %d sectors\n", CardConfig.blocksize);
    printf("Card capacity: %d MByte\n\n", (((CardConfig.sectorcnt >> 10) * CardConfig.sectorsize)) >> 10);
    printf("OCR(hex): ");
    for (i=0;i<4;i++) printf("%02x ", CardConfig.ocr[i]);
    printf("\n");
    printf("CID(hex): ");
    for (i=0;i<16;i++) printf("%02x ", CardConfig.cid[i]);
    printf("\n");
    printf("CSD(hex): ");
    for (i=0;i<16;i++) printf("%02x ", CardConfig.csd[i]);
    printf("\n");


    /* Test read/write of single sector */
    SingleSector_RW_Test ();

    /* Test read/write of multiple sectors */
    MultiSector_RW_Test ();


    /* Read speed test */
    printf("\n>Read speed test ...\n");

    i = 16;
    printf("\nReading %d sectors (%d bytes) ...", i, i*512);
    Timer = 0;
    if (SD_ReadSector(100, buf, i) == SD_FALSE)
    {
        printf("Failed.\n");
        while (1);
    }
    printf(" at speed of %d kB/sec.\n", Timer ? ((i*512) / Timer) : 0);

    i = 32;
    printf("Reading %d sectors (%d bytes) ...", i, i*512);
    Timer = 0;
    if (SD_ReadSector(100, buf, i) == SD_FALSE)
    {
        printf("Failed.\n");
        while (1);
    }
    printf(" at speed of %d kB/sec.\n", Timer ? ((i*512) / Timer) : 0);

    /* Write speed test */
    printf("\n>Write speed test ...\n");
    i = 16;
    printf("\nWriting %d sectors (%d bytes) ...", i, i*512);
    Timer = 0;
    if (SD_WriteSector(100, buf, i) == SD_FALSE)
    {
        printf("Failed.\n");
        while (1);
    }
    printf(" at speed of %d kB/sec.\n", Timer ? ((i*512) / Timer) : 0);

    i = 32;
    printf("Writing %d sectors (%d bytes) ...", i, i*512);
    Timer = 0;
    if (SD_WriteSector(100, buf, i) == SD_FALSE)
    {
        printf("Failed.\n");
        while (1);
    }
    printf(" at speed of %d kB/sec.\n", Timer ? ((i*512) / Timer) : 0);

    printf ("\nTest complete successfully.\n\n");

    printf(">self test by jiadong\n");

    SD_ReadSector(0, buf, 1);
    //int j = 0;
//    for(j = 0; j<=31; j++)
//    {
//    	printf("check the %d th line  ", j);
//    	for(i = 0; i<= 15; i++)
//    	{
//    		printf("%x ",buf[16*j + i]);
//    	}
//    	printf("\n");
//    }
    fl_init();
    if (fl_attach_media(media_read, media_write) != FAT_INIT_OK)
        {
            printf("ERROR: Media attach failed\n");
            return 0;
        }

    printf("hello, finished \n");
    fl_listdirectory("/");
//    fl_attach_media();
    //FL_FILE *test;
    FL_FILE *write;
    printf("testing to use write.c in root dir\n");
    //test = fl_fopen("/TEST.TXT", "w");
    write = fl_fopen("/write.txt", "r");
    printf("%d\n", write);
    //printf("%d", test);
    char temp  = fl_fgetc(write);
//    while(temp!='\n')
//    {
//    	printf("%c", temp);
//    	temp=fl_fgetc(write);
//    };

    //fl_fclose(test);
    fl_fclose(write);
    write = fl_fopen("/write.txt", "a");
    char* data = "jiadong\n";
    fl_fputs(data, write);
//    while(*data != '\n')
//    {
//    	fl_fputs(data, write);
//    	data ++;
//    };
//    fl_fputs('\n', )
//    fl_fputs();
    fl_fclose(write);
    fl_shutdown();
    printf("finished shut down\n");



    while (1);
}


