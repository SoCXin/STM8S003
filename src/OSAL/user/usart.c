
#include "usart.h"
#include "iostm8s003f3.h"

/****************************************************************************************
 * 函数名  ：Usart_Init
 * 描述    ：Usart初始化函数
 * 输入    ：bound - 波特率
 * 输出    ：串口输出
 * 返回    ：无
 * 调用    ：外部调用
 ***************************************************************************************/ 
void Usart_Init(uint32 bound)
{
	//UART_TX
    PD_DDR |= (1<<5);			//PD5配置为输出模式
	PD_CR1 |= (1<<5);			//推挽输出
	PD_CR2 |= (1<<5); 			//输出速率10MHz

	//USART_RX
	PD_DDR &= ~(1<<6);			//PD6配置为输入模式
	PD_CR1 |= (1<<6);			//上拉输入

	CLK_PCKENR1 |= (1<<2);		//使能USART时钟

	//设置波特率
	bound /= 100;

	//写BRR2应当先于在写BRR1，因为对BRR1的写操作会更新波特计数器
	UART1_BRR2 |= (unsigned char)( (((uint32)160000/bound)>>8)&0xF0 | ((uint32)160000/bound)&0x0F );
    UART1_BRR1 |= (unsigned char)( ((uint32)160000/bound)>>4 );

    UART1_CR2 |= (1<<2) | (1<<3);		//使能接收、发送
    // UART1_CR2 |= (1<<5);				//使能接收中断
} 

/****************************************************************************************
 * 函数名：Usart_transmit
 * 描述  ：Usart发送单字符数据
 * 输入  ：data - 字符数据
 * 输出  ：串口输出
 * 返回  ：无
 * 调用  ：内部调用
 ***************************************************************************************/
void Usart_Transmit(unsigned char data)
{
    while(!(UART1_SR&0x40));		//等待发送空
    UART1_DR = data;				//发送
}
/****************************************************************************************
 * 函数名：Usart_puts
 * 描述  ：Usart发送字符串数据
 * 输入  ：*string - 字符串
 * 输出  ：串口输出
 * 返回  ：无
 * 调用  ：内部调用
 ***************************************************************************************/
void Usart_Puts(char *string)
{
	while(*string)
		Usart_Transmit(*string++);
}
/****************************************************************************************
 * 函数名：Usart_printf
 * 描述  ：Usart打印格式字符串数据
 * 输入  ：*format,...- 格式字符串
 * 输出  ：串口输出
 * 返回  ：无
 * 调用  ：内、外部调用
 ***************************************************************************************/
void Usart_Printf(char *format, ...)
{
	__va_list arg_ptr;
	char str[100];

	va_start(arg_ptr, format);
	vsprintf(str, format, arg_ptr);
	va_end(arg_ptr);

	Usart_Puts(str);
}

//串口接收中断
#pragma vector=20							//串口中断函数
__interrupt void UART_RXNE_IRQHandler(void)
{
	uint8 cdata;

	cdata = UART1_DR;						//读数据寄存器会自动会自动清中断标志
	UART1_DR = cdata;
}

/************************************END OF FILE****************************************/
