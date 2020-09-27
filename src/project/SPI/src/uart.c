
#include "uart.h"

void UART_Config(void)
{
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1,ENABLE);								//串口1时钟开 
	GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);
	UART1_Cmd(ENABLE);
	UART1_Init(9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1,UART1_PARITY_NO, UART1_SYNCMODE_LASTBIT_DISABLE, UART1_MODE_TXRX_ENABLE);
	UART1_ITConfig(UART1_IT_TXE, DISABLE);//关串口开始发送中断  
	UART1_ITConfig(UART1_IT_TC, DISABLE);//关串口发送结束中断 
//	UART1_ITConfig(UART1_IT_RXNE, ENABLE);
	
}

void uart_sendbyte (u8 byte)
{
    while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);
    UART1_SendData8(byte);    
}
void uart_SendString(u8 *string)
{
  	while((*string) != 0x00)
	{
		while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);
		UART1_SendData8(*string);
		string++;
	}
}

void uart_SendDatas(u8 *Datas,u16 Num)
{
        u8 *Stk;
  	u16 Counter=Num;
        Stk = Datas;
  	while(Counter)
	{
		while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);
		UART1_SendData8(*Stk);
		Stk++;
                Counter --;
	}
}


/*u8 uart_ReceiveData(void)
{
  	u8 data=0;
	while(UART1_GetFlagStatus(UART1_FLAG_RXNE) != RESET);
	data = UART1_ReceiveData8(  );
	return data;
}*/

