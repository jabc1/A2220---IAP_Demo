#ifndef _flash_h
#define _flash_h
#include "stm32f1xx.h" 
//---------------------Ԥ����10k��EEPROM�洢
//--------------------0x0800 D400--------->1page---------------//
//--------------------0x0800 D800--------->1page---------------//
//--------------------0x0800 DC00--------->1page---------------//
//--------------------0x0800 E000--------->1page---------------//
//--------------------0x0800 E400--------->1page---------------//
//============================>��5k
//--------------------0x0800 E800--------->1page---------------//
//--------------------0x0800 EC00--------->1page---------------//
//--------------------0x0800 F000--------->1page---------------//
//--------------------0x0800 F400--------->1page last---------------//
#define NVIC_VectTab_FLASH				0X08000000
#define Bootloader_flag_ADD				0X08005c00
#define PROGRAM_UPGRADE_MODE      		0x55FFFF55 //upgrade mode��run in bootload 
#define PROGRAM_NORMAL_MODE      		0xAA0000AA //normal mode��run to app

#define	STM32_FLASH_BASE_ADD			0X0800E800

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
/************************** STM32 �ڲ� FLASH ���� *****************************/
#define STM32_FLASH_SIZE        64  // ��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN        1    // stm32оƬ����FLASH д��ʹ��(0������;1��ʹ��)

/* ��չ���� ------------------------------------------------------------------*/

/* �������� ------------------------------------------------------------------*/
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);		  //��������

void STMFLASH_WriteLenByte(uint32_t WriteAddr, uint32_t DataToWrite, uint16_t Len );	      //ָ����ַ��ʼд��ָ�����ȵ�����
uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr, uint16_t Len );					                    	//ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write( uint32_t WriteAddr, uint16_t * pBuffer, uint16_t NumToWrite );		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read( uint32_t ReadAddr, uint16_t * pBuffer, uint16_t NumToRead );   	//��ָ����ַ��ʼ����ָ�����ȵ�����

void CPU_FLASH_TEST(void);




#endif

