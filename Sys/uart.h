#ifndef _uart_h
#define _uart_h
#include "stm32f1xx.h"
#define 	Uart2_Read		0x01
#define		Uart2_Wait		0x02
#define		Uart2_Over		0x03
void MX_USART2_UART_Init(void);
void printf_test(void);
void Uart2_init(void);
void Uart2_Restatr(void);
#endif

