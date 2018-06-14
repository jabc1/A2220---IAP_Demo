#include "R485.h"
void Init_R485()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
	USART2_485_Recv_Enable();
}
void USART2_485_Recv_Enable()
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);//0 Recevice
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
}
void USART2_485_Send_Enable()
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);//1 Send
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
}

void USART2_Printf(const char *fmt, ...)
{
	//sprintf(USART_RAM, fmt,...);
	char Buff[256];
	va_list ap;      
	va_start(ap, fmt);  
	vsprintf(Buff, fmt, ap);
	va_end(ap);
	RS485_Send_Data(Buff,strlen(Buff));
}
void RS485_Send_Data(char *buf,uint32_t len)
{
	uint32_t  t;
	USART2_485_Send_Enable();        //设置为发送模式
	for(t=0;t<len;t++)               //循环发送数据
	{
		while((USART2->SR&0X40)==0);  //等待发送结束             
		USART2->DR=buf[t];
	}    
	while((USART2->SR&0X40)==0);     //等待发送结束    
	USART2_485_Recv_Enable();        //设置为接收模式 	
}


