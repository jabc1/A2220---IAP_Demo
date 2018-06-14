#ifndef _r485_h
#define _r485_h
#include "stm32f1xx.h" 
void USART2_485_Recv_Enable(void);
void USART2_485_Send_Enable(void);
void RS485_Send_Data(char *buf,uint32_t len1);
void Init_R485(void);
void USART2_Printf(const char *fmt, ...);
#endif

