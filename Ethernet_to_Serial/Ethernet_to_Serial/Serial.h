/*
 * Serial.h
 *
 * Created: 10.05.2017 10:27:29
 *  Author: Admin
 */ 


#ifndef SERIAL_H_
#define SERIAL_H_
#define SERIAL_TX_BUFFER_SIZE 400
#include <string.h>
unsigned char _tx_buffer[SERIAL_TX_BUFFER_SIZE];
volatile uint8_t _tx_buffer_head;
volatile uint8_t _tx_buffer_tail;
uint8_t write(uint8_t c);
uint8_t SerialPrintUint8_t(uint8_t n);
uint8_t SerialPrintln(const char *s);
uint8_t SerialPrint(const char str[]);
uint8_t SerialPrintEndl();
uint8_t SerialPrint(const char str[]);
uint8_t TimerSerial;
void SerialInit(){
TimerSerial = Timer16Alloc();
UCSR0B|=(1<<TXEN0);
UBRR0 = 103;
DDRA|=1<<PA2;
PORTA|=(1<<PA2);
}

void SerialCycle(){

}

ISR(USART0_UDRE_vect)
{
	// If interrupts are enabled, there must be more data in the output
	// buffer. Send the next byte
	unsigned char c = _tx_buffer[_tx_buffer_tail];
	_tx_buffer_tail = (_tx_buffer_tail + 1) % SERIAL_TX_BUFFER_SIZE;

	UDR0 = c;
	UCSR0A|=1<<TXC0;
	if (_tx_buffer_head == _tx_buffer_tail) {
		// Buffer empty, so disable interrupts
		UCSR0B&=~(1<<UDRIE0);
	}
}

uint8_t write(uint8_t c)
{

	uint8_t i = (_tx_buffer_head + 1) % SERIAL_TX_BUFFER_SIZE;
	_tx_buffer[_tx_buffer_head] = c;
	_tx_buffer_head = i;
	
	UCSR0B|=1<<UDRIE0;
	
	return 1;
}
uint8_t writeStr(char *str,uint8_t count){
	for(uint8_t a = 0; a<count; a++)
	{
		write(*(str+a));
	}
	return count;
}

uint8_t SerialPrintUint8_t(uint8_t n){
	char buf[8 * sizeof(uint8_t) + 1]; // Assumes 8-bit chars plus zero byte.
	char *str = &buf[sizeof(buf) - 1];

	*str = '\0';

	// prevent crash if called with base == 1
	uint8_t count = 0;

	do {
		uint8_t c = n %10;
		n /= 10;
		count++;
		*--str = c + 48;
	} while(n);

	return writeStr(str,count);
}

uint8_t SerialPrintUint16_t(uint16_t n){
	char buf[8 * sizeof(uint16_t) + 1]; // Assumes 8-bit chars plus zero byte.
	char *str = &buf[sizeof(buf) - 1];

	*str = '\0';

	// prevent crash if called with base == 1
	uint8_t count = 0;

	do {
		uint8_t c = n %10;
		n /= 10;
		count++;
		*--str = c + 48;
	} while(n);

	return writeStr(str,count);
}
uint8_t SerialPrint(const char str[]){
	for(uint8_t i=0;i<strlen(str);i++)
	{
		write(str[i]);
	}

	return strlen(str);
}
uint8_t SerialPrintln(const char str[]){
	for(uint8_t i=0;i<strlen(str);i++)
	{
		write(str[i]);
	}
	write('\r');
	write('\n');
	return strlen(str);
}
uint8_t SerialPrintEndl(){
	write('\r');
	write('\n');
	return 2;
}


#endif /* SERIAL_H_ */