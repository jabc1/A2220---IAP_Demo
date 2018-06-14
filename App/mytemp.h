#ifndef _mytemp_h
#define _mytemp_h
#include "stm32f1xx.h" 

#define Rxlen	1024
typedef struct
{
	uint16_t lenght;
	uint8_t buff[Rxlen];
	uint8_t back_buff[Rxlen];
	uint8_t start;
	uint8_t stop;
	uint8_t over;
	
}_RxBuff;
extern _RxBuff Uart2Rx;
#endif
