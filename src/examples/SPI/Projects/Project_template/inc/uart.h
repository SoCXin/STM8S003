#ifndef __UART_H
#define __UART_H
#include "stm8s_uart2.h"
#include "stm8s_gpio.h"
#include "stm8s_clk.h"

void UART_Config(void);
//u8 uart_ReceiveData(void);
void uart_sendbyte (u8 byte);
void uart_SendString(u8 *string);
void uart_SendDatas(u8 *Datas,u16 Num);
#endif	//__UART_H