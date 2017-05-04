/*
 * Ethernet_to_Serial.c
 *
 * Created: 03.05.2017 10:53:53
 * Author : Admin
 */ 
 #define F_CPU 32000000UL
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#define SPI_PORT PORTB
#define SPI_DDR DDRB
#define SCK_PIN	 1
#define SS_PIN   0
#define MOSI_PIN 2	  
#define MISO_PIN 3


uint8_t SPItransfer(uint8_t data) {
	// SPI_Init();
	SPDR = data;
	asm volatile("nop");
	while (!(SPSR & (1<<SPIF))) ; // wait
	
	return SPDR;
}

  uint8_t W5100write(uint16_t _addr, uint8_t _data)
  {
	
	 
	  SPI_PORT&=~(1<<SS_PIN);
	  SPItransfer(0xF0);
	  SPItransfer((uint8_t)(_addr >> 8));
	  SPItransfer((uint8_t)(_addr & 0xFF));
	  uint8_t sym = SPItransfer(_data);
	/*  if(sym!=3){
		  Serial.println("RESET FROM W5100 WRITE");
		  resetEthernet();
	  }*/
	  SPI_PORT|=(1<<SS_PIN);


	  return 1;
  }
  uint8_t W5100_SetMac(uint16_t oct1, uint16_t oct2, uint16_t oct3, uint16_t oct4, uint16_t oct5, uint16_t oct6)
  {
	  W5100write(0x09, oct1);
	  W5100write(0x0A, oct2);
	  W5100write(0x0B, oct3);
	  W5100write(0x0C, oct4);
	  W5100write(0x0D, oct5);
	  W5100write(0x0E, oct6);

	  return 1;
  }

  uint8_t W5100_SetIP(uint16_t oct1, uint16_t oct2, uint16_t oct3, uint16_t oct4)
  {
	  W5100write(0x0F, oct1);
	  W5100write(0x10, oct2);
	  W5100write(0x11, oct3);
	  W5100write(0x12, oct4);
	  return 1;
  }

void init(){
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

		DDRB|=1<<PB5;
		//	_delay_ms(600);

	PORTB&=~(1<<PB5);

	DDRE|=1<<PE4;
	PORTE&=~(1<<PE4);
	_delay_ms(2);
	DDRE&=~(1<<PE4);
			_delay_ms(600);
	 W5100write(0, 0x80);
	 W5100write(0x1B, 0x55);
	 W5100write(0x1A, 0x55);
	 W5100_SetMac(0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02);
	 W5100_SetIP(10,0,0,76);
	 
}




int main(void)
{
    init();
    while (1) 
    {
		PORTB|=1<<PB7;
		PORTG&=~(1<<PG3);
		_delay_ms(500);
		PORTB&=~(1<<PB7);
		PORTG|=1<<PG3;
		_delay_ms(500);
	// W5100write(0x1A, 0x55);

    }
}

