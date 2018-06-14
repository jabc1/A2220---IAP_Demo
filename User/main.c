/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx.h"  
IWDG_HandleTypeDef hiwdg;
void MX_IWDG_Init(uint8_t prv ,uint16_t rlv)
{
	hiwdg.Instance = IWDG;
	hiwdg.Init.Prescaler = prv;
	hiwdg.Init.Reload = rlv;
	HAL_IWDG_Init(&hiwdg);
}
int main(void)
{
	JumpToAPP();
	HAL_Init();
	SystemClock_Config();
	MX_USART2_UART_Init();
	Init_R485();
	Uart2_init();
	MX_IWDG_Init(IWDG_PR_PR_2,625*3);//1s
	//USART2_Printf("test");
	while (1)
	{
		HAL_IWDG_Refresh(&hiwdg);
		update_function();
	}
}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
