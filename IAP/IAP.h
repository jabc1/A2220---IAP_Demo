#ifndef _iap_h
#define _iap_h
//#include "stdint.h"
#include "stm32f1xx.h" 
typedef struct
{
	uint32_t flag;   //upgrade flag,=PROGRAM_UPGRADE_MODE or PROGRAM_UPGRADE_MODE
//	u8  type;   //board type
//	u8  ver;    //upgrade version
}UpdateFlash;
typedef struct 
{
	uint8_t  *Handle;
	uint8_t  *Handle_ID;
	uint8_t  *Node;
	uint8_t  *Node_ID;
	uint8_t  *Coder0;
	uint8_t  *Coder0_ID;
	uint8_t  *Coder1;
	uint8_t  *Coder1_ID;
	uint8_t  *Coder2;
	uint8_t  *Coder2_ID;
	uint8_t  *Coder3;
	uint8_t  *Coder3_ID;	
}_Command;
extern _Command Command;
void update_function(void);
void Command_Parse(void);
void  ProgramReady(uint16_t _lenth);
void UpdateFlag(void);
void IAP_Write_App_Bin ( uint32_t ulStartAddr, uint8_t * pBin_DataBuf, uint32_t ulBufLength );
uint16_t ChkCrcValueEx(uint8_t *buf, uint16_t len, uint16_t FirstData);
void JumpToAPP(void);



#endif

