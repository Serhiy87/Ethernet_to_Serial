/*
 * Ethernet_to_Serial.c
 *
 * Created: 03.05.2017 10:53:53
 * Author : Admin
 */ 
#define LOGGING
 #ifdef LOGGING
	 //#define Ethernet_Automat_BEGIN_LOGGING
	 //#define send_DHCP_MESSAGE_Automat_LOGGING
	 //#define DHCP_Automat_LOGGING
	 //#define UDP_DEBUG
	  //#define SNMP_Automat_LOGGING
	  #define Webserver_Automat_LOGGING
#endif
 // #define PROGMEM _attribute_((progmem))
 #define F_CPU 16000000UL
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "W5100.h"





#define Timer8ApplManuNumber	040
#define Timer16ApplManuNumber	060
#define Timer32ApplManuNumber	020
#define Timer8ApplNumber	(Timer8ApplManuNumber  + 040)
#define Timer16ApplNumber	(Timer16ApplManuNumber + 030)
#define Timer32ApplNumber	(Timer32ApplManuNumber + 020)
#include "time.h"
#include "Serial.h"
#include "udp_automat.h"

uint8_t LED_Timer;
uint8_t myMillisTimer;
uint32_t myMillis(void){
	return (1000000 - GetTimer32(myMillisTimer))*10;
};
#include "TEST_Automat.h"
#include "Ethernet.h"

void LED_Init(){
 LED_Timer = Timer16Alloc();
}
void LED_On(){
PORTG|=1<<PG3;
}
void LED_Off(){
	PORTG&=~(1<<PG3);
}

void LED_Cycle(){
static uint8_t state = 0;
	if(Timer16Stopp(LED_Timer)){
		StartTimer16(LED_Timer,100);
		if(state == 0){
				PORTB|=1<<PB7;
				//PORTG&=~(1<<PG3);
				state = 1;
		}
		else{
				PORTB&=~(1<<PB7);
				//PORTG|=1<<PG3;
				state = 0;
		}
	}
};

void sysInit(){
	//Инициализация портов светодиодов
	DDRB|=1<<PB7;
	DDRG|=1<<PG3;
	//Инициализация SPI
	SPCR |=(1<<SPE)|(1<<MSTR);
	SPSR = (0<<SPI2X);
	SPI_DDR|=1<<SCK_PIN;
	SPI_DDR|=1<<MOSI_PIN;
	SPI_DDR&=~(1<<MISO_PIN);
	SPI_DDR|=1<<SS_PIN;
	SPI_PORT|=1<<SS_PIN;
TimeInit();
myMillisTimer = Timer32Alloc();
StartTimer32(myMillisTimer,1000000);
TimersInc();
SerialInit();
}
void sysCycle(){
TimersInc();
TimeCycle();
}


int main(void)
{
	sysInit();
	TimeCycle();
    W5100_init();
		TimeCycle();
	Ethernet_Automat(0);
    while (1) 
    {
	_delay_ms(100);
		sysCycle();
		LED_Cycle();
		Ethernet_Automat(1);
		SerialCycle();
	// W5100write(0x1A, 0x55);

    }
}

