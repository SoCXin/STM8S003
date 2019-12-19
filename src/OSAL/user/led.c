#include "led.h"

void LED_Init(void)
{
	PD_DDR |= (1<<3);	//PD3配置为输出模式
	PD_CR1 |= (1<<3);	//推挽输出
	PD_CR2 |= (1<<3); 	//输出速率10MHz
	LED0 = 0;
}