/******************************************************************************

IAP 跳转到 APP 之间跳转函数

******************************************************************************/

void IAP_APP_Jump (void)

{

INT32U SpInitVal; //要跳转到程序的SP初值.

INT32U JumpAddr; //要跳转到程序的地址.即,要跳转到程序的入口

void (*pFun)(void); //定义一个函数指针.用于指向APP程序入口

RCC_DeInit(); //关闭外设

NVIC_DeInit ();                           //恢复NVIC为复位状态.使中断不再发生.

SpInitVal = *(INT32U *)IAP_ADDR;//IAP_ADDR IAP的栈顶地址（0x08000000）

//跳转到APP时 APP_ADDR AAP的栈顶地址（如：0x08003800）

JumpAddr = *(INT32U *)( IAP_ADDR + 4); //设置复位中断向量（如上面流程分析）

__set_MSP(SpInitVal); //设置SP.，堆栈栈顶地址

pFun = (void (*)(void))JumpAddr; //生成跳转函数.将复位中断向量地址做为函数指针

(*pFun) (); //执行函数，实现跳转.不再返回.

}

在IAP和APP中都需要进行中断向量表的设置，如正常程序中设置一样。
2、 经上一步修改后，IAP可以跳转到APP中，APP也能跳转到IAP中，但再重IAP就不能再跳回APP了。修改跳转函数

/***********************************************************************

函数功能：跳转到IAP函数

***********************************************************************/

void JumpToIAP(void)

{

INT32U IapSpInitVal;

INT32U IapJumpAddr;

void (*pIapFun)(void);

RCC_DeInit();

NVIC_DeInit();

__disable_irq(); //关中断（）

// APP如跳转前关中断，跳转到IAP后，IAP初始化后要打开中断

IapSpInitVal = *(INT32U *)IAP_ADDR;

IapJumpAddr = *(INT32U *)(IAP_ADDR + 4);

__set_CONTROL(0);

//进入用户级线程模式 进入软中断后才可以回到特权级线程模式

//APP如使用系统如ucos必须要有此过程否则跳到IAP后，无法再次跳到APP

__set_MSP (IapSpInitVal);

pIapFun = (void (*)(void))IapJumpAddr;

(*pIapFun) ();

}
/***********************************************************************

IAP 跳转到 APP 函数

***********************************************************************/

void Jumpto_APP(void)

{

INT32U IapSpInitVal;

INT32U IapJumpAddr;

void (*pIapFun)(void);

RCC_DeInit();//关闭外设

NVIC_DeInit();

__disable_irq(); //关中断（）如IAP关中断 APP如果没用UCOS系统，APP

//初始化后要开中断，用UCOS后，在起动任务后会开中断

IapSpInitVal = *(INT32U *)APP1_ADDR;

IapJumpAddr = *(INT32U *)(APP1_ADDR + 4);

if((IapSpInitVal & 0x2FFE0000)==0x20000000)//检查栈顶地址是否合法.

{

__set_MSP (IapSpInitVal);

pIapFun = (void (*)(void))IapJumpAddr;

(*pIapFun) ();

}

}
/***************************************************************************

更能：关闭计时器

输入参数：num 定时器号

***************************************************************************/

void CloseTim(u8 num)

{

u32 tim = 0;

tim = (APB1PERIPH_BASE + (num-2)*0x0400); //计算定时器地址

TIM_ITConfig((TIM_TypeDef*)tim,TIM_IT_Update,DISABLE);

TIM_Cmd((TIM_TypeDef*)tim, DISABLE);

}
但在BSP_Init()函数中确实有对复位中断向量进行了设置。

NVIC_SetVectorTable(NVIC_VectTab_FLASH,(APP1_ADDR+4));

说明一下，这里将中断向量表设置为 （APP1_ADDR+4）（偏移4个字节即为复位中断向量），而有的程序中设置为APP1_ADDR。实际上两种设置是一样的，在

NVIC_SetVectorTable()函数中执行下句

SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80); 所以可以看出是否偏移4字节是一样的。

经测试发现需要在调用OSStart()的前一句重新设置复位中断向量才能正确设置。
