#include "IAP.h"
UpdateFlash UpdateF;
_Command Command;

void update_function()
{
	Command_Parse();
}

void Command_Parse(void)
{
	uint32_t j;
	uint8_t *token,*segment_ptr[15];
	uint8_t i,ID;
	if(Uart2Rx.over == Uart2_Over)//接收完成一帧数据
	{
		if((Uart2Rx.buff[0] == 'Q')|| (Uart2Rx.buff[0] == 'R'))
		{
			// 清空命令段指针
			for(i = 0; i < 12; i++)
			{
				segment_ptr[i] = NULL;
			}
			// 字符串分段
			i = 0;
			for(j=0;j<Uart2Rx.lenght;j++)
			{
				Uart2Rx.back_buff[j] = Uart2Rx.buff[j];
			}		
			token = (uint8_t *)strtok((char *)(Uart2Rx.buff), ",=");
			//printf("=%s\r\n",(Uart2Rx.buff));
			while(token != NULL && i < 12)
			{
				segment_ptr[i] = token;	
				//printf("%s\r\n",segment_ptr[i]);	
				i++;
				token = (uint8_t *)strtok(NULL,",=");			
			}
			Command.Handle = segment_ptr[0];
			Command.Handle_ID = segment_ptr[1];
			Command.Node_ID = segment_ptr[3];
			ID = atoi((const char *)(segment_ptr[3]));
			if((strcmp((char *)(segment_ptr[2]), "Node")== 0)&&((ID == 0)||(ID == 0xff)))
			{
				if((strcmp((char *)(Command.Handle), "Qpdate")== 0))
				{
					Update_Operation(segment_ptr,(Uart2Rx.back_buff));
					//printf("=%s",(Uart2Rx.back_buff));
				}
			}
		}
		else
		{
			;
		}
		memset(Uart2Rx.buff,0,1024);
		Uart2_Restatr();
	}

}

static uint16_t ulBuf_Flash_App[1024];
void UpdateFlag(void)
{
	UpdateF.flag = PROGRAM_NORMAL_MODE;//不升级！
	STMFLASH_Write(FLASH_UPDATEFLAG_ADDR, (uint16_t *)&UpdateF,2);
}
void JumpToAPP(void)
{
	uint32_t flag;
	uint32_t IapSpInitVal;
	uint32_t IapJumpAddr;
	void (*pIapFun)(void);
	volatile uint32_t flash_flag;	
//	UpdateF.flag = PROGRAM_NORMAL_MODE;//不升级！
//	STMFLASH_Write(FLASH_UPDATEFLAG_ADDR, (uint16_t *)&UpdateF, 2);

	flag = *(uint32_t *)(FLASH_UPDATEFLAG_ADDR);//check jump para address
	
	IapSpInitVal = *(uint32_t *)Flash_APP_ADDR;
	IapJumpAddr = *(uint32_t *)(Flash_APP_ADDR + 4);	
	if(flag == PROGRAM_NORMAL_MODE)
	{
		if((IapSpInitVal & 0x2FFE0000)==0x20000000)//检查栈顶地址是否合法.
		{
			__set_MSP(IapSpInitVal);
			pIapFun = (void (*)(void))IapJumpAddr;   
			(*pIapFun) ();
		}
		while(1);
	}
}
void IAP_Write_App_Bin ( uint32_t ulStartAddr, uint8_t * pBin_DataBuf, uint32_t ulBufLength )
{
	uint16_t us, usCtr=0, usTemp;
	uint32_t ulAdd_Write = ulStartAddr;                                //当前写入的地址
	uint8_t * pData = pBin_DataBuf;
	
	for ( us = 0; us < ulBufLength; us += 2 )
	{						    
		usTemp =  ( uint16_t ) pData[1]<<8;
		usTemp += ( uint16_t ) pData[0];	  
		pData += 2;                                                      //偏移2个字节
		ulBuf_Flash_App [ usCtr ++ ] = usTemp;	    
		if ( usCtr == 1024 )
		{
			usCtr = 0;
			STMFLASH_Write ( ulAdd_Write, ulBuf_Flash_App, 1024 );	
			ulAdd_Write += 2048;                                           //偏移2048  16=2*8.所以要乘以2.
		}
	}
	if ( usCtr ) 
    STMFLASH_Write ( ulAdd_Write, ulBuf_Flash_App, usCtr );//将最后的一些内容字节写进去.  
}
const uint16_t CRCTable[] =
{
    //多项式为    0x18005   X16+X15+X2+1
    0x0000,0xC0C1,0xC181,0x0140,0xC301,0x03C0,0x0280,0xC241,
    0xC601,0x06C0,0x0780,0xC741,0x0500,0xC5C1,0xC481,0x0440,
    0xCC01,0x0CC0,0x0D80,0xCD41,0x0F00,0xCFC1,0xCE81,0x0E40,
    0x0A00,0xCAC1,0xCB81,0x0B40,0xC901,0x09C0,0x0880,0xC841,
    0xD801,0x18C0,0x1980,0xD941,0x1B00,0xDBC1,0xDA81,0x1A40,
    0x1E00,0xDEC1,0xDF81,0x1F40,0xDD01,0x1DC0,0x1C80,0xDC41,
    0x1400,0xD4C1,0xD581,0x1540,0xD701,0x17C0,0x1680,0xD641,
    0xD201,0x12C0,0x1380,0xD341,0x1100,0xD1C1,0xD081,0x1040,
    0xF001,0x30C0,0x3180,0xF141,0x3300,0xF3C1,0xF281,0x3240,
    0x3600,0xF6C1,0xF781,0x3740,0xF501,0x35C0,0x3480,0xF441,
    0x3C00,0xFCC1,0xFD81,0x3D40,0xFF01,0x3FC0,0x3E80,0xFE41,
    0xFA01,0x3AC0,0x3B80,0xFB41,0x3900,0xF9C1,0xF881,0x3840,
    0x2800,0xE8C1,0xE981,0x2940,0xEB01,0x2BC0,0x2A80,0xEA41,
    0xEE01,0x2EC0,0x2F80,0xEF41,0x2D00,0xEDC1,0xEC81,0x2C40,
    0xE401,0x24C0,0x2580,0xE541,0x2700,0xE7C1,0xE681,0x2640,
    0x2200,0xE2C1,0xE381,0x2340,0xE101,0x21C0,0x2080,0xE041,
    0xA001,0x60C0,0x6180,0xA141,0x6300,0xA3C1,0xA281,0x6240,
    0x6600,0xA6C1,0xA781,0x6740,0xA501,0x65C0,0x6480,0xA441,
    0x6C00,0xACC1,0xAD81,0x6D40,0xAF01,0x6FC0,0x6E80,0xAE41,
    0xAA01,0x6AC0,0x6B80,0xAB41,0x6900,0xA9C1,0xA881,0x6840,
    0x7800,0xB8C1,0xB981,0x7940,0xBB01,0x7BC0,0x7A80,0xBA41,
    0xBE01,0x7EC0,0x7F80,0xBF41,0x7D00,0xBDC1,0xBC81,0x7C40,
    0xB401,0x74C0,0x7580,0xB541,0x7700,0xB7C1,0xB681,0x7640,
    0x7200,0xB2C1,0xB381,0x7340,0xB101,0x71C0,0x7080,0xB041,
    0x5000,0x90C1,0x9181,0x5140,0x9301,0x53C0,0x5280,0x9241,
    0x9601,0x56C0,0x5780,0x9741,0x5500,0x95C1,0x9481,0x5440,
    0x9C01,0x5CC0,0x5D80,0x9D41,0x5F00,0x9FC1,0x9E81,0x5E40,
    0x5A00,0x9AC1,0x9B81,0x5B40,0x9901,0x59C0,0x5880,0x9841,
    0x8801,0x48C0,0x4980,0x8941,0x4B00,0x8BC1,0x8A81,0x4A40,
    0x4E00,0x8EC1,0x8F81,0x4F40,0x8D01,0x4DC0,0x4C80,0x8C41,
    0x4400,0x84C1,0x8581,0x4540,0x8701,0x47C0,0x4680,0x8641,
    0x8201,0x42C0,0x4380,0x8341,0x4100,0x81C1,0x8081,0x4040
};
uint16_t ChkCrcValueEx(uint8_t *buf, uint16_t len, uint16_t FirstData)
{
    uint16_t i;
    uint8_t  index;
    uint8_t  CRCH = 0;                      //以前是0xFF,由于上位机改成了0，此处改成0
    uint8_t  CRCL = 0;                      //以前是0xFF,由于上位机改成了0，此处改成0
    
    CRCH = (uint8_t)(FirstData>>8);
    CRCL = (uint8_t)(FirstData);
    
    for( i = 0 ; i < len ; i++ )                            //传输消息缓冲区
    {
        index = CRCH ^ buf[i];
        CRCH  = (uint8_t)CRCL ^ (uint8_t)CRCTable[index];
        CRCL  = CRCTable[index] >> 8;
    }
    return( CRCL<<8|CRCH) ;     //CRCL 在数据流中先发送 CRCH 在数据流中后发送
		
}


