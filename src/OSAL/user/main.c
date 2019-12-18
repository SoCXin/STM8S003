#include "iostm8s003f3.h"
#include "usart.h"
#include "led.h"

//硬件定时器初始化，设定系统时钟
void TIM2_Init(void)
{
	CLK_PCKENR1 |= (1<<5);					//使能TIM2时钟

	TIM2_PSCR = 0; 							//配置 TIM2 预分频，2^4 = 16 分频，则计时频率1MHz
	TIM2_ARRH = 50000/256; 					//自动重装值，1ms中断
	TIM2_ARRL = 50000%256;
	TIM2_CNTRH = 1000/256;					//计时器值
	TIM2_CNTRL = 1000%256;
	TIM2_CR1 |= 0x81; 						//使能TIM2及自动重装载
	TIM2_IER |= 0x01; 						//使能TIM2溢出中断
	__asm("rim");                           //使能全局中断
}

//此处添加硬件定时器中断溢出函数，并调用系统时钟更新函数osal_update_timers()
#pragma vector=15							//TIM2中断函数
__interrupt void TIM2_UPDATE_IRQHandler(void)
{
	TIM2_SR1 &=~(0x01);						//清除中断标志

	LED0 = ~LED0;
}

int main(void)
{
    CLK_CKDIVR = 0x00;			//系统时钟不分频，等于内部高速时钟16MHz
    //LED_Init();
    TIM2_Init();
    Usart_Init(115200);
    Usart_Printf("STM8 Run OSAL System !\r\n");
    while(1)
    {
            
    }
}