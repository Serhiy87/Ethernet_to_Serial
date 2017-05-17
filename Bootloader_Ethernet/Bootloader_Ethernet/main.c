/*
 * Bootloader_Ethernet.c
 *
 * Created: 15.05.2017 10:23:41
 * Author : Admin
 */ 

#include <avr/io.h>

#define Timer8ApplManuNumber	040
#define Timer16ApplManuNumber	060
#define Timer32ApplManuNumber	020
#define Timer8ApplNumber	(Timer8ApplManuNumber  + 040)
#define Timer16ApplNumber	(Timer16ApplManuNumber + 030)
#define Timer32ApplNumber	(Timer32ApplManuNumber + 020)
#include "time.h"
#include "W5100_Boot.h"

int main(void)
{
	W5100_init();	
    /* Replace with your application code */
    while (1) 
    {
    }
}

