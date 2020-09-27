/**
 ******************************************************************************
 * @file    src\main.c
 * @author  bluebear233
 * @version V1.0
 * @date    2015-12-2
 * @brief   This file contains the main function for example project.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "I2C.h"
#include "usart.h"
#include "delay.h"
#include "stm8s.h"
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Evalboard I/Os configuration */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
void main(void) {
	/* System Init */
	CLK_SYSCLKConfig (CLK_PRESCALER_HSIDIV1);

	/* GPIO Init */
	GPIO_Init(GPIOA, GPIO_PIN_ALL, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(GPIOB, GPIO_PIN_ALL, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(GPIOC, GPIO_PIN_ALL, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(GPIOD, GPIO_PIN_ALL, GPIO_MODE_OUT_PP_LOW_SLOW);

	/* Close All Peripheral Clock */
	CLK->PCKENR1 = 0X00;
	CLK->PCKENR2 = 0X00;

	Delay_Config();

	USART_Config(57600);

	I2C_Config_Poll(100000);

	uint8_t buf;

	buf = 0x02;

	I2C_Multiple_Write_Poll(0X77, 0X2A, &buf, 1);

	I2C_Multiple_Read_Poll(0X77, 0XD0, &buf, 1);

	while (TRUE) {
		printf("c projcet example \n\r\n");
	}
}

#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *   where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (TRUE)
	{
		printf("Wrong parameters value: file %s on line %lu\r\n", file, line);
	}
}
#endif

/**
 * @}
 */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
