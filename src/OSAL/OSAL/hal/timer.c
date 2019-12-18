/****************************************************************************************
 * 文件名  ：timer.c
 * 描述    ：硬件定时器配置文件，为osal操作系统提供系统时钟，移植时需要修改的文件
 * 开发平台：STM8S003F3
 * 库版本  ：无
 ***************************************************************************************/
#include "timer.h"
#include "osal_timer.h"

//硬件定时器初始化，设定系统时钟
void OSAL_TIMER_TICKINIT(void)
{
	CLK_PCKENR1 |= (1<<5);					//使能TIM2时钟

	TIM2_PSCR = 4; 							//配置 TIM2 预分频，2^4 = 16 分频，则计时频率1MHz
	TIM2_ARRH = 1000/256; 					//自动重装值，1ms中断
	TIM2_ARRL = 1000%256;
	TIM2_CNTRH = 1000/256;					//计时器值
	TIM2_CNTRL = 1000%256;
	TIM2_CR1 |= 0x81; 						//使能TIM2及自动重装载
	TIM2_IER |= 0x01; 						//使能TIM2溢出中断
	__asm("rim");                           //使能全局中断
}

//开启硬件定时器
void OSAL_TIMER_TICKSTART(void )
{
	TIM2_CR1 |= 0x01; 						//使能TIM2
}

//关闭硬件定时器
void OSAL_TIMER_TICKSTOP(void )
{
	TIM2_CR1 &= ~0x01; 						//失能TIM2
}

//此处添加硬件定时器中断溢出函数，并调用系统时钟更新函数osal_update_timers()
#pragma vector=15							//TIM2中断函数
__interrupt void TIM2_UPDATE_IRQHandler(void)
{
	TIM2_SR1 &=~(0x01);						//清除中断标志

	osal_update_timers();
}
