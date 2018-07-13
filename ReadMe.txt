/**
******************************************************************************
* @file    ReadMe.txt	bin文件生成连接 
* @author  KK			例如======>C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --bin -o A-2220.bin  ./Objects/HAL_Demo.axf
* @date    2018.05.02
* @brief   用于IAP移植使用
******************************************************************************
*/
20180519
1、初始工程用于移植iap，通过串口进行升级
20180521
1、添加IAP串口空闲中断
2、IAP升级程序测试成功
3、添加485通信，测试成功
20180615
1、添加IAP跳转说明

Flash分区参考：
Flash 大小	Bootloader_0大小	Bootloader_1大小	标志区	用户区大小	用户数据区大小
64K Flash		1K	23K	1K	39K	保留①
128K Flash	1K	23K	1K	103K	保留①
256K Flash	2K	30K	2K	222K	保留①
384K Flash	2K	30K	2K	350K	保留①
512K Flash	2K	30K	2K	478K	保留①
1M Flash		16K	16K+64K	16K	912K	保留①
说明：
①、在bootloader_1缺失的情况下，不允许升级APP程序。
②、用户区大小和数据区的大小是不同的设备产品而定义，不在做统一规定，在制作烧录包时，需要设计者指定最小的用户区保留大小；
升级包参数（16个字节）
板类型(1 byte) + APP/IAP(1 byte) + 版本号(1 byte) + CRC(2 byte) + 长度值(2 byte) + 9byte(空)
③、标志区目前主要存储以下数据（可拓展）
struct _UpfradeInfo	 //flag区信息
{
	unsigned int Jump_flag;				//Bootloader_0程序跳转标记
	unsigned int bootloader_Version;	   //Bootloader版本号
	unsigned char bootloader0_Flag;		//bootloader0标志
	unsigned char bootloader1_Flag;		//bootloader1标志
};


