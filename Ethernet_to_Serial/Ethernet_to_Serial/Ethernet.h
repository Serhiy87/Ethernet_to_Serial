/*
 * Ethernet.h
 *
 * Created: 04.05.2017 9:42:59
 *  Author: Admin
 */ 


#ifndef ETHERNET_H_
#define ETHERNET_H_
#include "W5100.h"
#include "udp_automat.h"
#include "TEST_Automat.h"
#include "DHCP_Automat.h"
#include "SNMP_Automat.h"
#include "Webserver_Automat.h"

uint8_t DHCP_Enable=1;
uint8_t static_IP[4]={10,0,0,76};
uint8_t Ethernet_Automat(uint8_t event);
extern void resetEthernet(void){
	
	Ethernet_Automat(0);
	//SerialPrintln("RESET ETHERNET!!!");
}

uint8_t Ethernet_Automat(uint8_t event){
	static uint8_t state=0;
	uint8_t stateOld=state;
	#ifdef Ethernet_Automat_BEGIN_LOGGING
	SerialPrint("Ethernet_Automat ") ;
	SerialPrint("start state:") ;
	SerialPrintUint8_t(state) ;
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
		state=1;
		break;
		
		case 1:
	/*	SPI_Init();
		if(ResetW5100_automat(1)!=255)
		{
			break;
		}*/
		W5100_init();
		W5100_SetMac(0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02);
		_dhcpMacAddr[0]=0x00;
		_dhcpMacAddr[1]=0xAA;
		_dhcpMacAddr[2]=0xBB;
		_dhcpMacAddr[3]=0xCC;
		_dhcpMacAddr[4]=0xDE;
		_dhcpMacAddr[5]=0x02;
		#ifdef Ethernet_Automat_BEGIN_LOGGING
		SerialPrint("Buffer Mac: ");
		SerialPrintUint8_t(_dhcpMacAddr[0]);
		SerialPrint(":");
		SerialPrintUint8_t(_dhcpMacAddr[1]);
		SerialPrint(":");
		SerialPrintUint8_t(_dhcpMacAddr[2]);
		SerialPrint(":");
		SerialPrintUint8_t(_dhcpMacAddr[3]);
		SerialPrint(":");
		SerialPrintUint8_t(_dhcpMacAddr[4]);
		SerialPrint(":");
		SerialPrintUint8_t(_dhcpMacAddr[5]);
		SerialPrintEndl();
		#endif
		// W5100_SetMac(0x00, 0xDD, 0xFF, 0x01, 0xDE, 0x02);
		if(!DHCP_Enable){
			W5100_SetIP(static_IP[0],static_IP[1],static_IP[2],static_IP[3]);
		}
		else
		{
			W5100_SetIP(0,0,0,0);
		}
		state=2;
		break;

		case 2:
			DHCP_automat(1);
			SNMP_Automat(1);
			TEST_Automat(1);
			Webserver_Automat(1);
		break;
	}
	if(state!=stateOld){
		switch(state)
		{
			case 0:
			break;
			case 1:
			//ResetW5100_automat(0);
			break;
			case 2:
				DHCP_automat(0);
				SNMP_Automat(0);
				TEST_Automat(0);
				Webserver_Automat(0);
			break;
		}
	}
	#ifdef Ethernet_Automat_BEGIN_LOGGING
	SerialPrint("Ethernet_Automat ") ;
	SerialPrint("end state:") ;
	SerialPrintUint8_t(state) ;
	SerialPrintEndl();
	#endif
	return state;
}


#endif /* ETHERNET_H_ */