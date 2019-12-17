/**
 ******************************************************************************
 * @file    src\SYSTEM\usart.c
 * @author  bluebear233
 * @version V1.0
 * @date    2015-12-2
 * @brief   This file is functions for config USART
 ******************************************************************************
 */
#include <stdio.h>
#include <usart.h>

/* Private funcitons ****************************************************/
int putchar(int c);
/* Public functions *****************************************************/
/**
 * @brief  Configure USART peripheral to print characters on Hyperteminal
 * @param  BaudRate : USART BaudRate
 * @retval None
 */
void USART_Config(uint32_t BaudRate) {

	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);

	UART1_DeInit();

	UART1_Init(BaudRate, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
			UART1_SYNCMODE_CLOCK_DISABLE,
			(UART1_Mode_TypeDef)(UART1_MODE_TX_ENABLE));
}

/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  c Character to send
 * @retval char Character sent
 */
int putchar(int c) {
	/* Write a character to the USART */
	UART1_SendData8(c);

	/* Loop until the end of transmission */
	while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
		;

	return (c);
}
