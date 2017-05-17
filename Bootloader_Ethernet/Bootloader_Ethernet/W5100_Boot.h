/*
 * W5100_Boot.h
 *
 * Created: 15.05.2017 10:32:51
 *  Author: Admin
 */ 


#ifndef W5100_BOOT_H_
#define W5100_BOOT_H_

 #define F_CPU 16000000UL
 #define SPI_PORT PORTB
 #define SPI_DDR DDRB
 #define SCK_PIN	 1
 #define SS_PIN   0
 #define MOSI_PIN 2
 #define MISO_PIN 3

 
 #define MAX_SOCK_NUM 4
 #define TXBUF_BASE 0x4000
 #define RXBUF_BASE 0x6000
 static const uint16_t CH_BASE = 0x0400;
 static const uint16_t CH_SIZE = 0x0100;

 //Socket states
 #define CLOSED       0x00
 #define INIT         0x13
 #define LISTEN       0x14
 #define SYNSENT      0x15
 #define SYNRECV      0x16
 #define ESTABLISHED  0x17
 #define FIN_WAIT     0x18
 #define CLOSING      0x1A
 #define TIME_WAIT    0x1B
 #define CLOSE_WAIT   0x1C
 #define LAST_ACK     0x1D
 #define UDP          0x22
 #define IPRAW        0x32
 #define MACRAW       0x42
 #define PPPOE        0x5F

 //Socket commands
 #define   Sock_OPEN       0x01
 #define   Sock_LISTEN     0x02
 #define   Sock_CONNECT    0x04
 #define   Sock_DISCON     0x08
 #define   Sock_CLOSE      0x10
 #define   Sock_SEND       0x20
 #define   Sock_SEND_MAC   0x21
 #define   Sock_SEND_KEEP  0x22
 #define   Sock_RECV       0x40

 static const uint8_t SEND_OK = 0x10;
 static const uint8_t TIMEOUT = 0x08;
 static const uint8_t RECV    = 0x04;
 static const uint8_t DISCON  = 0x02;
 static const uint8_t CON     = 0x01;

 #define  RST  7 // Reset BIT
 #define SOCKETS  4
 static const uint16_t SMASK = 0x07FF; // Tx buffer MASK
 static const uint16_t RMASK = 0x07FF; // Rx buffer MASK
 static const uint16_t SSIZE = 2048; // Max Tx buffer size
 static const uint16_t RSIZE = 2048; // Max Rx buffer size
 uint16_t SBASE[SOCKETS]; // Tx buffer base address
 uint16_t RBASE[SOCKETS]; // Rx buffer base address
  uint8_t W5100write(uint16_t _addr, uint8_t _data);
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

   uint8_t W5100_SetGateway(uint16_t oct1, uint16_t oct2, uint16_t oct3, uint16_t oct4)
   {
	   W5100write(0x01, oct1);
	   W5100write(0x02, oct2);
	   W5100write(0x03, oct3);
	   W5100write(0x04, oct4);
	   return 1;
   }
   uint8_t W5100_SetMask(uint16_t oct1, uint16_t oct2, uint16_t oct3, uint16_t oct4)
   {
	   W5100write(0x05, oct1);
	   W5100write(0x06, oct2);
	   W5100write(0x07, oct3);
	   W5100write(0x08, oct4);
	   return 1;
   }

 void W5100_init(){

	DDRB|=1<<PB5;
	PORTB|=1<<PB5;

	PORTB&=~(1<<PB5);
	_delay_ms(200);
	DDRE|=1<<PE4;
	PORTE&=~(1<<PE4);
	_delay_ms(200);
	DDRE&=~(1<<PE4);
		//PORTE|=(1<<PE4) ;
	_delay_ms(200);

	  for (int i=0; i<MAX_SOCK_NUM; i++) {
		  SBASE[i] = TXBUF_BASE + SSIZE * i;
		  RBASE[i] = RXBUF_BASE + RSIZE * i;
	  }
	W5100write(0, 0x80);
	W5100write(0x1B, 0x55);
	W5100write(0x1A, 0x55);
	W5100_SetMac(0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02);
	W5100_SetIP(10,0,0,76);
	W5100_SetGateway(10, 0, 0, 254);
	W5100_SetMask(255, 255, 255, 0);
	
}
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
	 if(sym!=3){
		 
		 SPI_PORT|=(1<<SS_PIN);
		 W5100_init();
	 }
	 SPI_PORT|=(1<<SS_PIN);


	 return 1;
 }

 uint8_t W5100read(uint16_t _addr)
 {

	 // setSS();
	 SPI_PORT&=~(1<<SS_PIN);
	 SPItransfer(0x0F);
	 SPItransfer(_addr >> 8);
	 SPItransfer(_addr & 0xFF);
	 uint8_t _data = SPItransfer(0);
	 //resetSS();
	 SPI_PORT|=(1<<SS_PIN);

	 return _data;
 }


#endif /* W5100_BOOT_H_ */