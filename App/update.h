#ifndef _update_h
#define _update_h
#include "stm32f1xx.h"
#define 	VERSION							0x01
#define 	Edition_type					0x33
#define     CMD_SIZE           				(1024)      //max cmd length

#define 	FLASH_UPDATEFLAG_ADDR   		0x08005c00//upgrade flag flash store address 23KµÄÎ»ÖÃ
#define 	Flash_APP_ADDR  				0x08006000   //24K
#define 	UPGRADE_LENGTH_PER_SECTION      960  //upgrade length per section
#define 	UPGRADE_LENGTH_FIRST_SECTION    16   //upgrade first section length

typedef struct
{
	uint8_t  type;     //board type
	uint8_t  ver;      //upgrade version
	uint16_t crc16;    //crc16
	uint32_t length;   //total program package length
}UpdateRam;

void Update_Operation(uint8_t *segment_ptr[],uint8_t *tmp_string);
uint16_t ChkCrcValueEx(uint8_t *buf, uint16_t len, uint16_t FirstData);
void GenerateSystemReset(void);
#endif

