#include "update.h"
/**********************************************
升级协议如下：
		Qpedate=1234,Node=0111,Boot=n,lenth,data(每一包数据都会带这个协议)
		其中n代表升级节点，1和2为引导，3以上是升级节点，
		lenth是每一包的bin文件长度，
		第一二包为16，接下来的为512.
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
		boot_node = atoi((char *)(segment_ptr[5])); //读取升级节点
		lenth =  atoi((char *)(segment_ptr[6])); //读取每一包长度
		command_lenth = 29 + strlen((char *)(segment_ptr[5])) + strlen((char *)(segment_ptr[6]));
		if(boot_node == 2) //擦除flash并且记录升级长度	
		{
			udata = (UpdateRam *)&tmp_string[command_lenth];
			if(udata->ver == VERSION) //版本相同，不升级，标志位弄为正常标志位并且软件复位跳转到APP
			{
				//更新flash标志并软件复位
				UpdateFlag();
				GenerateSystemReset();					
			}
			else if((udata->type == Edition_type) && (lenth == UPGRADE_LENGTH_FIRST_SECTION)) //是本板类型并且首包为16个字节
			{
				update_crc16 = udata->crc16;//记录CRC
				update_lenth = udata->length; //记录长度
				segnum = update_lenth%512 ? (update_lenth>>9) + 1 : (update_lenth>>9); //拆分512个字节，记录总的升级包数量
				ProgramReady(update_lenth);//擦除APP flash
			}
			USART2_Printf("boot");
		}	
		else if((segnum != (boot_node - 2)) &&(boot_node > 2)) //第1包到n-1包
		{	
			//printf("%4d\r\n",temp++);			
			update_addr = Flash_APP_ADDR + (boot_node - 3)*lenth;			  
			IAP_Write_App_Bin(update_addr,&tmp_string[command_lenth],lenth);	
			
		}
		else if(segnum == (boot_node - 2)) //最后一包
		{
			//printf("%4d\r\n",temp++);
			update_addr = Flash_APP_ADDR + (boot_node - 3) * 512;
			IAP_Write_App_Bin(update_addr,&tmp_string[command_lenth],lenth);	
			paddr = (uint8_t *)(Flash_APP_ADDR);
			if(update_crc16 == ChkCrcValueEx(paddr,update_lenth,0xFFFF)) //check正确
			{
				//更新flash标志并软件复位
				UpdateFlag();
				GenerateSystemReset();
			}
		}		
	}
}

void GenerateSystemReset(void)
{
	__set_FAULTMASK(1);      // 关闭所有中端
//	NVIC_SystemReset();// 复位
	HAL_NVIC_SystemReset();
}



