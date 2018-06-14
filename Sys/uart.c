#include "uart.h"
#define uart2
#ifdef uart2
uint8_t temp;
extern _RxBuff Uart2Rx;
UART_HandleTypeDef huart2;
void MX_USART2_UART_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		;//_Error_Handler(__FILE__, __LINE__);
	}
}
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	if(uartHandle->Instance==USART2)
	{
		/* USART2 clock enable */
		__HAL_RCC_USART2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**USART2 GPIO Configuration    
		PA2     ------> USART2_TX
		PA3     ------> USART2_RX 
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_2;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}
void uart2_IRQHandler(void);

void USART2_IRQHandler(void)
{
	uart2_IRQHandler();
}
void uart2_IRQHandler()
{
	if((__HAL_UART_GET_FLAG(&huart2,UART_FLAG_RXNE)!=RESET))//有数据来
	{
		if(Uart2Rx.start == Uart2_Read)
		{
			Uart2Rx.start = Uart2_Wait;
			Uart2Rx.lenght = 0;
			Uart2Rx.buff[Uart2Rx.lenght++] = (uint8_t)(huart2.Instance->DR);
		}
		else if(Uart2Rx.start == Uart2_Wait)
		{
			Uart2Rx.buff[Uart2Rx.lenght++] = (uint8_t)(huart2.Instance->DR);
		}
		__HAL_UART_CLEAR_FLAG(&huart2,UART_FLAG_RXNE);
	}
	if((__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE)!=RESET))  //进入空闲中断
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart2);
		//if(Uart2Rx.start == Uart2_Wait)
		{
			Uart2Rx.over = Uart2_Over;
		}
		__HAL_UART_DISABLE_IT(&huart2,UART_IT_IDLE);//关闭空闲中断
		__HAL_UART_DISABLE_IT(&huart2,UART_IT_RXNE);//关了接收完成中断
	}
}
void Uart2_init()
{
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	Uart2Rx.start = Uart2_Read;
	Uart2Rx.over = 0x00;
	//__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);  //打开空闲中断
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_PE);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_ERR);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
}
void printf_test()
{
	if(Uart2Rx.over	== Uart2_Over)
	{
		printf("====%s",Uart2Rx.buff);
		Uart2Rx.start  = Uart2_Read;
		Uart2Rx.over = 0x00;
		Uart2Rx.lenght = 0x00;
		__HAL_UART_ENABLE_IT(&huart2, UART_IT_PE);
		__HAL_UART_ENABLE_IT(&huart2, UART_IT_ERR);
		__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);		
	}
}
void Uart2_Restatr()
{
	Uart2Rx.over = 0x00;
	Uart2Rx.start  = Uart2_Read;
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_PE);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_ERR);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
}
#define test_printf
#ifdef test_printf
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
	USART2->DR = (u8) ch;      
	return ch;
}
#endif



#endif














