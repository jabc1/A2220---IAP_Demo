#include "update.h"
/**********************************************
����Э�����£�
		Qpedate=1234,Node=0111,Boot=n,lenth,data(ÿһ�����ݶ�������Э��)
		����n���������ڵ㣬1��2Ϊ������3�����������ڵ㣬
		lenth��ÿһ����bin�ļ����ȣ�
		��һ����Ϊ16����������Ϊ512.
**********************************************/

UpdateRam *udata;
uint16_t update_crc16;
uint32_t update_lenth;
uint8_t  segnum;
void Update_Operation(uint8_t *segment_ptr[],uint8_t *tmp_string)
{
	static uint16_t temp = 0;
	uint16_t  boot_node;
	uint32_t update_addr;
	uint16_t i;
	uint8_t *paddr;
	uint16_t lenth;
	uint16_t  command_lenth;
	if(strcmp((char *)(segment_ptr[4]), "Version") == 0)
	{
		USART2_Printf("Bootleader version=%04d\r\n",VERSION);
	}	
	if(strcmp((char *)(segment_ptr[4]), "Boot") == 0)
	{
		boot_node = atoi((char *)(segment_ptr[5])); //��ȡ�����ڵ�
		lenth =  atoi((char *)(segment_ptr[6])); //��ȡÿһ������
		command_lenth = 29 + strlen((char *)(segment_ptr[5])) + strlen((char *)(segment_ptr[6]));
		if(boot_node == 2) //����flash���Ҽ�¼��������	
		{
			udata = (UpdateRam *)&tmp_string[command_lenth];
			if(udata->ver == VERSION) //�汾��ͬ������������־λŪΪ������־λ���������λ��ת��APP
			{
				//����flash��־�������λ
				UpdateFlag();
				GenerateSystemReset();					
			}
			else if((udata->type == Edition_type) && (lenth == UPGRADE_LENGTH_FIRST_SECTION)) //�Ǳ������Ͳ����װ�Ϊ16���ֽ�
			{
				update_crc16 = udata->crc16;//��¼CRC
				update_lenth = udata->length; //��¼����
				segnum = update_lenth%512 ? (update_lenth>>9) + 1 : (update_lenth>>9); //���512���ֽڣ���¼�ܵ�����������
				ProgramReady(update_lenth);//����APP flash
			}
			USART2_Printf("boot");
		}	
		else if((segnum != (boot_node - 2)) &&(boot_node > 2)) //��1����n-1��
		{	
			//printf("%4d\r\n",temp++);			
			update_addr = Flash_APP_ADDR + (boot_node - 3)*lenth;			  
			IAP_Write_App_Bin(update_addr,&tmp_string[command_lenth],lenth);	
			
		}
		else if(segnum == (boot_node - 2)) //���һ��
		{
			//printf("%4d\r\n",temp++);
			update_addr = Flash_APP_ADDR + (boot_node - 3) * 512;
			IAP_Write_App_Bin(update_addr,&tmp_string[command_lenth],lenth);	
			paddr = (uint8_t *)(Flash_APP_ADDR);
			if(update_crc16 == ChkCrcValueEx(paddr,update_lenth,0xFFFF)) //check��ȷ
			{
				//����flash��־�������λ
				UpdateFlag();
				GenerateSystemReset();
			}
		}		
	}
}

void GenerateSystemReset(void)
{
	__set_FAULTMASK(1);      // �ر������ж�
//	NVIC_SystemReset();// ��λ
	HAL_NVIC_SystemReset();
}



