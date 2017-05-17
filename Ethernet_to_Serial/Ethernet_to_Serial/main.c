/*
 * Ethernet_to_Serial.c
 *
 * Created: 03.05.2017 10:53:53
 * Author : Admin
 */ 
//#define LOGGING
 #ifdef LOGGING
	 //#define Ethernet_Automat_BEGIN_LOGGING
	 //#define send_DHCP_MESSAGE_Automat_LOGGING
	 #define DHCP_Automat_LOGGING
	 //#define UDP_DEBUG
	 //#define SNMP_Automat_LOGGING
	 //#define Webserver_Automat_LOGGING
#endif


#define  MODBUS
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
#ifdef LOGGING
#include "Serial.h"

#endif








#ifdef MODBUS
//#include "usart.h"
//#include "modbus.h"

uint8_t		MB_Coil[4];
uint8_t		MB_Input[4];
uint16_t	MB_HoldReg[58];
uint16_t	MB_InReg[80];

#define Modbus_Map_List {				\
	{									\
		MB_Coil,		32,			\
		MB_Input,		32,			\
		MB_HoldReg,		58,				\
		MB_InReg,		80				\
	},									\
}

#define Modbus_Qt 1
#define MB_Port_List {Port(0) }
#define Init_Modbus_Param { {EvenParity, 1, MBBR19200} }
#define MB_Role_List {Role_Master}

#define Init_Modbus_M_Param {[0]={2, 20, 5, 1}}		// Period; Timeout, Try, TimeOff;
#define Init_MB_NativeAddr {1}

#include "modbus.h"


// ~~~~~~~~~~~~~
void
USART_Init(void)
{
	MB_Init();
}
extern int32_t Tfree2cond_var;
void MB_AppCycle(){
//MB_HoldReg[1] =  Tfree2cond_var;
};
// ~~~~~~~~~~~~~~
void
USART_Cycle(void)
{
	MB_Cycle();
	MB_AppCycle();
}

Modbus_ISR(0)

Modbus_ISR(1)



// MB_Query MYQUERRY[] = { {4, 0, 25, 0}, {16, 1, 4, 0} , {3, 1, 4, 0} };
  MB_Query MYQUERRY[] = { {4, 0, 25, 0}, {16, 1, 4, 0}  };
 MB_Slave MB_Slave_List[] = { {1, MYQUERRY, 2} };
 MB_Master MB_Master_List[] = { {MB_Slave_List, 1} };


#endif
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
#ifdef LOGGING
SerialInit();
#endif
USART_Init();
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
		#ifdef LOGGING
		SerialCycle();
		#endif
		USART_Cycle();
	// W5100write(0x1A, 0x55);

    }
}

