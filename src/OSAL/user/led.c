#include "led.h"

void LED_Init(void)
{
	PA_DDR |= (1<<3);	//PA3配置为输出模式
	PA_CR1 |= (1<<3);	//推挽输出
	PA_CR2 |= (1<<3); 	//输出速率10MHz

	LED0 = 0;
}