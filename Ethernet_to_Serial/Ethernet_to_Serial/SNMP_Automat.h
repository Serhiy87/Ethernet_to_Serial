/*
 * SNMP_Automat.h
 *
 * Created: 12.05.2017 9:30:39
 *  Author: Admin
 */ 


#ifndef SNMP_AUTOMAT_H_
#define SNMP_AUTOMAT_H_

#include "String.h"
#include "avr/pgmspace.h"
#include "UDP_Automat.h"
#include "stdio.h"

byte count=0;
extern
#define SNMP_DEFAULT_PORT  161
#define SNMP_MIN_OID_LEN  2
#define SNMP_MAX_OID_LEN  64 // 128
#define SNMP_MAX_NAME_LEN 20
#define SNMP_MAX_VALUE_LEN      64  // 128 ??? should limit this
#define SNMP_MAX_PACKET_LEN     SNMP_MAX_VALUE_LEN + SNMP_MAX_OID_LEN + 25  //???
#define SNMP_MAX_VALUES_COUNT 10
#define SNMP_FREE(s)   do { if (s) { free((void *)s); s=NULL; } } while(0)
#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))
uint8_t NonRepeaters, MaxRepetition;
uint16_t BulkValuesIndexes[SNMP_MAX_VALUES_COUNT];
uint8_t BulkValuesCount;

 int32_t ledstate=0;

typedef union  {
	uint64_t uint64;
	byte data[8];
}uint64_u;

typedef union  {
	int32_t int32;
	byte data[4];
}int32_u;

typedef union  {
	uint32_t uint32;
	byte data[4];
}uint32_u;

typedef union  {
	int16_t int16;
	byte data[2];
}int16_u;


//typedef union uint16_u {
//  uint16_t uint16;
//  byte data[2];
//};


typedef enum A {
	//   ASN/BER base types
	ASN_BER_BASE_UNIVERSAL   = 0x0,
	ASN_BER_BASE_APPLICATION = 0x40,
	ASN_BER_BASE_CONTEXT   = 0x80,
	ASN_BER_BASE_PUBLIC    = 0xC0,
	ASN_BER_BASE_PRIMITIVE   = 0x0,
	ASN_BER_BASE_CONSTRUCTOR = 0x20
}SN_BER_BASE_TYPES;

typedef enum {
	// PDU choices
	SNMP_PDU_GET    = ASN_BER_BASE_CONTEXT | ASN_BER_BASE_CONSTRUCTOR | 0,
	SNMP_PDU_GET_NEXT = ASN_BER_BASE_CONTEXT | ASN_BER_BASE_CONSTRUCTOR | 1,
	SNMP_PDU_RESPONSE = ASN_BER_BASE_CONTEXT | ASN_BER_BASE_CONSTRUCTOR | 2,
	SNMP_PDU_SET    = ASN_BER_BASE_CONTEXT | ASN_BER_BASE_CONSTRUCTOR | 3,
	SNMP_PDU_TRAP   = ASN_BER_BASE_CONTEXT | ASN_BER_BASE_CONSTRUCTOR | 4,
	SNMP_PDU_GET_BULK   = ASN_BER_BASE_CONTEXT | ASN_BER_BASE_CONSTRUCTOR | 5
}SNMP_PDU_TYPES ;


byte _packet[SNMP_MAX_PACKET_LEN];
uint16_t _packetSize;
uint16_t _packetPos;
SNMP_PDU_TYPES _dstType;
uint8_t _dstIp[4];
uint16_t _dstPort;
const char *_getCommName;
size_t _getSize;
const char *_setCommName;
size_t _setSize;

typedef enum  {
	//   Trap generic types:
	SNMP_TRAP_COLD_START          = 0,
	SNMP_TRAP_WARM_START          = 1,
	SNMP_TRAP_LINK_DOWN           = 2,
	SNMP_TRAP_LINK_UP			  = 3,
	SNMP_TRAP_AUTHENTICATION_FAIL = 4,
	SNMP_TRAP_EGP_NEIGHBORLOSS    = 5,
	SNMP_TRAP_ENTERPRISE_SPECIFIC = 6
}SNMP_TRAP_TYPES;

typedef enum{
	SNMP_ERR_NO_ERROR         = 0,
	SNMP_ERR_TOO_BIG		  = 1,
	SNMP_ERR_NO_SUCH_NAME     = 2,
	SNMP_ERR_BAD_VALUE        = 3,
	SNMP_ERR_READ_ONLY        = 4,
	SNMP_ERR_GEN_ERROR        = 5,

	SNMP_ERR_NO_ACCESS        = 6,
	SNMP_ERR_WRONG_TYPE       = 7,
	SNMP_ERR_WRONG_LENGTH     = 8,
	SNMP_ERR_WRONG_ENCODING   = 9,
	SNMP_ERR_WRONG_VALUE      = 10,
	SNMP_ERR_NO_CREATION      = 11,
	SNMP_ERR_INCONSISTANT_VALUE     = 12,
	SNMP_ERR_RESOURCE_UNAVAILABLE   = 13,
	SNMP_ERR_COMMIT_FAILED      = 14,
	SNMP_ERR_UNDO_FAILED      = 15,
	SNMP_ERR_AUTHORIZATION_ERROR    = 16,
	SNMP_ERR_NOT_WRITABLE     = 17,
	SNMP_ERR_INCONSISTEN_NAME   = 18
} SNMP_ERR_CODES;

typedef enum{
	SNMP_API_STAT_SUCCESS = 0,
	SNMP_API_STAT_MALLOC_ERR = 1,
	SNMP_API_STAT_NAME_TOO_BIG = 2,
	SNMP_API_STAT_OID_TOO_BIG = 3,
	SNMP_API_STAT_VALUE_TOO_BIG = 4,
	SNMP_API_STAT_PACKET_INVALID = 5,
	SNMP_API_STAT_PACKET_TOO_BIG = 6,
	SNMP_API_STAT_NO_SUCH_NAME = 7,
} SNMP_API_STAT_CODES ;

SNMP_API_STAT_CODES api_status;
//
// http://oreilly.com/catalog/esnmp/chapter/ch02.html Table 2-1: SMIv1 Datatypes

typedef enum{
	//   SNMP ObjectSyntax values
	SNMP_SYNTAX_SEQUENCE         = ASN_BER_BASE_UNIVERSAL | ASN_BER_BASE_CONSTRUCTOR | 0x10,
	//   These values are used in the "syntax" member of VALUEs
	SNMP_SYNTAX_BOOL         = ASN_BER_BASE_UNIVERSAL | ASN_BER_BASE_PRIMITIVE | 1,
	SNMP_SYNTAX_INT          = ASN_BER_BASE_UNIVERSAL | ASN_BER_BASE_PRIMITIVE | 2,
	SNMP_SYNTAX_BITS         = ASN_BER_BASE_UNIVERSAL | ASN_BER_BASE_PRIMITIVE | 3,
	SNMP_SYNTAX_OCTETS       = ASN_BER_BASE_UNIVERSAL | ASN_BER_BASE_PRIMITIVE | 4,
	SNMP_SYNTAX_NULL         = ASN_BER_BASE_UNIVERSAL | ASN_BER_BASE_PRIMITIVE | 5,
	SNMP_SYNTAX_OID          = ASN_BER_BASE_UNIVERSAL | ASN_BER_BASE_PRIMITIVE | 6,
	SNMP_SYNTAX_INT32        = SNMP_SYNTAX_INT,
	SNMP_SYNTAX_IP_ADDRESS   = ASN_BER_BASE_APPLICATION | ASN_BER_BASE_PRIMITIVE | 0,
	SNMP_SYNTAX_COUNTER      = ASN_BER_BASE_APPLICATION | ASN_BER_BASE_PRIMITIVE | 1,
	SNMP_SYNTAX_GAUGE        = ASN_BER_BASE_APPLICATION | ASN_BER_BASE_PRIMITIVE | 2,
	SNMP_SYNTAX_TIME_TICKS   = ASN_BER_BASE_APPLICATION | ASN_BER_BASE_PRIMITIVE | 3,
	SNMP_SYNTAX_OPAQUE       = ASN_BER_BASE_APPLICATION | ASN_BER_BASE_PRIMITIVE | 4,
	SNMP_SYNTAX_NSAPADDR     = ASN_BER_BASE_APPLICATION | ASN_BER_BASE_PRIMITIVE | 5,
	SNMP_SYNTAX_COUNTER64    = ASN_BER_BASE_APPLICATION | ASN_BER_BASE_PRIMITIVE | 6,
	SNMP_SYNTAX_UINT32       = ASN_BER_BASE_APPLICATION | ASN_BER_BASE_PRIMITIVE | 7,
} SNMP_SYNTAXES ;

typedef struct  {
	byte data[SNMP_MAX_OID_LEN];  // ushort array insted??
	size_t size;
	//

	//

}SNMP_OID;


	void SNMP_OID_toString(SNMP_OID oid, char *buffer) {
		buffer[0] = '1';
		buffer[1] = '.';
		buffer[2] = '3';
		buffer[3] = '\0';
		//
		char buff[16];
		byte hsize = oid.size - 1;
		byte hpos = 1;
		uint16_t subid;
		//
		while ( hsize > 0 ) {
			subid = 0;
			uint16_t b = 0;
			do {
				uint16_t next = oid.data[hpos++];
				b = next & 0xFF;
				subid = (subid << 7) + (b & ~0x80);
				hsize--;
			} while ( (hsize > 0) && ((b & 0x80) != 0) );
			utoa(subid, buff, 10);
			strcat(buffer, ".");
			strcat(buffer, buff);
		}
	};

void SNMP_OID_fromString(SNMP_OID *oid, const char *buffer) {
					if (buffer[0] == '1' && buffer[1] == '.' && buffer[2] == '3' && buffer[3] == '.') {
						memset(oid->data, 0, SNMP_MAX_OID_LEN);
						oid->data[0] = 0x2B;
						int fs_ilen = strlen(buffer);
						int fs_ic = 0;
						int fs_id = 1;
						char fs_Csl[5];
						memset(fs_Csl, 0, 5);
						for (int fs_i = 4; fs_i < fs_ilen; fs_i++){
							if (buffer[fs_i] == '.') {
								uint16_t fs_oidw = atol(fs_Csl);
								if (fs_oidw < 128) {
									oid->data[fs_id] = fs_oidw;
									} else if (fs_oidw < 16384 ) {
										byte fs_loidb = lowByte(fs_oidw) & 127;
										uint16_t fs_oidwtmp = fs_oidw << 1;
										byte fs_hoidb = highByte(fs_oidwtmp) | 128;
										oid->data[fs_id] = fs_hoidb;
										fs_id++;
										oid->data[fs_id] = fs_loidb;
									} else {
										byte fs_loidb = lowByte(fs_oidw) & 127;
										uint16_t fs_oidwtmp = fs_oidw << 1;
										byte fs_hoidb = highByte(fs_oidwtmp) | 128;
										fs_oidwtmp = fs_oidw >> 14;
										byte fs_hhoidb = (lowByte(fs_oidwtmp) & 3) | 128;
										oid->data[fs_id] = fs_hhoidb;
										fs_id++;
										oid->data[fs_id] = fs_hoidb;
										fs_id++;
										oid->data[fs_id] = fs_loidb;
								}
								memset(fs_Csl, 0, 5);
								fs_ic = 0;
								fs_id++;
								} else {
								fs_Csl[fs_ic] = buffer[fs_i];
								fs_ic++;
							}
						}
						uint16_t fs_oidw = atoi(fs_Csl);
						if (fs_oidw < 128) {
							oid->data[fs_id] = fs_oidw;
							} else if (fs_oidw < 16384 ) {
							byte fs_loidb = lowByte(fs_oidw) & 127;
							uint16_t fs_oidwtmp = fs_oidw << 1;
							byte fs_hoidb = highByte(fs_oidwtmp) | 128;
							oid->data[fs_id] = fs_hoidb;
							fs_id++;
							oid->data[fs_id] = fs_loidb;
							}  else {
							byte fs_loidb = lowByte(fs_oidw) & 127;
							uint16_t fs_oidwtmp = fs_oidw << 1;
							byte fs_hoidb = highByte(fs_oidwtmp) | 128;
							fs_oidwtmp = fs_oidw >> 14;
							byte fs_hhoidb = (lowByte(fs_oidwtmp) & 3) | 128;
							oid->data[fs_id] = fs_hhoidb;
							fs_id++;
							oid->data[fs_id] = fs_hoidb;
							fs_id++;
							oid->data[fs_id] = fs_loidb;
						}

						fs_id++;
						oid->size = fs_id;
					}
				}

				//

	

// union for values?
//
typedef struct  {
	byte data[SNMP_MAX_VALUE_LEN];
	size_t size;
	SNMP_SYNTAXES syntax;
	byte i; // for encoding/decoding functions
}SNMP_VALUE;

//
// clear's buffer and sets size to 0
void clear(SNMP_VALUE *value) {
		memset(value->data, 0, SNMP_MAX_VALUE_LEN);
		value->size = 0;
	}


	//
	//
	// ASN.1 decoding functions (Преобразовывают из SNMP типов в Си типы)
	//
	// decode's an octet string, object-identifier, opaque syntax to string
	SNMP_ERR_CODES decodeOctetStringToString(SNMP_VALUE *snmp_value, char *value, size_t max_size) {
		if ( snmp_value->syntax == SNMP_SYNTAX_OCTETS || snmp_value->syntax == SNMP_SYNTAX_OID
		|| snmp_value->syntax == SNMP_SYNTAX_OPAQUE ) {
			if ( strlen(value) - 1 < max_size ) {
				if ( snmp_value->syntax == SNMP_SYNTAX_OID ) {
					value[0] = '1';
					value[1] = '.';
					value[2] = '3';
					value[3] = '\0';
					//
					char buff[16];
					byte hsize = snmp_value->size - 1;
					byte hpos = 1;
					uint16_t subid;
					//
					while ( hsize > 0 ) {
						subid = 0;
						uint16_t b = 0;
						do {
							uint16_t next = snmp_value->data[hpos++];
							b = next & 0xFF;
							subid = (subid << 7) + (b & ~0x80);
							hsize--;
						} while ( (hsize > 0) && ((b & 0x80) != 0) );
						utoa(subid, buff, 10);
						strcat(value, ".");
						strcat(value, buff);
					}
					} else {
					for ( snmp_value->i = 0; snmp_value->i < snmp_value->size; snmp_value->i++ ) {
						value[snmp_value->i] = (char)snmp_value->data[snmp_value->i];
					}
					value[snmp_value->size] = '\0';
				}
				return SNMP_ERR_NO_ERROR;
				} else {
				clear(snmp_value);
				return SNMP_ERR_TOO_BIG;
			}
			} else {
			clear(snmp_value);
			return SNMP_ERR_WRONG_TYPE;
		}
	}
	//
	// decode's an int syntax to int16
	SNMP_ERR_CODES decodeInt16toInt16(SNMP_VALUE *snmp_value, int16_t *value) {
		if ( snmp_value->syntax == SNMP_SYNTAX_INT ) {
			uint8_t *p = (uint8_t*)value, i;
			memset(value, 0, sizeof(*value));
			for(i = 0;i < snmp_value->size;i++)
			{
				*p++ = snmp_value->data[snmp_value->size - 1 - i];
			}
			return SNMP_ERR_NO_ERROR;
			} else {
			clear(snmp_value);
			return SNMP_ERR_WRONG_TYPE;
		}
	}

	SNMP_ERR_CODES decodeInt16toHoldReg(SNMP_VALUE *snmp_value, int16_t *value) {
			if ( snmp_value->syntax == SNMP_SYNTAX_INT ) {
				if((snmp_value->size)>2){
					return SNMP_ERR_TOO_BIG;
				}
				uint8_t *p = (uint8_t*)value, i;
				memset(value, 0, sizeof(*value));
				for(i = 0;i < snmp_value->size;i++)
				{
					*p++ = snmp_value->data[snmp_value->size - 1 - i];
				}
				return SNMP_ERR_NO_ERROR;
				} else {
				clear(snmp_value);
				return SNMP_ERR_WRONG_TYPE;
			}
		}
	//
	// decode's an int32 syntax to int32
	SNMP_ERR_CODES decodeInt32ToInt32(SNMP_VALUE *snmp_value, int32_t *value) {
		if ( snmp_value->syntax == SNMP_SYNTAX_INT32 ) {
			uint8_t *p = (uint8_t*)value, i;
			memset(value, 0, sizeof(*value));
			for(i = 0;i < snmp_value->size;i++)
			{
				*p++ = snmp_value->data[snmp_value->size - 1 - i];
			}
			return SNMP_ERR_NO_ERROR;
			} else {
			clear(snmp_value);
			return SNMP_ERR_WRONG_TYPE;
		}
	}
	//
	// decode's an uint32, counter, time-ticks, gauge syntax to uint32
	SNMP_ERR_CODES decodeUint32CounterTimeticksGaugeToUint32(SNMP_VALUE *snmp_value, uint32_t *value) {
		if ( snmp_value->syntax == SNMP_SYNTAX_COUNTER || snmp_value->syntax == SNMP_SYNTAX_TIME_TICKS
		|| snmp_value->syntax == SNMP_SYNTAX_GAUGE || snmp_value->syntax == SNMP_SYNTAX_UINT32 ) {
			uint8_t *p = (uint8_t*)value, i;
			memset(value, 0, sizeof(*value));
			for(i = 0;i < snmp_value->size;i++)
			{
				*p++ = snmp_value->data[snmp_value->size - 1 - i];
			}
			return SNMP_ERR_NO_ERROR;
			} else {
			clear(snmp_value);
			return SNMP_ERR_WRONG_TYPE;
		}
	}
	//
	// decode's an ip-address, NSAP-address syntax to an ip-address byte array
	SNMP_ERR_CODES decodeIpAddressToByteArray(SNMP_VALUE *snmp_value, byte *value) {
		memset(snmp_value->data, 0, SNMP_MAX_VALUE_LEN);
		if ( snmp_value->syntax == SNMP_SYNTAX_IP_ADDRESS || snmp_value->syntax == SNMP_SYNTAX_NSAPADDR ) {
			uint8_t *p = (uint8_t*)value, i;
			memset(value, 0, 4);
			for(i = 0;i < snmp_value->size;i++)
			{
				*p++ = snmp_value->data[snmp_value->size - 4 - i];
			}
			return SNMP_ERR_NO_ERROR;
			} else {
			clear(snmp_value);
			return SNMP_ERR_WRONG_TYPE;
		}
	}
	//
	// decode's a boolean syntax to boolean
	SNMP_ERR_CODES decodeBoolToUint8(SNMP_VALUE *snmp_value, uint8_t *value) {
		if ( snmp_value->syntax == SNMP_SYNTAX_BOOL ) {
			*value = (snmp_value->data[0] != 0);
			return SNMP_ERR_NO_ERROR;
			} else {
			clear(snmp_value);
			return SNMP_ERR_WRONG_TYPE;
		}
	}

//
//
// ASN.1 encoding functions (Преобразовывает типы Си в типы, котрые используются в SNMP (берем то что в value(Сишном) и заполняем поле SNMP_VALUE структуры SNMP_PDU согласно требованиям))
//
// encode's a octet string to a string, opaque syntax
// encode object-identifier here??
SNMP_ERR_CODES encodeStringToOctetString(SNMP_VALUE *snmp_value, SNMP_SYNTAXES syn, const char *value) {
	memset(snmp_value->data, 0, SNMP_MAX_VALUE_LEN);
	if ( syn == SNMP_SYNTAX_OCTETS || syn == SNMP_SYNTAX_OPAQUE ) {
		if ( strlen(value) - 1 < SNMP_MAX_VALUE_LEN ) {
			snmp_value->syntax = syn;
			snmp_value->size = strlen(value);
			for ( snmp_value->i = 0; snmp_value->i < snmp_value->size; snmp_value->i++ ) {
				snmp_value->data[snmp_value->i] = (byte)value[snmp_value->i];
			}
			return SNMP_ERR_NO_ERROR;
			} else {
			clear(snmp_value);
			return SNMP_ERR_TOO_BIG;
		}
		} else {
		clear(snmp_value);
		return SNMP_ERR_WRONG_TYPE;
	}
}
//
// encode's an int16 to int, opaque  syntax
SNMP_ERR_CODES encodeInt16ToInt(SNMP_VALUE *snmp_value, SNMP_SYNTAXES syn, const int16_t value) {
	memset(snmp_value->data, 0, SNMP_MAX_VALUE_LEN);
	if ( syn == SNMP_SYNTAX_INT || syn == SNMP_SYNTAX_OPAQUE ) {
		int16_u tmp;
		snmp_value->size = 2;
		snmp_value->syntax = syn;
		tmp.int16 = value;
		snmp_value->data[0] = tmp.data[1];
		snmp_value->data[1] = tmp.data[0];
		return SNMP_ERR_NO_ERROR;
		} else {
		clear(snmp_value);
		return SNMP_ERR_WRONG_TYPE;
	}
}
//
// encode's an int32 to int32, opaque  syntax
SNMP_ERR_CODES encodeInt32ToInt32(SNMP_VALUE *snmp_value, SNMP_SYNTAXES syn, const int32_t value) {
	memset(snmp_value->data, 0, SNMP_MAX_VALUE_LEN);
	if ( syn == SNMP_SYNTAX_INT32 || syn == SNMP_SYNTAX_OPAQUE ) {

		int32_u tmp;
		snmp_value->size = 4;
		snmp_value->syntax = syn;
		tmp.int32 = value;
		snmp_value->data[0] = tmp.data[3];
		snmp_value->data[1] = tmp.data[2];
		snmp_value->data[2] = tmp.data[1];
		snmp_value->data[3] = tmp.data[0];
		return SNMP_ERR_NO_ERROR;
		} else {
		clear(snmp_value);
		return SNMP_ERR_WRONG_TYPE;
	}
}
//
// encode's an uint32 to uint32, counter, time-ticks, gauge, opaque  syntax
SNMP_ERR_CODES encodeUint32ToUint32(SNMP_VALUE *snmp_value, SNMP_SYNTAXES syn, const uint32_t value) {
	memset(snmp_value->data, 0, SNMP_MAX_VALUE_LEN);
	if ( syn == SNMP_SYNTAX_COUNTER || syn == SNMP_SYNTAX_TIME_TICKS
	|| syn == SNMP_SYNTAX_GAUGE || syn == SNMP_SYNTAX_UINT32
	|| syn == SNMP_SYNTAX_OPAQUE ) {
		uint32_u tmp;
		snmp_value->size = 4;
		snmp_value->syntax = syn;
		tmp.uint32 = value;
		snmp_value->data[0] = tmp.data[3];
		snmp_value->data[1] = tmp.data[2];
		snmp_value->data[2] = tmp.data[1];
		snmp_value->data[3] = tmp.data[0];
		return SNMP_ERR_NO_ERROR;
		} else {
		clear(snmp_value);
		return SNMP_ERR_WRONG_TYPE;
	}
}
//
// encode's an ip-address byte array to ip-address, NSAP-address, opaque  syntax
SNMP_ERR_CODES encodeByteArrayToIpAddress(SNMP_VALUE *snmp_value, SNMP_SYNTAXES syn, const byte *value) {
	memset(snmp_value->data, 0, SNMP_MAX_VALUE_LEN);
	if ( syn == SNMP_SYNTAX_IP_ADDRESS || syn == SNMP_SYNTAX_NSAPADDR
	|| syn == SNMP_SYNTAX_OPAQUE ) {
		if ( sizeof(value) > 4 ) {
			clear(snmp_value);
			return SNMP_ERR_TOO_BIG;
			} else {
			snmp_value->size = 4;
			snmp_value->syntax = syn;
			snmp_value->data[0] = value[3];
			snmp_value->data[1] = value[2];
			snmp_value->data[2] = value[1];
			snmp_value->data[3] = value[0];
			return SNMP_ERR_NO_ERROR;
		}
		} else {
		clear(snmp_value);
		return SNMP_ERR_WRONG_TYPE;
	}
}
//
// encode's a boolean to boolean, opaque  syntax
SNMP_ERR_CODES encodeUint8ToBool(SNMP_VALUE *snmp_value, SNMP_SYNTAXES syn, const uint8_t value) {
	memset(snmp_value->data, 0, SNMP_MAX_VALUE_LEN);
	if ( syn == SNMP_SYNTAX_BOOL || syn == SNMP_SYNTAX_OPAQUE ) {
		snmp_value->size = 1;
		snmp_value->syntax = syn;
		if(value>0)
		{
			snmp_value->data[0] = 1;
		}else{
			snmp_value->data[0] = 0;
		}
		return SNMP_ERR_NO_ERROR;
		} else {
		clear(snmp_value);
		return SNMP_ERR_WRONG_TYPE;
	}
}
//
// encode's an uint64 to counter64, opaque  syntax
SNMP_ERR_CODES encodeUint64ToCounter64(SNMP_VALUE *snmp_value, SNMP_SYNTAXES syn, const uint64_t value) {
	memset(snmp_value->data, 0, SNMP_MAX_VALUE_LEN);
	if ( syn == SNMP_SYNTAX_COUNTER64 || syn == SNMP_SYNTAX_OPAQUE ) {
		uint64_u tmp;
		snmp_value->size = 8;
		snmp_value->syntax = syn;
		tmp.uint64 = value;
		snmp_value->data[0] = tmp.data[7];
		snmp_value->data[1] = tmp.data[6];
		snmp_value->data[2] = tmp.data[5];
		snmp_value->data[3] = tmp.data[4];
		snmp_value->data[4] = tmp.data[3];
		snmp_value->data[5] = tmp.data[2];
		snmp_value->data[6] = tmp.data[1];
		snmp_value->data[7] = tmp.data[0];
		return SNMP_ERR_NO_ERROR;
		} else {
		clear(snmp_value);
		return SNMP_ERR_WRONG_TYPE;
	}
}
//
// encode's a null to null, opaque  syntax
SNMP_ERR_CODES encodeNullToNull(SNMP_VALUE *snmp_value, SNMP_SYNTAXES syn) {
	clear(snmp_value);
	if ( syn == SNMP_SYNTAX_NULL || syn == SNMP_SYNTAX_OPAQUE ) {
		snmp_value->size = 0;
		snmp_value->syntax = syn;
		return SNMP_ERR_NO_ERROR;
		} else {
		return SNMP_ERR_WRONG_TYPE;
	}
}


typedef struct  {
	SNMP_PDU_TYPES type;
	int32_t version;
	int32_t requestId;
	SNMP_ERR_CODES error;
	int32_t errorIndex;
	SNMP_OID OID;
	SNMP_VALUE VALUE;
}SNMP_PDU;

uint8_t SNMP_VARS_Processing(char* oid, SNMP_PDU* pdu);
SNMP_API_STAT_CODES SNMP_begin()
{
	// set community names
	_getCommName = "public";
	_setCommName = "public";
	//
	// set community name set/get sizes
	_setSize = strlen(_setCommName);
	_getSize = strlen(_getCommName);
	//
	// init UDP socket
	//  Udp.begin(SNMP_DEFAULT_PORT);
	//
	return SNMP_API_STAT_SUCCESS;
}
char oid[SNMP_MAX_OID_LEN];     // will hold the OID of an SNMP request
SNMP_ERR_CODES status;

uint8_t destination_addr[]={10,0,0,51};
uint16_t destination_port=50000;

static char locDescr[]              = "ELAS FREECOOLING";  // read-only (static)
static char locObjectID[]           = "1.3.6.1.3.2009.0";                       // read-only (static)
uint32_t locUpTime           = 0;                                        // read-only (static)
static char locContact[20]          = "www.elas.com.ua";                            // should be stored/read from EEPROM - read/write (not done for simplicity)
static char locName[20]             = "ELAS OFFICE";                              // should be stored/read from EEPROM - read/write (not done for simplicity)
static char locLocation[20]         = "Fastiv, UKR";                        // should be stored/read from EEPROM - read/write (not done for simplicity)
static int32_t locServices          = 7;

const static char sysDescr[] PROGMEM      = "1.3.6.1.2.1.1.1.0";  // read-only  (DisplayString)
// .iso.org.dod.internet.mgmt.mib-2.system.sysObjectID (.1.3.6.1.2.1.1.2)
const static char sysObjectID[] PROGMEM   = "1.3.6.1.2.1.1.2.0";  // read-only  (ObjectIdentifier)
// .iso.org.dod.internet.mgmt.mib-2.system.sysUpTime (.1.3.6.1.2.1.1.3)
const static char sysUpTime[] PROGMEM     = "1.3.6.1.2.1.1.3.0";  // read-only  (TimeTicks)
// .iso.org.dod.internet.mgmt.mib-2.system.sysContact (.1.3.6.1.2.1.1.4)
const static char sysContact[] PROGMEM    = "1.3.6.1.2.1.1.4.0";  // read-write (DisplayString)
// .iso.org.dod.internet.mgmt.mib-2.system.sysName (.1.3.6.1.2.1.1.5)
const static char sysName[] PROGMEM       = "1.3.6.1.2.1.1.5.0";  // read-write (DisplayString)
// .iso.org.dod.internet.mgmt.mib-2.system.sysLocation (.1.3.6.1.2.1.1.6)
const static char sysLocation[] PROGMEM   = "1.3.6.1.2.1.1.6.0";  // read-write (DisplayString)
// .iso.org.dod.internet.mgmt.mib-2.system.sysServices (.1.3.6.1.2.1.1.7)
const static char sysServices[] PROGMEM   = "1.3.6.1.2.1.1.7.0";  // read-only  (Integer)

// custom OIDs
const static char AnalogPin0[] PROGMEM   = "1.3.6.1.3.2017.0.3.0";  // Analog pin 0 Level (0-1023)
const static char ledState[] PROGMEM     = "1.3.6.1.4.1.2017.11.1.0";  // Analog pin 0 Level (0-1023)
const static char Tfree2cond[] PROGMEM   = "1.3.6.1.4.1.2017.11.2.0";  // Analog pin 0 Level (0-1023)
const static char Tair[] PROGMEM         = "1.3.6.1.4.1.2017.11.3.0";  // Analog pin 0 Level (0-1023)
const static char Troom[] PROGMEM        = "1.3.6.1.4.1.2017.11.4.0";  // Analog pin 0 Level (0-1023)
const static char Tcond[] PROGMEM        = "1.3.6.1.4.1.2017.11.5.0";  // Analog pin 0 Level (0-1023)

uint8_t data[300];
uint16_t _size=4;
uint32_t UtoS_Timer=0;
uint16_t SerialVal1, SerialVal2;

typedef enum{Byte,Word,Text,Long_Word,Long_Int}varTypes;
typedef struct {
	const char* oid;
	SNMP_SYNTAXES syn;
	void * variable;
	uint8_t variableType;
	uint8_t AccessLevel;
}SNMP_FIELD;

enum{READ, READWRITE};

#ifdef MODBUS
SNMP_FIELD SNMP_VARS[] = {
	{"1.3.6.1.2.1.1.1.0",		 SNMP_SYNTAX_OCTETS,	locDescr,			Text,		READ},
	{"1.3.6.1.2.1.1.2.0",		 SNMP_SYNTAX_OCTETS,	locObjectID,		Text,		READ},
	{"1.3.6.1.2.1.1.3.0",		 SNMP_SYNTAX_TIME_TICKS,&locUpTime,			Long_Word,	READ},
	{"1.3.6.1.2.1.1.4.0",		 SNMP_SYNTAX_OCTETS,	locContact,			Text,		READWRITE},
	{"1.3.6.1.2.1.1.5.0",		 SNMP_SYNTAX_OCTETS,	locName,			Text,		READWRITE},
	{"1.3.6.1.2.1.1.6.0",		 SNMP_SYNTAX_OCTETS,	locLocation,		Text,		READWRITE},
	{"1.3.6.1.2.1.1.7.0",		 SNMP_SYNTAX_INT,		&locServices,		Long_Int,	READ},
	{"1.3.6.1.4.1.2017.12.1.0",  SNMP_SYNTAX_INT32,		&MB_HoldReg[0],		Word,		READWRITE},
	{"1.3.6.1.4.1.2017.12.2.0",  SNMP_SYNTAX_INT32,		&MB_HoldReg[1],		Word,		READWRITE},
	{"1.3.6.1.4.1.2017.12.3.0",  SNMP_SYNTAX_INT32,		&MB_HoldReg[2],		Word,		READWRITE},
	{"1.3.6.1.4.1.2017.12.4.0",  SNMP_SYNTAX_INT32,		&MB_HoldReg[3],		Word,		READWRITE},
	{"1.3.6.1.4.1.2017.12.5.0",  SNMP_SYNTAX_INT32,		&MB_InReg[0],		Word,		READ},
	{"1.3.6.1.4.1.2017.12.6.0",  SNMP_SYNTAX_INT32,		&MB_InReg[1],		Word,		READ},
	{"1.3.6.1.4.1.2017.12.7.0",  SNMP_SYNTAX_INT32,		&MB_InReg[2],		Word,		READ},
	{"1.3.6.1.4.1.2017.12.8.0",  SNMP_SYNTAX_INT32,		&MB_InReg[3],		Word,		READ},
	{"1.3.6.1.4.1.2017.12.9.0",  SNMP_SYNTAX_INT32,		&MB_InReg[4],		Word,		READ},
	{"1.3.6.1.4.1.2017.12.10.0", SNMP_SYNTAX_INT32,		&MB_InReg[5],		Word,		READ},
	{"1.3.6.1.4.1.2017.12.11.0", SNMP_SYNTAX_INT32,		&MB_InReg[6],		Word,		READ},
	{"1.3.6.1.4.1.2017.12.12.0", SNMP_SYNTAX_INT32,		&MB_InReg[7],		Word,		READ},
	{"1.3.6.1.4.1.2017.13.1.0",  SNMP_SYNTAX_INT32,		&MB_Coil_Vars[0],   Word,		READWRITE},
	{"1.3.6.1.4.1.2017.13.2.0",	 SNMP_SYNTAX_INT32,		&MB_Coil_Vars[1],   Word,		READWRITE},
	{"1.3.6.1.4.1.2017.13.3.0",	 SNMP_SYNTAX_INT32,		&MB_Coil_Vars[2],   Word,		READWRITE},
	{"1.3.6.1.4.1.2017.13.4.0",	 SNMP_SYNTAX_INT32,		&MB_Coil_Vars[3],   Word,		READWRITE},
	{"1.3.6.1.4.1.2017.13.5.0",  SNMP_SYNTAX_INT32,		&MB_Coil_Vars[4],   Word,		READWRITE},
	{"1.3.6.1.4.1.2017.13.6.0",  SNMP_SYNTAX_INT32,		&MB_Coil_Vars[5],   Word,		READWRITE},
	{"1.3.6.1.4.1.2017.13.7.0",  SNMP_SYNTAX_INT32,		&MB_Coil_Vars[6],   Word,		READWRITE},
	{"1.3.6.1.4.1.2017.13.8.0",  SNMP_SYNTAX_INT32,		&MB_Coil_Vars[7],   Word,		READWRITE},
	{"1.3.6.1.4.1.2017.13.9.0",  SNMP_SYNTAX_INT32,		&MB_Input_Vars[0],  Word,		READ},
	{"1.3.6.1.4.1.2017.13.10.0", SNMP_SYNTAX_INT32,		&MB_Input_Vars[1],  Word,		READ},
	{"1.3.6.1.4.1.2017.13.11.0", SNMP_SYNTAX_INT32,		&MB_Input_Vars[2],  Word,		READ},
	{"1.3.6.1.4.1.2017.13.12.0", SNMP_SYNTAX_INT32,		&MB_Input_Vars[3],  Word,		READ},
	{"1.3.6.1.4.1.2017.13.13.0", SNMP_SYNTAX_INT32,		&MB_Input_Vars[4],  Word,		READ},
	{"1.3.6.1.4.1.2017.13.14.0", SNMP_SYNTAX_INT32,		&MB_Input_Vars[5],  Word,		READ},
	{"1.3.6.1.4.1.2017.13.15.0", SNMP_SYNTAX_INT32,		&MB_Input_Vars[6],  Word,		READ},
	{"1.3.6.1.4.1.2017.13.16.0", SNMP_SYNTAX_INT32,		&MB_Input_Vars[7],  Word,		READ},
};
#endif
unsigned int GetOIDCount(const char* str){
	unsigned int i, count=0;
	for(i=0;i<strlen(str);i++){
		if(str[i] == '.')count++;
	}
	return ++count;
};
unsigned int GetNVal_FromOID_String(unsigned int n, const char* str){
	char substr[6];
	unsigned int res;
	
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int k = 0;
	while(((str[i]!='.')||(j<n))&&(i<strlen(str))){
		if(j == n-1){
			if(str[i]!='.'){
				substr[k]=str[i];
			k++;}
		}
		if(str[i+1]=='.'){j++;};
		i++;
	}
	substr[++k] = '\0';
	sscanf(substr,"%u",&res);
	return res;
	
}
int CompareOIDS(const char* str1,const char* str2){
	if(GetOIDCount(str1)>GetOIDCount(str2)){
		for(int i = 1;i<GetOIDCount(str2)+1;i++){
			if(GetNVal_FromOID_String(i,str1)>GetNVal_FromOID_String(i,str2))return 1;
			if(GetNVal_FromOID_String(i,str1)<GetNVal_FromOID_String(i,str2))return -1;
		}
		return 1;
	}
	if(GetOIDCount(str1)<GetOIDCount(str2)){
		for(int i = 1;i<GetOIDCount(str1)+1;i++){
			if(GetNVal_FromOID_String(i,str1)>GetNVal_FromOID_String(i,str2))return 1;
			if(GetNVal_FromOID_String(i,str1)<GetNVal_FromOID_String(i,str2))return -1;
		}
		return -1;
	}
	if(GetOIDCount(str1)==GetOIDCount(str2)){
		for(int i = 1;i<GetOIDCount(str1)+1;i++){
			if(GetNVal_FromOID_String(i,str1)>GetNVal_FromOID_String(i,str2))return 1;
			if(GetNVal_FromOID_String(i,str1)<GetNVal_FromOID_String(i,str2))return -1;
		}
		return -1;
	}
	return -1;
}
int GetNextOid(const char* str){
	#ifdef MODBUS
	for(int i = 0;i<sizeof(SNMP_VARS)/sizeof(SNMP_FIELD);i++){
		if(CompareOIDS(str, SNMP_VARS[i].oid)==-1){
			return i;
		}
	}
	#endif
								
	return -1;
}

SNMP_ERR_CODES Encode(void* C_value, uint8_t C_type, SNMP_VALUE *snmp_value, SNMP_SYNTAXES syn ){
	SNMP_ERR_CODES status = 0;
	if((syn == SNMP_SYNTAX_INT)&&(C_type == Word)){
		status = encodeInt16ToInt(snmp_value, SNMP_SYNTAX_INT, *(int16_t *)C_value);
		return status;
	}
	if((syn == SNMP_SYNTAX_OCTETS)&&(C_type == Text)){
		status = encodeStringToOctetString(snmp_value, SNMP_SYNTAX_OCTETS, (const char*)C_value);
		return status;
	}
	if((syn == SNMP_SYNTAX_TIME_TICKS)&&(C_type == Long_Word)){
		status = encodeUint32ToUint32(snmp_value, SNMP_SYNTAX_TIME_TICKS, *(uint32_t *)C_value);
		return status;
	}
	if((syn == SNMP_SYNTAX_INT)&&(C_type == Long_Int)){
		status = encodeInt32ToInt32(snmp_value, SNMP_SYNTAX_INT32, *(uint32_t*)C_value);
		return status;
	}
	return status;
};

SNMP_ERR_CODES Decode(SNMP_VALUE *snmp_value, SNMP_SYNTAXES syn, void* C_value, uint8_t C_type){
	SNMP_ERR_CODES status = 0;
	if((C_type == Text)&&(syn == SNMP_SYNTAX_OCTETS)){
		status = decodeOctetStringToString(snmp_value, ( char*)locContact, strlen((const char*)locContact));
		return status;
	}
	if((C_type == Word)&&(syn == SNMP_SYNTAX_INT)){
		status = decodeInt16toHoldReg(snmp_value, (int16_t *)C_value);
		return status;
	}
	if((syn == SNMP_SYNTAX_TIME_TICKS)&&(C_type == Long_Word)){

		return status;
	}
	if((syn == SNMP_SYNTAX_INT)&&(C_type == Long_Int)){

		return status;
	}
	return status;

};
SNMP_API_STAT_CODES SNMP_requestPdu(SNMP_PDU *pdu)
{
	//char *community;

	// sequence length
	//byte seqLen= 0;
	// version
	byte verLen, verEnd;
	// community string
	byte comLen, comEnd;
	// pdu
	byte pduTyp;
	//byte pduLen;
	byte ridLen, ridEnd;
	byte errLen, errEnd;
	byte eriLen, eriEnd;
	//byte vblTyp;
	byte vblLen, vblEnd;
	//byte vbiTyp;
	byte vbiLen;
	byte obiLen, obiEnd;
	byte valTyp, valLen;
//	byte valEnd;
	byte i;
	//
	// set packet packet size (skip UDP header)
	_packetSize = _SNMPUdpSocket_available();


	//
	// reset packet array
	memset(_packet, 0, SNMP_MAX_PACKET_LEN);
	//
	// validate packet
	if ( _packetSize != 0 && _packetSize > SNMP_MAX_PACKET_LEN ) {
		//
		//SNMP_FREE(_packet);

		return SNMP_API_STAT_PACKET_TOO_BIG;
	}
	//
	// get UDP packet
	//Udp.parsePacket();
	_SNMPSocket_read(_packet, _packetSize);

	//  Udp.readPacket(_packet, _packetSize, _dstIp, &_dstPort);
	//
	// packet check 1
	if ( _packet[0] != 0x30 ) {
		//
		//SNMP_FREE(_packet);

		return SNMP_API_STAT_PACKET_INVALID;
	}
	//
	// sequence length
	//seqLen = _packet[1];
	// version
	verLen = _packet[3];
	verEnd = 3 + verLen;
	// community string
	comLen = _packet[verEnd + 2];
	comEnd = verEnd + 2 + comLen;
	// pdu
	pduTyp = _packet[comEnd + 1];
	//pduLen = _packet[comEnd + 2];
	ridLen = _packet[comEnd + 4];
	ridEnd = comEnd + 4 + ridLen;
	errLen = _packet[ridEnd + 2];
	errEnd = ridEnd + 2 + errLen;
	eriLen = _packet[errEnd + 2];
	eriEnd = errEnd + 2 + eriLen;
	//vblTyp = _packet[eriEnd + 1];
	vblLen = _packet[eriEnd + 2];
	vblEnd = eriEnd + 2;

		// extract reqiest-id 0x00 0x00 0x00 0x01 (4-byte int aka int32)
		pdu->requestId = 0;
		for ( i = 0; i < ridLen; i++ ) {
			pdu->requestId = (pdu->requestId << 8) | _packet[comEnd + 5 + i];
		}
  obiLen = _packet[eriEnd + 6];
  obiEnd = eriEnd + obiLen + 6;
  valTyp = _packet[obiEnd + 1];
  valLen = _packet[obiEnd + 2];
	pdu->type = (SNMP_PDU_TYPES)pduTyp;
	if(pdu->type!=SNMP_PDU_GET_BULK){
		//vbiTyp = _packet[eriEnd + 3];
		vbiLen = _packet[eriEnd + 4];
		obiLen = _packet[eriEnd + 6];
		obiEnd = eriEnd + obiLen + 6;
		valTyp = _packet[obiEnd + 1];
		valLen = _packet[obiEnd + 2];
		//valEnd = obiEnd + 2 + valLen;
		}
	else{
		uint16_t SumLenOfVars = 0;
		//int i=0;
		BulkValuesCount = 0;
		while(SumLenOfVars<vblLen){
			vbiLen = _packet[vblEnd + 2 + SumLenOfVars];

			SNMP_OID oid;
			oid.size = _packet[vblEnd + 4 + SumLenOfVars];
			for(int i=0;i<oid.size;i++){
				oid.data[i] = _packet[vblEnd + 5 + SumLenOfVars + i];		
			}
			char buf[30];
			SNMP_OID_toString(oid, &buf[0]);
			uint8_t a = GetNextOid(&buf[0]);
			if(a>=0){
				BulkValuesIndexes[BulkValuesCount++] = a;				
			}
			SumLenOfVars += 2 + vbiLen;
		}

	}
	//
	// extract version

	pdu->version = 0;
	for ( i = 0; i < verLen; i++ ) {
		pdu->version = (pdu->version << 8) | _packet[5 + i];
	}
	//
	// validate version
	//
	// pdu-type
	pdu->type = (SNMP_PDU_TYPES)pduTyp;
	_dstType = pdu->type;
	//
	// validate community size
	if ( comLen > SNMP_MAX_NAME_LEN ) {
		// set pdu error
		pdu->error = SNMP_ERR_TOO_BIG;
		//
		return SNMP_API_STAT_NAME_TOO_BIG;
	}
	//
	//
	// validate community name
	if(pdu->type == SNMP_PDU_GET_NEXT){
		/*SerialPrint("Type: ");
		SerialPrintln(pdu->type);
		Serial.print("_setSize =  ");
		Serial.println(_setSize);
		Serial.print("comLen =  ");
		Serial.println(comLen);*/
	}
	if ( pdu->type == SNMP_PDU_SET && comLen == _setSize ) {
		//
		for ( i = 0; i < _setSize; i++ ) {
			if( _packet[verEnd + 3 + i] != (byte)_setCommName[i] ) {

				// set pdu error
				pdu->error = SNMP_ERR_NO_SUCH_NAME;
				//
				return SNMP_API_STAT_NO_SUCH_NAME;
			}
		}
		} else if ( (pdu->type == SNMP_PDU_GET && comLen == _getSize)|| (pdu->type == SNMP_PDU_GET_NEXT && comLen == _getSize)) {
		//
		for ( i = 0; i < _getSize; i++ ) {
											if( _packet[verEnd + 3 + i] != (byte)_getCommName[i] ) {
												// set pdu error
												pdu->error = SNMP_ERR_NO_SUCH_NAME;
												//
												return SNMP_API_STAT_NO_SUCH_NAME;
											}
										}
		} else {
		pdu->VALUE.size = 0;
		pdu->OID.size = 0;
		// set pdu error
		pdu->error = SNMP_ERR_NO_SUCH_NAME;
		//
		return SNMP_API_STAT_NO_SUCH_NAME;
	}

/*	for(int i=0;i<_packetSize;i++)
	{
		Serial.print(_packet[i]);
		Serial.print(", ");
		Serial.println();
	}*/
	//
	//

	//
	// extract error
	pdu->error = SNMP_ERR_NO_ERROR;
	int32_t err = 0;
	for ( i = 0; i < errLen; i++ ) {
		err = (err << 8) | _packet[ridEnd + 3 + i];
	}
	pdu->error = (SNMP_ERR_CODES)err;
	//
	// extract error-index
	pdu->errorIndex = 0;
	for ( i = 0; i < eriLen; i++ ) {
		pdu->errorIndex = (pdu->errorIndex << 8) | _packet[errEnd + 3 + i];
	}
	//
	//
	// validate object-identifier size
	if ( obiLen > SNMP_MAX_OID_LEN ) {
		// set pdu error
		pdu->error = SNMP_ERR_TOO_BIG;

		return SNMP_API_STAT_OID_TOO_BIG;
	}
	//
	// extract and contruct object-identifier
	memset(pdu->OID.data, 0, SNMP_MAX_OID_LEN);
	pdu->OID.size = obiLen;
	for ( i = 0; i < obiLen; i++ ) {
		pdu->OID.data[i] = _packet[eriEnd + 7 + i];
	}
	//
	// value-type
	pdu->VALUE.syntax = (SNMP_SYNTAXES)valTyp;
	//
	// validate value size
	if ( obiLen > SNMP_MAX_VALUE_LEN ) {
		// set pdu error
		pdu->error = SNMP_ERR_TOO_BIG;

		return SNMP_API_STAT_VALUE_TOO_BIG;
	}
	//
	// value-size
	pdu->VALUE.size = valLen;
	//
	// extract value
	memset(pdu->VALUE.data, 0, SNMP_MAX_VALUE_LEN);
	for ( i = 0; i < valLen; i++ ) {
		pdu->VALUE.data[i] = _packet[obiEnd + 3 + i];
	}
	//
	return SNMP_API_STAT_SUCCESS;
}


SNMP_API_STAT_CODES SNMP_responsePdu(SNMP_PDU *pdu)
{
	int32_u u;
	byte i;
	//
	// Length of entire SNMP packet
	_packetPos = 0;  // 23
	_packetSize = 25 + sizeof(pdu->requestId) + sizeof((int32_t)pdu->error) + sizeof(pdu->errorIndex) + pdu->OID.size + pdu->VALUE.size;
	//				
	memset(_packet, 0, SNMP_MAX_PACKET_LEN);
	//
	if ( _dstType == SNMP_PDU_SET ) {
		_packetSize += _setSize;
		} else {
		_packetSize += _getSize;
	}
	//
	_packet[_packetPos++] = (byte)SNMP_SYNTAX_SEQUENCE; // type  25+4+4+4+8+16+6-2
	_packet[_packetPos++] = (byte)_packetSize - 2;    // length
	//
	// SNMP version
	_packet[_packetPos++] = (byte)SNMP_SYNTAX_INT;  // type
	_packet[_packetPos++] = 0x01;     // length
	_packet[_packetPos++] = 0x00;     // value
	//
	// SNMP community string
	_packet[_packetPos++] = (byte)SNMP_SYNTAX_OCTETS; // type
	if ( _dstType == SNMP_PDU_SET ) {
		_packet[_packetPos++] = (byte)_setSize; // length
		for ( i = 0; i < _setSize; i++ ) {
			_packet[_packetPos++] = (byte)_setCommName[i];
		}
		} else {
		_packet[_packetPos++] = (byte)_getSize; // length
		for ( i = 0; i < _getSize; i++ ) {
			_packet[_packetPos++] = (byte)_getCommName[i];
		}
	}
	//
	// SNMP PDU
	_packet[_packetPos++] = (byte)pdu->type;
	_packet[_packetPos++] = (byte)( sizeof(pdu->requestId) + sizeof((int32_t)pdu->error) + sizeof(pdu->errorIndex) + pdu->OID.size + pdu->VALUE.size + 14 );
	//
	// Request ID (size always 4 e.g. 4-byte int)
	_packet[_packetPos++] = (byte)SNMP_SYNTAX_INT;  // type
	_packet[_packetPos++] = (byte)sizeof(pdu->requestId);
	u.int32 = pdu->requestId;
	_packet[_packetPos++] = u.data[3];
	_packet[_packetPos++] = u.data[2];
	_packet[_packetPos++] = u.data[1];
	_packet[_packetPos++] = u.data[0];
	//
	// Error (size always 4 e.g. 4-byte int)
	_packet[_packetPos++] = (byte)SNMP_SYNTAX_INT;  // type
	_packet[_packetPos++] = (byte)sizeof((int32_t)pdu->error);
	u.int32 = pdu->error;
	_packet[_packetPos++] = u.data[3];
	_packet[_packetPos++] = u.data[2];
	_packet[_packetPos++] = u.data[1];
	_packet[_packetPos++] = u.data[0];
	//
	// Error Index (size always 4 e.g. 4-byte int)
	_packet[_packetPos++] = (byte)SNMP_SYNTAX_INT;  // type
	_packet[_packetPos++] = (byte)sizeof(pdu->errorIndex);
	u.int32 = pdu->errorIndex;
	_packet[_packetPos++] = u.data[3];
	_packet[_packetPos++] = u.data[2];
	_packet[_packetPos++] = u.data[1];
	_packet[_packetPos++] = u.data[0];
	//
	// Varbind List
	_packet[_packetPos++] = (byte)SNMP_SYNTAX_SEQUENCE; // type
	_packet[_packetPos++] = (byte)( pdu->OID.size + pdu->VALUE.size + 6 ); //4
	//
	// Varbind
	_packet[_packetPos++] = (byte)SNMP_SYNTAX_SEQUENCE; // type
	_packet[_packetPos++] = (byte)( pdu->OID.size + pdu->VALUE.size + 4 ); //2
	//
	// ObjectIdentifier
	_packet[_packetPos++] = (byte)SNMP_SYNTAX_OID;  // type
	_packet[_packetPos++] = (byte)(pdu->OID.size);
	for ( i = 0; i < pdu->OID.size; i++ ) {
		_packet[_packetPos++] = pdu->OID.data[i];
	}
	//
	// Value
	_packet[_packetPos++] = (byte)pdu->VALUE.syntax;  // type
	_packet[_packetPos++] = (byte)(pdu->VALUE.size);
	for ( i = 0; i < pdu->VALUE.size; i++ ) {
		_packet[_packetPos++] = pdu->VALUE.data[i];
	}
	//

/*	Serial.print("Remote IP:");Serial.print(_remoteIP[0]);Serial.print(", ");  Serial.print(_remoteIP[1]);Serial.print(", ");  Serial.print(_remoteIP[2]);Serial.print(", ");  Serial.print(_remoteIP[3]);Serial.println(";");
	Serial.print("Remote PORT:");Serial.print(_snmpUdpSocket_remotePort());*/
	//LED_On();
	_UDPtoSerialSocket_beginPacket(_remoteIP, _snmpUdpSocket_remotePort());
	_UDPtoSerialSocket_write(_packet, _packetSize);
	///  Udp.endPacket();
	//  Udp.write(_packet, _packetSize, _dstIp, _dstPort);
	//
	return SNMP_API_STAT_SUCCESS;
}


SNMP_API_STAT_CODES SNMP_responsePduGetBulk(SNMP_PDU *pdu)
{
		pdu->error = SNMP_ERR_NO_ERROR;
		pdu->errorIndex = 0;
	int32_u u;
	uint16_t varssize = 0;
	byte i;
	//
	// Length of entire SNMP packet
	_packetPos = 0;  // 23
	_packetSize = 19 + sizeof(pdu->requestId) + sizeof((int32_t)pdu->error) + sizeof(pdu->errorIndex);
	//		
	for(uint8_t i=0;i<BulkValuesCount;i++){
	#ifdef MODBUS
		SNMP_PDU pdu1;
		Encode(SNMP_VARS[BulkValuesIndexes[i]].variable, SNMP_VARS[BulkValuesIndexes[i]].variableType, &(pdu1.VALUE), SNMP_VARS[BulkValuesIndexes[i]].syn);
		SNMP_OID_fromString(&pdu1.OID, SNMP_VARS[BulkValuesIndexes[i]].oid);		
		varssize += ((byte)(pdu1.OID.size) + (byte)(pdu1.VALUE.size) + 6);
		//varssize += pdu->VALUE.size ;
		#endif
	}
	_packetSize+=varssize;

	memset(_packet, 0, SNMP_MAX_PACKET_LEN);
	//
	if ( _dstType == SNMP_PDU_SET ) {
		_packetSize += _setSize;
		} else {
		_packetSize += _getSize;
	}
	//
	_packet[_packetPos++] = (byte)SNMP_SYNTAX_SEQUENCE; // type  25+4+4+4+8+16+6-2
	_packet[_packetPos++] = 0x82;
	_packet[_packetPos++] = highByte(_packetSize - 4);
	_packet[_packetPos++] = lowByte(_packetSize - 4);
	//_packet[_packetPos++] = (byte)_packetSize - 2;    // length
	
	//
	// SNMP version
	_packet[_packetPos++] = (byte)SNMP_SYNTAX_INT;  // type
	_packet[_packetPos++] = 0x01;     // length
	_packet[_packetPos++] = 0x00;     // value
	//
	// SNMP community string
	_packet[_packetPos++] = (byte)SNMP_SYNTAX_OCTETS; // type
	if ( _dstType == SNMP_PDU_SET ) {
		_packet[_packetPos++] = (byte)_setSize; // length
		for ( i = 0; i < _setSize; i++ ) {
			_packet[_packetPos++] = (byte)_setCommName[i];
		}
		} else {
		_packet[_packetPos++] = (byte)_getSize; // length
		for ( i = 0; i < _getSize; i++ ) {
			_packet[_packetPos++] = (byte)_getCommName[i];
		}
	}
	//
	// SNMP PDU
	pdu->type = SNMP_PDU_RESPONSE;
	_packet[_packetPos++] = (byte)pdu->type;
	_packet[_packetPos++] = (byte)( sizeof(pdu->requestId) + sizeof((int32_t)pdu->error) + sizeof(pdu->errorIndex) + varssize + 8 );
	//
	// Request ID (size always 4 e.g. 4-byte int)
	_packet[_packetPos++] = (byte)SNMP_SYNTAX_INT;  // type
	_packet[_packetPos++] = (byte)sizeof(pdu->requestId);
	u.int32 = pdu->requestId;
	_packet[_packetPos++] = u.data[3];
	_packet[_packetPos++] = u.data[2];
	_packet[_packetPos++] = u.data[1];
	_packet[_packetPos++] = u.data[0];
	//
	// Error (size always 4 e.g. 4-byte int)
	_packet[_packetPos++] = (byte)SNMP_SYNTAX_INT;  // type
	_packet[_packetPos++] = (byte)sizeof((int32_t)pdu->error);
	u.int32 = pdu->error;
	_packet[_packetPos++] = u.data[3];
	_packet[_packetPos++] = u.data[2];
	_packet[_packetPos++] = u.data[1];
	_packet[_packetPos++] = u.data[0];
	//
	// Error Index (size always 4 e.g. 4-byte int)
	_packet[_packetPos++] = (byte)SNMP_SYNTAX_INT;  // type
	_packet[_packetPos++] = (byte)sizeof(pdu->errorIndex);
	u.int32 = pdu->errorIndex;
	_packet[_packetPos++] = u.data[3];
	_packet[_packetPos++] = u.data[2];
	_packet[_packetPos++] = u.data[1];
	_packet[_packetPos++] = u.data[0];
	//
	// Varbind List
	_packet[_packetPos++] = (byte)SNMP_SYNTAX_SEQUENCE; // type
	_packet[_packetPos++] = (byte)( varssize ); //4


		for(uint8_t i=0;i<BulkValuesCount;i++){
			//Encode(SNMP_VARS[BulkValuesIndexes[i]].variable, SNMP_VARS[BulkValuesIndexes[i]].variableType, &pdu->VALUE, SNMP_VARS[BulkValuesIndexes[i]].syn);
			//SNMP_OID_fromString(&pdu->OID, SNMP_VARS[BulkValuesIndexes[i]].oid);

			SNMP_PDU pdu1;
			#ifdef MODBUS
			Encode(SNMP_VARS[BulkValuesIndexes[i]].variable, SNMP_VARS[BulkValuesIndexes[i]].variableType, &(pdu1.VALUE), SNMP_VARS[BulkValuesIndexes[i]].syn);

			SNMP_OID_fromString(&pdu1.OID, SNMP_VARS[BulkValuesIndexes[i]].oid);
					#endif
			//
			// Varbind
			_packet[_packetPos++] = (byte)SNMP_SYNTAX_SEQUENCE; // type
			_packet[_packetPos++] = (byte)( pdu1.OID.size + pdu1.VALUE.size + 4 ); //2
			//
			// ObjectIdentifier
			_packet[_packetPos++] = (byte)SNMP_SYNTAX_OID;  // type
			_packet[_packetPos++] = (byte)(pdu1.OID.size);
			for (int j = 0; j < pdu1.OID.size; j++ ) {
				_packet[_packetPos++] = pdu1.OID.data[j];
			}
			//
			// Value
			_packet[_packetPos++] = (byte)pdu1.VALUE.syntax;  // type
			_packet[_packetPos++] = (byte)(pdu1.VALUE.size);
			for (int m = 0; m < pdu1.VALUE.size; m++ ) {
				_packet[_packetPos++] = pdu1.VALUE.data[m];
			}
			//
	}
/*	Serial.print("Remote IP:");Serial.print(_remoteIP[0]);Serial.print(", ");  Serial.print(_remoteIP[1]);Serial.print(", ");  Serial.print(_remoteIP[2]);Serial.print(", ");  Serial.print(_remoteIP[3]);Serial.println(";");
	Serial.print("Remote PORT:");Serial.print(_snmpUdpSocket_remotePort());*/
	//LED_On();
	_UDPtoSerialSocket_beginPacket(_remoteIP, _snmpUdpSocket_remotePort());
	_UDPtoSerialSocket_write(_packet, _packetSize);
	///  Udp.endPacket();
	//  Udp.write(_packet, _packetSize, _dstIp, _dstPort);
	//
	return SNMP_API_STAT_SUCCESS;
}

void pduReceived()  // is being called when an SNMP packet has been received
{
	SNMP_PDU pdu;


	api_status = SNMP_requestPdu(&pdu);

	// check if valid packet
	if ( ((pdu.type == SNMP_PDU_GET) || (pdu.type == SNMP_PDU_GET_NEXT) || (pdu.type == SNMP_PDU_SET))
	&& pdu.error == SNMP_ERR_NO_ERROR && api_status == SNMP_API_STAT_SUCCESS )
	{
		/*Serial.println("TYPES: ");
		Serial.println(SNMP_PDU_GET);
		Serial.println(SNMP_PDU_GET_NEXT);
		Serial.println(SNMP_PDU_SET);*/
		//
		//char tmpOIDfs[SNMP_MAX_OID_LEN];
		SNMP_OID_toString(pdu.OID, oid);

		#ifdef MODBUS
		
		/*else*/ if(SNMP_VARS_Processing( oid,  &pdu) == 1){

		}
		else 
		#endif
		 if( pdu.type == SNMP_PDU_GET_NEXT ){
			int a = GetNextOid(oid);				
			if(a>=0){	
					#ifdef MODBUS
					
					SNMP_OID_fromString(&(pdu.OID), SNMP_VARS[a].oid);
					status = Encode(SNMP_VARS[a].variable, SNMP_VARS[a].variableType, &pdu.VALUE,SNMP_VARS[a].syn);
						#endif
					pdu.type = SNMP_PDU_RESPONSE;
					pdu.error = status;


					}else{
						pdu.type = SNMP_PDU_RESPONSE;
						pdu.error = SNMP_ERR_NO_SUCH_NAME;
					}
			}
	
			else
			{
				// oid does not exist
				//
				// response packet - object not found
				//pdu.VALUE.encode(SNMP_SYNTAX_NULL);
				pdu.type = SNMP_PDU_RESPONSE;
				pdu.error = SNMP_ERR_NO_SUCH_NAME;

			}
		
		//
		SNMP_responsePdu(&pdu);
	
		count++;
	}else if(pdu.type == SNMP_PDU_GET_BULK){

	SNMP_responsePduGetBulk(&pdu);
	}
	
	else
	// packet not valid, send GENERAL_ERROR response. Required, otherwise the invalid packet
	// will get stuck in the buffer and processed over and over again
	
	{
		/*Serial.println("Unknown Packet!!");
		
		Serial.print("PDU Type: ");
		Serial.print(pdu.type);
		Serial.print(" PDU Error: ");
		Serial.print(pdu.error);
		Serial.print(" API status: ");
		Serial.println(api_status);*/
		//resetEthernet();
		//return;
		pdu.type = SNMP_PDU_RESPONSE;
		pdu.error = SNMP_ERR_GEN_ERROR;
		SNMP_responsePdu(&pdu);
		/*Serial.println("Sent 'GENERAL_ERROR' response");*/
	}

	//
	// Serial.print("freeing PDU..");
	// Serial.print(" RAM:");
	// freeMemory() << endl;
	// Agentuino.freePdu(&pdu);
	//
	// Serial << "UDP Packet Received End.." << " RAM:" << freeMemory() << endl;
}
extern void resetEthernet(void);

uint8_t SNMP_Automat(uint8_t event)
{
	static uint8_t state=0;
	uint8_t stateOld=state;
	#ifdef SNMP_Automat_LOGGING
		SerialPrint("  SNMP_Automat_start_state:") ;
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
			if(UDPbegin1_Automat(1, SNMP_DEFAULT_PORT)!=255)
			{
				break;
			}
			state=2;
		break;

		case 2:
			if(readSnSR(1)!=0x22){
				#ifdef SNMP_Automat_LOGGING
					SerialPrintln("RESET FROM SNMP!");
				#endif
				resetEthernet();
			}
			if(_UDPtoSerialSocket_parsePacket())
			{
				if(_remaining[1]){
					pduReceived();
					state=3;
				}
			}
		break;

		case 3:
		if(_UDPtoSerialSocket_endPacket_Automat(1)!=255){
			break;
		}
		state = 2;
		break;
		case 4:

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
				UDPbegin1_Automat(0, SNMP_DEFAULT_PORT);
				// set community names
				_getCommName = "public";
				_setCommName = "public";
				//
				// set community name set/get sizes
				_setSize = strlen(_setCommName);
				_getSize = strlen(_getCommName);
			break;
			
			case 2:
			
			break;

			case 3:
				_UDPtoSerialSocket_endPacket_Automat(0);
			break;
			
			case 255:
			break;
		}
	}
	#ifdef SNMP_Automat_LOGGING
	SerialPrint("  SNMP_Automat_End_state:") ;
	SerialPrintUint8_t(state) ;
	SerialPrintEndl();
	#endif
	return state;
}

extern void resetSNMP(void){
	SNMP_Automat(0);

};

#ifdef MODBUS


uint8_t SNMP_VARS_Processing(char* oid, SNMP_PDU* pdu){
	for(uint8_t i = 0; i<sizeof(SNMP_VARS)/sizeof(SNMP_FIELD); i++){
				if(strcmp(oid, SNMP_VARS[i].oid) == 0){
					if( pdu->type == SNMP_PDU_SET)
					{
						if(SNMP_VARS[i].AccessLevel == READ){
							pdu->type = SNMP_PDU_RESPONSE;
							pdu->error = SNMP_ERR_READ_ONLY;
						}
						else{
							status = Decode(&pdu->VALUE,SNMP_VARS[i].syn,SNMP_VARS[i].variable, SNMP_VARS[i].variableType);
							pdu->type = SNMP_PDU_RESPONSE;
							pdu->error = status;
						}
					}
					else if(pdu->type == SNMP_PDU_GET){
						status = Encode(SNMP_VARS[i].variable, SNMP_VARS[i].variableType, &pdu->VALUE,SNMP_VARS[i].syn);
						pdu->type = SNMP_PDU_RESPONSE;
						pdu->error = status;
					}
					else if( pdu->type == SNMP_PDU_GET_NEXT ){
						if(i<(sizeof(SNMP_VARS)/sizeof(SNMP_FIELD) - 1)){
							SNMP_OID_fromString(&(pdu->OID), SNMP_VARS[i+1].oid);
							status = Encode(SNMP_VARS[i+1].variable, SNMP_VARS[i+1].variableType, &pdu->VALUE,SNMP_VARS[i+1].syn);
							pdu->type = SNMP_PDU_RESPONSE;
							pdu->error = status;
							}else{
							pdu->type = SNMP_PDU_RESPONSE;
							pdu->error = SNMP_ERR_NO_SUCH_NAME;
						}
					}
					return 1;
				}
		}
		return 0;
	}



#endif
#endif /* SNMP_AUTOMAT_H_ */