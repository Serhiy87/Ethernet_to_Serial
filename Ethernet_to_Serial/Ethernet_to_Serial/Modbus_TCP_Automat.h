/*
 * Modbus_TCP_Automat.h
 *
 * Created: 17.05.2017 14:57:03
 *  Author: Admin
 */ 


#ifndef MODBUS_TCP_AUTOMAT_H_
#define MODBUS_TCP_AUTOMAT_H_

#include "string.h"
#include "W5100.h"
#include <avr/pgmspace.h>
#include <stdio.h>
#define MAX_BUF 1024
uint8_t buf[MAX_BUF];
char buf1[100];
//String stringOne;

#define TXBUFADDR	0x5800      // W5100 Send Buffer Base Address
#define RXBUFADDR  0x7800      // W5100 Read Buffer Base Address
#define RX_BUF_MASK 0x7FF
#define TX_BUF_MASK 0x7FF
#define WebserverSocket_MR      0x0700      // Socket 0: Mode Register Address
#define WebserverSocket_CR      0x0701      // Socket 0: Command Register Address
#define WebserverSocket_IR      0x0702      // Socket 0: Interrupt Register Address
#define WebserverSocket_SR      0x0703      // Socket 0: Status Register Address
#define WebserverSocket_PORT    0x0704      // Socket 0: Source Port: 0x0404 to 0x0405
#define WebserverSocket_TX_FSR  0x0720      // Socket 0: Tx Free Size Register: 0x0420 to 0x0421
#define WebserverSocket_TX_RD   0x0722      // Socket 0: Tx Read Pointer Register: 0x0422 to 0x0423
#define WebserverSocket_TX_WR   0x0724      // Socket 0: Tx Write Pointer Register: 0x0424 to 0x0425
#define WebserverSocket_RX_RSR  0x0726      // Socket 0: Rx Received Size Pointer Register: 0x0425 to 0x0427
#define WebserverSocket_RX_RD   0x0728      // Socket 0: Rx Read Pointer: 0x0428 to 0x0429
#define WebserverSocketNum      3

// WebserverSocket_SR values
#define SOCK_CLOSED      0x00     // Closed
#define SOCK_INIT        0x13    // Init state
#define SOCK_LISTEN      0x14   // Listen state
#define SOCK_SYNSENT     0x15   // Connection state
#define SOCK_SYNRECV     0x16   // Connection state
#define SOCK_ESTABLISHED 0x17   // Success to connect
#define SOCK_FIN_WAIT    0x18   // Closing state
#define SOCK_CLOSING     0x1A   // Closing state
#define SOCK_TIME_WAIT   0x1B   // Closing state
#define SOCK_CLOSE_WAIT  0x1C   // Closing state
#define SOCK_LAST_ACK    0x1D   // Closing state
#define SOCK_UDP         0x22   // UDP socket
#define SOCK_IPRAW       0x32   // IP raw mode socket
#define SOCK_MACRAW      0x42   // MAC raw mode socket
#define SOCK_PPPOE       0x5F   // PPPOE socket
// WebserverSocket_MR values
#define MR_CLOSE	0x00    // Unused socket
#define MR_TCP      0x01    // TCP
#define MR_UDP      0x02    // UDP
#define MR_IPRAW    0x03    // IP LAYER RAW SOCK
#define MR_MACRAW   0x04    // MAC LAYER RAW SOCK
#define MR_PPPOE    0x05    // PPPoE
#define MR_ND		0x20    // No Delayed Ack(TCP) flag
#define MR_MULTI    0x80    // support multicating

// WebserverSocket_CR values
#define CR_OPEN          0x01    // Initialize or open socket
#define CR_LISTEN        0x02   // Wait connection request in tcp mode(Server mode)
#define CR_CONNECT       0x04   // Send connection request in tcp mode(Client mode)
#define CR_DISCON        0x08   // Send closing reqeuset in tcp mode
#define CR_CLOSE         0x10   // Close socket
#define CR_SEND          0x20   // Update Tx memory pointer and send data
#define CR_SEND_MAC      0x21   // Send data with MAC address, so without ARP process
#define CR_SEND_KEEP     0x22   // Send keep alive message
#define CR_RECV          0x40   // Update Rx memory buffer pointer and receive data

#define TCP_PORT 502
uint8_t sockstat;
uint16_t rsize;
int getidx,get0idx,get1idx,get2idx,get3idx,risidx,ris1idx;
int cC=0,ii=0,nn=0,nc=0;


uint16_t Webserver_recv_size(void)
{
	return ((W5100read(WebserverSocket_RX_RSR) & 0x00FF) << 8 ) + W5100read(WebserverSocket_RX_RSR + 1);
}
void WebserverSocket_close()
{
	// if (sock != 0) return;
	// Send Close Command
	W5100write(WebserverSocket_CR,CR_CLOSE);
	// Waiting until the S0_CR is clear
	while(W5100read(WebserverSocket_CR));
}
uint8_t WebserverSocketOpen(uint8_t sock,uint8_t eth_protocol,uint16_t tcp_port)
{
	uint8_t retval=0;

	//if (sock != 0) return retval;
	// Make sure we close the socket first
	if (W5100read(WebserverSocket_SR) == SOCK_CLOSED) {
		WebserverSocket_close();
	};
	// Assigned Socket 0 Mode Register
	W5100write(WebserverSocket_MR,eth_protocol);
	// Now open the Socket 0
	W5100write(WebserverSocket_PORT,((tcp_port & 0xFF00) >> 8 ));
	W5100write(WebserverSocket_PORT + 1,(tcp_port & 0x00FF));
	W5100write(WebserverSocket_CR,CR_OPEN);                   // Open Socket
	// Wait for Opening Process
	while(W5100read(WebserverSocket_CR));
	// Check for Init Status
	if (W5100read(WebserverSocket_SR) == SOCK_INIT)
	retval=1;
	else
	WebserverSocket_close();

	return retval;
}

uint8_t Webserver_listen()
{
	uint8_t retval = 0;
	
	if (W5100read(WebserverSocket_SR) == SOCK_INIT) {
		// Send the LISTEN Command
		W5100write(WebserverSocket_CR,CR_LISTEN);
		// Wait for Listening Process
		while(W5100read(WebserverSocket_CR));
		// Check for Listen Status
		if (W5100read(WebserverSocket_SR) == SOCK_LISTEN)
		retval=1;
		else
		WebserverSocket_close();
	}
	return retval;
}
uint16_t Webserver_recv(uint8_t *buf,uint16_t buflen)
{
	uint16_t ptr,offaddr,realaddr;
	if (buflen <= 0) return 1;
	// If the request size > MAX_BUF,just truncate it
	if (buflen > MAX_BUF)
	buflen=MAX_BUF - 2;
	// Read the Rx Read Pointer
	ptr = W5100read(WebserverSocket_RX_RD);
	offaddr = (((ptr & 0x00FF) << 8 ) + W5100read(WebserverSocket_RX_RD + 1));
	while(buflen) {
		buflen--;
		realaddr=RXBUFADDR + (offaddr & RX_BUF_MASK);
		*buf = W5100read(realaddr);
		offaddr++;
		buf++;
	}
	*buf='\0';        // String terminated character
	// Increase the S0_RX_RD value, so it point to the next receive
	W5100write(WebserverSocket_RX_RD,(offaddr & 0xFF00) >> 8 );
	W5100write(WebserverSocket_RX_RD + 1,(offaddr & 0x00FF));

	// Now Send the RECV command
	W5100write(WebserverSocket_CR,CR_RECV);
	_delay_us(5);    // Wait for Receive Process
	return 1;
}
void WebserverSocket_disconnect()
{
	
	// Send Disconnect Command
	W5100write(WebserverSocket_CR,CR_DISCON);
	// Wait for Disconecting Process
	while(W5100read(WebserverSocket_CR));
}
uint16_t Webserver_send(const uint8_t *buf,uint16_t buflen)
{
	uint16_t ptr,offaddr,realaddr,txsize,timeout;
	if (buflen <= 0 ) return 0;
	// Make sure the TX Free Size Register is available
	txsize=W5100read(WebserverSocket_TX_FSR);
	txsize=(((txsize & 0x00FF) << 8 ) + W5100read(WebserverSocket_TX_FSR + 1));
	timeout=0;
	while (txsize < buflen) {
		_delay_ms(1);
		txsize=W5100read(WebserverSocket_TX_FSR);
		txsize=(((txsize & 0x00FF) << 8 ) + W5100read(WebserverSocket_TX_FSR + 1));
		// Timeout for approx 1000 ms
		if (timeout++ > 1000) {
			// Disconnect the connection
			WebserverSocket_disconnect();
			return 0;
		}
	}
	// Read the Tx Write Pointer
	ptr = W5100read(WebserverSocket_TX_WR);
	offaddr = (((ptr & 0x00FF) << 8 ) + W5100read(WebserverSocket_TX_WR + 1));
	while(buflen) {
		buflen--;
		// Calculate the real W5100 physical Tx Buffer Address
		realaddr = TXBUFADDR + (offaddr & TX_BUF_MASK);
		// Copy the application data to the W5100 Tx Buffer
		W5100write(realaddr,*buf);
		offaddr++;
		buf++;
	}
	// Increase the S0_TX_WR value, so it point to the next transmit
	W5100write(WebserverSocket_TX_WR,(offaddr & 0xFF00) >> 8 );
	W5100write(WebserverSocket_TX_WR + 1,(offaddr & 0x00FF));
	// Now Send the SEND command
	W5100write(WebserverSocket_CR,CR_SEND);
	// Wait for Sending Process
	while(W5100read(WebserverSocket_CR));
	return 1;
}
int strindex(char *s,char *t)
{
	uint16_t i,n;
	n=strlen(t);
	for(i=0;*(s+i); i++) {
		if (strncmp(s+i,t,n) == 0)
		return i;
	}
	return -1;
}
#define MB_N_R 125
#define  MB_N_C 128

  uint16_t R[MB_TCP_HOLDING_REGISTERS_QT];
  uint16_t IR[MB_TCP_INPUT_REGISTERS_QT];
  uint8_t C[MB_N_C];
enum MB_FC {
	MB_FC_NONE				   = 0,
	MB_FC_READ_COILS		   = 1,
	MB_FC_READ_DISCRETE_INPUTS = 2,
	MB_FC_READ_REGISTERS	   = 3,
	MB_FC_READ_INPUT_REGISTERS = 4,
	MB_FC_WRITE_COIL		   = 5,
	MB_FC_WRITE_REGISTER	   = 6,
	//Function codes 15 & 16 by Martin Pettersson http://siamect.com
	MB_FC_WRITE_MULTIPLE_COILS = 15,
	MB_FC_WRITE_MULTIPLE_REGISTERS = 16
};
uint16_t word(uint8_t high,uint8_t low){
	uint16_t res = ((uint16_t)high<<8) + low;
	return res;
}

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

void SendErrorWrongAddress(uint8_t *buf){
	buf[7]|=1<<7;
	buf[8]|=0x02;	
	Webserver_send(buf,9);
}

void ModbusTCPRun(uint8_t FC)
{ 

  uint16_t Start, WordDataLength, ByteDataLength, CoilDataLength, MessageLength;

  //****************** Read Coils **********************
  if(FC == MB_FC_READ_COILS)
  {
    Start = word(buf[8],buf[9]);

    CoilDataLength = word(buf[10],buf[11]);
	if(((Start + CoilDataLength) > MB_TCP_COILS_QT)){
			SendErrorWrongAddress(buf);
			return;
	}
    ByteDataLength = CoilDataLength / 8;
    if(ByteDataLength * 8 < CoilDataLength) ByteDataLength++;      
    CoilDataLength = ByteDataLength * 8;
    #ifdef MbDebug
      SerialPrint(" MB_FC_READ_COILS S=");
      SerialPrintUint16_t(Start);
      SerialPrint(" L=");
      SerialPrintUint16_t(CoilDataLength);
    #endif
    buf[5] = ByteDataLength + 3; //Number of bytes after this one.
    buf[8] = ByteDataLength;     //Number of bytes after this one (or number of bytes of data).
    for(int i = 0; i < ByteDataLength ; i++)
    {
      for(int j = 0; j < 8; j++)
      {
        //bitWrite(buf[9 + i], j, C[Start + i * 8 + j]);
		#ifdef MODBUS
		if((i*8+j)<CoilDataLength){
		bitWrite(buf[9 + i], j, MB_Coil_Vars[Start + i * 8 + j]);
		}
		#endif
      }
    }
    MessageLength = ByteDataLength + 9;
	Webserver_send(buf,MessageLength);
 //   client.write(ByteArray, MessageLength);
//    Writes = 1 + Writes * (Writes < 999);
    FC = MB_FC_NONE;
  }

  //****************** Read Discrete Inputs **********************
  if(FC == MB_FC_READ_DISCRETE_INPUTS)
  {
    Start = word(buf[8],buf[9]);
    CoilDataLength = word(buf[10],buf[11]);
	if(((Start + CoilDataLength) > MB_TCP_DISCRETE_INPUTS_QT)){
			SendErrorWrongAddress(buf);
			return;
		}
    ByteDataLength = CoilDataLength / 8;
    if(ByteDataLength * 8 < CoilDataLength) ByteDataLength++;      
    CoilDataLength = ByteDataLength * 8;
    #ifdef MbDebug
      SerialPrint(" MB_FC_READ_COILS S=");
      SerialPrintUint16_t(Start);
      SerialPrint(" L=");
      SerialPrintUint16_t(CoilDataLength);
    #endif
    buf[5] = ByteDataLength + 3; //Number of bytes after this one.
    buf[8] = ByteDataLength;     //Number of bytes after this one (or number of bytes of data).
    for(int i = 0; i < ByteDataLength ; i++)
    {
      for(int j = 0; j < 8; j++)
      {
		#ifdef MODBUS
			bitWrite(buf[9 + i], j, MB_Input_Vars[Start + i * 8 + j]);
		#endif
      }
    }
    MessageLength = ByteDataLength + 9;
	Webserver_send(buf,MessageLength);
    FC = MB_FC_NONE;
  }
  //****************** Read Holding Registers ******************
  if(FC == MB_FC_READ_REGISTERS)
  {
    Start = word(buf[8],buf[9]);
    WordDataLength = word(buf[10],buf[11]);
	if(((Start + WordDataLength) > MB_TCP_HOLDING_REGISTERS_QT)){
			SendErrorWrongAddress(buf);
			return;
	}
	//WordDataLength = 11;
    ByteDataLength = WordDataLength * 2;
    #ifdef Modbus_TCP_Automat_LOGGING
      SerialPrint(" MB_FC_READ_REGISTERS S=");
      SerialPrintUint16_t(Start);
      SerialPrint(" L=");
      SerialPrintUint16_t(WordDataLength);
	  SerialPrintEndl();
    #endif

    buf[5] = ByteDataLength + 3; //Number of bytes after this one.
    buf[8] = ByteDataLength;     //Number of bytes after this one (or number of bytes of data).
    for(int i = 0; i < WordDataLength; i++)
    {
	  #ifdef MODBUS
		R[Start + i] = MB_HoldReg[Start + i];
	  #endif
      buf[ 9 + i * 2] = highByte(R[Start + i]);
      buf[10 + i * 2] =  lowByte(R[Start + i]);
    }
    MessageLength = ByteDataLength + 9;
    Webserver_send(buf, MessageLength);
    FC = MB_FC_NONE;
  }

//****************** Read Input Registers ******************
    if(FC == MB_FC_READ_INPUT_REGISTERS)
    {
	    Start = word(buf[8],buf[9]);
	    WordDataLength = word(buf[10],buf[11]);
		if(((Start + WordDataLength) > MB_TCP_INPUT_REGISTERS_QT)){
			SendErrorWrongAddress(buf);
			return;
		}
	    ByteDataLength = WordDataLength * 2;
	    #ifdef Modbus_TCP_Automat_LOGGING
			SerialPrint(" MB_FC_READ_REGISTERS S=");
			SerialPrintUint16_t(Start);
			SerialPrint(" L=");
			SerialPrintUint16_t(WordDataLength);
			SerialPrintEndl();
	    #endif
	    #ifdef MODBUS
		    IR[Start] = MB_InReg[Start];
	    #endif
	    buf[5] = ByteDataLength + 3; //Number of bytes after this one.
	    buf[8] = ByteDataLength;     //Number of bytes after this one (or number of bytes of data).
	    for(int i = 0; i < WordDataLength; i++)
	    {
				buf[ 9 + i * 2] = highByte(IR[Start + i]);
				buf[10 + i * 2] =  lowByte(IR[Start + i]);
			#ifdef MODBUS
				buf[ 9 + i * 2] = highByte(MB_InReg[Start + i]);
				buf[10 + i * 2] =  lowByte(MB_InReg[Start + i]);				
			#endif
	    }
	    MessageLength = ByteDataLength + 9;
	    Webserver_send(buf, MessageLength);
	    //Writes = 1 + Writes * (Writes < 999);
	    FC = MB_FC_NONE;
    }
  //****************** Write Coil **********************
  if(FC == MB_FC_WRITE_COIL)
  {
    Start = word(buf[8],buf[9]);
  //  C[Start] = word(buf[10],buf[11]) > 0;
  	if(((Start) > MB_TCP_COILS_QT)){
			SendErrorWrongAddress(buf);
	 		return;
  	}
	#ifdef MODBUS
	MB_Coil_Vars[Start] = word(buf[10],buf[11]) > 0;
	#endif
    #ifdef MbDebug
      SerialPrint(" MB_FC_WRITE_COIL C");
      SerialPrintUint16_t(Start);
      SerialPrint("=");
      SerialPrintUint16_t(C[Start]);
	  SerialPrintEndl();
    #endif
    buf[5] = 6; //Number of bytes after this one.
    MessageLength = 12;
	Webserver_send(buf,MessageLength);
    //client.write(ByteArray, MessageLength);
    //Writes = 1 + Writes * (Writes < 999);
    FC = MB_FC_NONE;
  }

  //****************** Write Holding Register ******************
  if(FC == MB_FC_WRITE_REGISTER)
  {
    Start = word(buf[8],buf[9]);
	if(((Start) > MB_TCP_HOLDING_REGISTERS_QT)){
			SendErrorWrongAddress(buf);
			return;
	}
    R[Start] = word(buf[10],buf[11]);
	#ifdef MODBUS
	 MB_HoldReg[Start] = R[Start];
	#endif
    #ifdef MbDebug
      SerialPrint(" MB_FC_WRITE_REGISTER R");
      SerialPrint(Start);
      SerialPrint("=");
      SerialPrintln(R[Start]);
    #endif
    buf[5] = 6; //Number of bytes after this one.
    MessageLength = 12;
    Webserver_send(buf, MessageLength);
   // Writes = 1 + Writes * (Writes < 999);
    FC = MB_FC_NONE;
  }


  //****************** Write Multiple Coils **********************
  //Function codes 15 & 16 by Martin Pettersson http://siamect.com
  if(FC == MB_FC_WRITE_MULTIPLE_COILS)
  {  
	uint16_t allCoils = 0;
    Start = word(buf[8],buf[9]);
    CoilDataLength = word(buf[10],buf[11]);
	allCoils = CoilDataLength;
	if(((Start + CoilDataLength) > MB_TCP_COILS_QT)){
		SendErrorWrongAddress(buf);
		return;
	}
    ByteDataLength = CoilDataLength / 8;
    if(ByteDataLength * 8 < CoilDataLength) ByteDataLength++;
    CoilDataLength = ByteDataLength * 8;
    #ifdef MbDebug
      SerialPrint(" MB_FC_WRITE_MULTIPLE_COILS S=");
      SerialPrintUint16_t(Start);
      SerialPrint(" L=");
      SerialPrintUint16_t(CoilDataLength);
    #endif
    buf[5] = 6 ; //Number of bytes after this one.
    for(int i = 0; i < ByteDataLength ; i++)
    {
      for(int j = 0; j < 8; j++)
      {
       // C[Start + i * 8 + j] = bitRead( buf[13 + i], j);
	   if(((i*8)+j)<allCoils){
			#ifdef MODBUS
			MB_Coil_Vars[Start + i * 8 + j] = bitRead( buf[13 + i], j); 
			#endif
		} 
      }
    }
    MessageLength = 12;
    Webserver_send(buf, MessageLength);
   // Writes = 1 + Writes * (Writes < 999);
    FC = MB_FC_NONE;
  }
 

  //****************** Write Multiple Registers ******************
  //Function codes 15 & 16 by Martin Pettersson http://siamect.com
  if(FC == MB_FC_WRITE_MULTIPLE_REGISTERS)
  {
    Start = word(buf[8],buf[9]);
    WordDataLength = word(buf[10],buf[11]);
    ByteDataLength = WordDataLength * 2;
	if(((Start + WordDataLength) > MB_TCP_HOLDING_REGISTERS_QT)){
		SendErrorWrongAddress(buf);
		return;
	}
    #ifdef MbDebug
      SerialPrint(" MB_FC_READ_REGISTERS S=");
      SerialPrintUint16_t(Start);
      SerialPrint(" L=");
      SerialPrintUint16_t(WordDataLength);
	  SerialPrintEndl();
    #endif
  //  buf[5] = ByteDataLength + 3; //Number of bytes after this one.
	buf[5] = 6; //Number of bytes after this one.
    for(int i = 0; i < WordDataLength; i++)
    {
      R[Start + i] =  word(buf[ 13 + i * 2],buf[14 + i * 2]);
	  #ifdef MODBUS
		 MB_HoldReg[Start + i] = R[Start + i];
	  #endif
    }
    MessageLength = 12;
    Webserver_send(buf, MessageLength);
    //Writes = 1 + Writes * (Writes < 999);

    FC = MB_FC_NONE;
  }
/*
  #ifdef MbDebug
    Serial.print("Mb runs: ");
    Serial.print(Runs);
    Serial.print("  reads: ");
    Serial.print(Reads);
    Serial.print("  writes: ");
    Serial.print(Writes);
    Serial.println();
  #endif
  */
}


uint8_t SetFC(int fc)
{
  uint8_t FC = 0;
	  if(fc == 1) FC = MB_FC_READ_COILS;
	  if(fc == 2) FC = MB_FC_READ_DISCRETE_INPUTS;
	  if(fc == 3) FC = MB_FC_READ_REGISTERS;
	  if(fc == 4) FC = MB_FC_READ_INPUT_REGISTERS;
	  if(fc == 5) FC = MB_FC_WRITE_COIL;
	  if(fc == 6) FC = MB_FC_WRITE_REGISTER;
	  if(fc == 15) FC = MB_FC_WRITE_MULTIPLE_COILS;
	  if(fc == 16) FC = MB_FC_WRITE_MULTIPLE_REGISTERS;
  return FC;
}


uint8_t Modbus_TCP_Automat(uint8_t event)
{
	static uint8_t state=0;
	uint8_t stateOld=state;
	//static uint32_t Timer;
	static uint8_t Timer = 0;
	static uint32_t clo=0;
	#ifdef Modbus_TCP_Automat_LOGGING
		SerialPrint("  Modbus_automat_Start_state:") ;
		SerialPrintUint8_t(state);
		SerialPrintEndl(); 
	#endif
	switch(event){
		case 0:
		state=0;
		break;
		default:
		break;
	}
	switch(state)
	{
		case 0:
		if(Timer == 0)
			{
				Timer = Timer16Alloc();
			}
			state=1;
		break;
		
		case 1:
			sockstat=W5100read(WebserverSocket_SR);
			#ifdef Modbus_TCP_Automat_LOGGING
				SerialPrint("SOCKET_STATE:");
				SerialPrintUint8_t(sockstat);
				SerialPrintEndl();
			#endif
			switch(sockstat)
			{
				case SOCK_CLOSED:
				state = 2;
				break;
				case SOCK_ESTABLISHED:
				if(clo==0){
					state=5;
					//Timer=millis();
					StartTimer16(Timer,1);
					}
				else{
					state=6;
					//Timer=millis();
					StartTimer16(Timer,1);
				}
				break;
				case SOCK_INIT:
					state = 3;
					break;

				case SOCK_FIN_WAIT:
				case SOCK_CLOSING:
				case SOCK_TIME_WAIT:
				case SOCK_CLOSE_WAIT:
				case SOCK_LAST_ACK:

				// Force to close the socket
				WebserverSocket_close();
				break;
			}
			break;

		case 2:
			if(WebserverSocketOpen(WebserverSocketNum, MR_TCP, TCP_PORT)>0)
			{
				state=3;
			}
			else{
				state=1;
			};
			break;


		case 3:
			if(Webserver_listen()<=0){
						state=4;
					}else{
						state=1;
						}
					break;
					
		case 4:
			state=1;
			break;
			
		case 5:
			if(Timer16Stopp(Timer)){
				StartTimer16(Timer,1);
				state=6;
			};
			break;
			
		case 6:
			if(Timer16Stopp(Timer)){
				if(clo>=1000 && rsize==0){
					StartTimer16(Timer,50);
					state=7;
				}
				else{
					if(rsize>0)
					{
						state=9;
					}
					else{
						state=1;
					}
				}
			}
			break;
		case 7:
			if(Timer16Stopp(Timer)){
				clo=0;
				state=8;
				break;
			}
			break;
		case 8:
			rsize=Webserver_recv_size();
			if(rsize>0){
					state=9;
					break;
				}else{
					WebserverSocket_disconnect();
					state=1;
				}
				break;
		case 9:
				clo=0;
				Webserver_recv(buf,rsize);
				#ifdef Modbus_TCP_Automat_LOGGING
					SerialPrintln("FUNCTION:");
					SerialPrintUint8_t(buf[7]);
					SerialPrintEndl();
					for(int i = 0; i<rsize; i++){
						SerialPrintUint8_t(buf[i]);
						SerialPrint(",");
					}
					SerialPrintEndl();
				#endif
				 
				ModbusTCPRun(SetFC(buf[7]));			
				state = 1;
				break;
		case 13:
				WebserverSocket_disconnect();
				state=1;
				break;
		case 14:
								
				break;
		case 15:
				WebserverSocket_disconnect();
				WebserverSocket_close();
				state=1;
				break;
		case 255:
				
				break;
			}
			if(state!=stateOld){
				switch(state)
				{
					case 0:
					break;
					
					case 1:

					break;
					
					case 2:

					break;
					
					case 4:

					break;

					case 6:
						rsize=Webserver_recv_size();
						clo++;
					break;

					case 13:

					break;
					case 255:
					break;
				}
			}
			#ifdef Modbus_TCP_Automat_LOGGING
				SerialPrint("  Modbus_automat_End_state:") ;
				SerialPrintUint8_t(state);
				SerialPrintEndl();
			#endif
			return state;
		}

#endif /* MODBUS_TCP_AUTOMAT_H_ */