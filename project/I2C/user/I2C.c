/**
 ******************************************************************************
 * @file     src/SYSTEM/I2C.c
 * @author   luzhipeng
 * @version  V1.2
 * @date     13-Dec-2015
 * @brief    This file containsall the functions for I2C Communication.
 ******************************************************************************
 @par How to use it ?

 1.实现Hard I2C PIN的硬件I2C引脚定义
 2.根据所需功能实现Funtions Define
 3.如果在中断里面读取I2C应使用I2C_Multiple_Read_Poll_In_Interrupt，写不区分
 4.定义了DEBUG，应在I2C初始化前先初始化串口

 */

/* Includes ------------------------------------------------------------------*/
#include "I2C.h"
#include "stm8s.h"
#include "delay.h"

/* Hard I2C PIN ---------------------------------------------------------------*/
#define I2C_SDA_GPIO GPIOB
#define I2C_SCL_GPIO GPIOB
#define I2C_SDA_PIN GPIO_PIN_5
#define I2C_SCL_PIN GPIO_PIN_4

/* Funtions Define -----------------------------------------------------------*/
#define DEBUG                            //串口调试
#define I2C_AUTO_RELEASE_DEADLOCK        //自动解决I2C死锁,注释了所有读写操作的函数失败时都会返回!0，否则失败时会解决I2C死锁，超时时并复位单片机
#define AUTO_RESET_WHEN_I2C_DEAD_LOCK    //解决死锁后自动复位单片机
#define Delay_1_ms() Delay_ms(1)         //延时1ms函数
//#define USE_LIB                          //使用库

/* Private Define ------------------------------------------------------------*/
#ifdef DEBUG
#include <stdio.h>
#define SET_COMMUNICATIO_STATE(state) if(g_TIME_COUNT)g_COMMUNICATIO_STATE = state
#else
#define SET_COMMUNICATIO_STATE(state)
#endif

//COMMUNICATIO_STATE
#define I2C_BUYS 0X01
#define I2C_RESPONSE 0x02
#define I2C_SLAVE_FINED 0X03
#define I2C_COMMUNICATIO 0X04

#define START_TIME_COUNT() g_TIME_COUNT=255

/* Pivate variables ----------------------------------------------------------*/
static uint8_t g_TIME_COUNT = 255;
#ifdef DEBUG
static uint8_t g_COMMUNICATIO_STATE = 0;
static uint8_t g_I2C_CONFIG_STATE = 0;
#endif

/* Pivate Function -----------------------------------------------------------*/
static void I2C_Release_deadlock();
static uint8_t TIME_OUT();
static void RELOAD_TIME_COUNT();
static void Printf_I2C_Error();

/* Public Functions ----------------------------------------------------------*/
/**
 * @brief I2C Poll Config
 * @param OutputClockFrequencyHz : I2C Communication Speed Hz
 * @return default 0
 */
uint8_t I2C_Config_Poll(uint32_t OutputClockFrequencyHz) {

	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);

	I2C_DeInit();

	I2C_Init(OutputClockFrequencyHz, 0, I2C_DUTYCYCLE_16_9, I2C_ACK_CURR,
			I2C_ADDMODE_7BIT, 16);

#if defined DEBUG
	g_I2C_CONFIG_STATE = 1;
#endif

	return 0;
}
/**
 * @brief  multiple read I2C by Poll.
 * @param  slave_address : slave address.
 * @param  reg_address : register address.
 * @param  buff_point ： want to write buffer point
 * @param  read_len : read lenght
 * @retval communication state 0 is ok
 */
uint8_t I2C_Multiple_Read_Poll(uint8_t slave_address, uint8_t reg_address,
		uint8_t *buff_point, uint8_t read_len) {
#ifdef USE_LIB
	START_TIME_COUNT();
	SET_COMMUNICATIO_STATE(I2C_BUYS);

	/*--------------- Wait I2C bus free -------------------------*/
	while ((I2C_GetFlagStatus(I2C_FLAG_BUSBUSY) == SET) && TIME_OUT()) {
		I2C_GenerateSTOP (ENABLE);
	}

	I2C_AcknowledgeConfig (I2C_ACK_CURR);

	/*--------------- Start communication -----------------------*/
	I2C_GenerateSTART (ENABLE);

	RELOAD_TIME_COUNT();
	SET_COMMUNICATIO_STATE(I2C_RESPONSE);
	while ((I2C_GetFlagStatus(I2C_FLAG_STARTDETECTION) == RESET) && TIME_OUT())
	;

	/*------------------ Address send ---------------------------*/
	I2C_Send7bitAddress(slave_address << 1, I2C_DIRECTION_TX);

	RELOAD_TIME_COUNT();
	SET_COMMUNICATIO_STATE(I2C_SLAVE_FINED);
	while ((I2C_GetFlagStatus(I2C_FLAG_ADDRESSSENTMATCHED) == RESET)
			&& TIME_OUT())
	;

	I2C->SR3;

	RELOAD_TIME_COUNT();
	while ((I2C_GetFlagStatus(I2C_FLAG_TXEMPTY) == RESET) && TIME_OUT())
	;

	/*--------------- Register/Command send ---------------------*/
	I2C_SendData(reg_address);

	RELOAD_TIME_COUNT();
	while ((I2C_GetFlagStatus(I2C_FLAG_TXEMPTY) == RESET) && TIME_OUT())
	;

	/*--------------- Restart communication ---------------------*/
	I2C_GenerateSTART(ENABLE);

	RELOAD_TIME_COUNT();
	while ((I2C_GetFlagStatus(I2C_FLAG_STARTDETECTION) == RESET) && TIME_OUT())
	;

	/*------------------ Address send ---------------------------*/
	I2C_Send7bitAddress(slave_address << 1, I2C_DIRECTION_RX);

	RELOAD_TIME_COUNT();
	SET_COMMUNICATIO_STATE(I2C_COMMUNICATIO);
	while ((I2C_GetFlagStatus(I2C_FLAG_ADDRESSSENTMATCHED) == RESET)
			&& TIME_OUT())
	;

	/*------------------- Data Receive --------------------------*/
	if (TIME_OUT()) {
		switch (read_len) {
			case 1:
			I2C_AcknowledgeConfig (I2C_ACK_NONE);
			disableInterrupts();
			I2C->SR3;
			I2C_GenerateSTOP(ENABLE);
			enableInterrupts();
			RELOAD_TIME_COUNT();
			while ((I2C_GetFlagStatus(I2C_FLAG_RXNOTEMPTY) == RESET)
					&& TIME_OUT())
			;
			*buff_point = I2C_ReceiveData();
			break;
			case 2:
			I2C_AcknowledgeConfig (I2C_ACK_NEXT);
			disableInterrupts();
			I2C->SR3;
			I2C_AcknowledgeConfig(I2C_ACK_NONE);
			enableInterrupts();
			RELOAD_TIME_COUNT();
			while ((I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET)
					&& TIME_OUT())
			;
			disableInterrupts();
			I2C_GenerateSTOP(ENABLE);
			*buff_point++ = I2C_ReceiveData();
			enableInterrupts();
			*buff_point = I2C_ReceiveData();
			break;
			default:
			I2C->SR3;
			RELOAD_TIME_COUNT();
			while ((read_len > 3) && TIME_OUT()) {
				RELOAD_TIME_COUNT();
				while ((I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET)
						&& TIME_OUT())
				;
				*buff_point++ = I2C_ReceiveData();
				--read_len;
			}
			RELOAD_TIME_COUNT();
			while ((I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET)
					&& TIME_OUT())
			;
			I2C_AcknowledgeConfig(I2C_ACK_NONE);
			disableInterrupts();
			*buff_point++ = I2C_ReceiveData();
			I2C_GenerateSTOP(ENABLE);
			*buff_point++ = I2C_ReceiveData();
			RELOAD_TIME_COUNT();
			enableInterrupts();
			while ((I2C_GetFlagStatus(I2C_FLAG_RXNOTEMPTY) == RESET)
					&& TIME_OUT())
			;
			*buff_point++ = I2C->DR;
			break;
		}
	}

	/*--------------- All Data Received -----------------------*/
	RELOAD_TIME_COUNT();
	while ((I2C->CR2 & I2C_CR2_STOP) && TIME_OUT())
	;
#else
	START_TIME_COUNT();
	SET_COMMUNICATIO_STATE(I2C_BUYS);
	/*--------------- Wait I2C bus free -------------------------*/
	while ((I2C->SR3 & I2C_SR3_BUSY) && TIME_OUT()) {
		I2C->CR2 |= I2C_CR2_STOP;
	}

	I2C->CR2 |= I2C_CR2_ACK;
	/*--------------- Start communication -----------------------*/
	I2C->CR2 |= I2C_CR2_START;

	//reload time count
	RELOAD_TIME_COUNT();
	//save communication state when define DEBUG
	SET_COMMUNICATIO_STATE(I2C_RESPONSE);
	while (((I2C->SR1 & I2C_SR1_SB) == 0) && TIME_OUT())
		;
	/*------------------ Address send ---------------------------*/

	I2C->DR = (slave_address << 1);

	RELOAD_TIME_COUNT();
	SET_COMMUNICATIO_STATE(I2C_RESPONSE);
	while ((!(I2C->SR1 & I2C_SR1_ADDR)) && TIME_OUT())
		;

	I2C->SR3;

	RELOAD_TIME_COUNT();
	while ((!(I2C->SR1 & I2C_SR1_TXE)) && TIME_OUT())
		;
	/*--------------- Register/Command send ---------------------*/
	I2C->DR = reg_address;

	RELOAD_TIME_COUNT();
	while (((I2C->SR1 & (I2C_SR1_TXE | I2C_SR1_BTF))
			!= (I2C_SR1_TXE | I2C_SR1_BTF)) && TIME_OUT())
		;

	/*--------------- Restart communication ---------------------*/
	I2C->CR2 |= I2C_CR2_START;

	RELOAD_TIME_COUNT();
	while (((I2C->SR1 & I2C_SR1_SB) == 0) && TIME_OUT())
		;
	// Wait for start bit detection (SB)
	/*------------------ Address send ---------------------------*/
	I2C->DR = (slave_address << 1) | 1;

	RELOAD_TIME_COUNT();
	SET_COMMUNICATIO_STATE(I2C_COMMUNICATIO);
	while ((!(I2C->SR1 & I2C_SR1_ADDR)) && TIME_OUT())
		;
	/*------------------- Data Receive --------------------------*/
	if (TIME_OUT()) {
		//data lenght
		switch (read_len) {
		case 1:
			I2C->CR2 &= ~I2C_CR2_ACK;
			disableInterrupts();
			// Clear ADDR Flag
			I2C->SR3;
			I2C->CR2 |= I2C_CR2_STOP;
			RELOAD_TIME_COUNT();
			enableInterrupts();
			while ((!(I2C->SR1 & I2C_SR1_RXNE)) && TIME_OUT())
				;
			*buff_point = I2C->DR;
			break;
		case 2:
			I2C->CR2 |= I2C_CR2_POS;
			disableInterrupts();
			// Clear ADDR Flag
			I2C->SR3;
			I2C->CR2 &= ~I2C_CR2_ACK;
			RELOAD_TIME_COUNT();
			enableInterrupts();
			while ((!(I2C->SR1 & I2C_SR1_BTF)) && TIME_OUT())
				;
			disableInterrupts();
			I2C_GenerateSTOP (ENABLE);
			// Read 1st Data byte
			*buff_point++ = I2C->DR;
			enableInterrupts();
			// Read 2nd Data byte
			*buff_point = I2C->DR;
			break;
		default:
			// ADDR clearing sequence
			I2C->SR3;
			RELOAD_TIME_COUNT();
			// not last three bytes?
			while ((read_len > 3) && TIME_OUT()) {
				RELOAD_TIME_COUNT();
				while ((!(I2C->SR1 & I2C_SR1_BTF)) && TIME_OUT())
					;
				// Reading next data byte
				*buff_point++ = I2C->DR;
				--read_len;
			}
			RELOAD_TIME_COUNT();
			while ((!(I2C->SR1 & I2C_SR1_BTF)) && TIME_OUT())
				;
			I2C->CR2 &= ~I2C_CR2_ACK;
			disableInterrupts();
			// Read 1st byte
			*buff_point++ = I2C->DR;
			I2C->CR2 |= I2C_CR2_STOP;
			// Read 2nd byte
			*buff_point++ = I2C->DR;
			enableInterrupts();
			RELOAD_TIME_COUNT();
			while ((!(I2C->SR1 & I2C_SR1_RXNE)) && TIME_OUT())
				;
			// Read 3th byte
			*buff_point++ = I2C->DR;
			break;
		}
	}
	/*--------------- All Data Received -----------------------*/
	RELOAD_TIME_COUNT();
	while ((I2C->CR2 & I2C_CR2_STOP) && TIME_OUT())
		;
	I2C->CR2 &= ~I2C_CR2_POS;

#endif

#if defined DEBUG || defined I2C_AUTO_RELEASE_DEADLOCK
	if (!TIME_OUT()) {
#if defined DEBUG
		Printf_I2C_Error();
#endif
#if defined I2C_AUTO_RELEASE_DEADLOCK
		I2C_Release_deadlock();
#endif
	}
#endif

	return (TIME_OUT() ? 0 : -1);
}
/**
 * @brief  multiple read I2C by Poll in interrupt.
 * @param  slave_address : slave address.
 * @param  reg_address : register address.
 * @param  buff_point ： want to write buffer point
 * @param  read_len : read lenght
 * @retval communication state 0 is ok
 */
uint8_t I2C_Multiple_Read_Poll_In_Interrupt(uint8_t slave_address,
		uint8_t reg_address, uint8_t *buff_point, uint8_t read_len) {
#ifdef USE_LIB
	START_TIME_COUNT();
	SET_COMMUNICATIO_STATE(I2C_BUYS);

	/*--------------- Wait I2C bus free -------------------------*/
	while ((I2C_GetFlagStatus(I2C_FLAG_BUSBUSY) == SET) && TIME_OUT()) {
		I2C_GenerateSTOP (ENABLE);
	}

	I2C_AcknowledgeConfig (I2C_ACK_CURR);

	/*--------------- Start communication -----------------------*/
	I2C_GenerateSTART (ENABLE);

	RELOAD_TIME_COUNT();
	SET_COMMUNICATIO_STATE(I2C_RESPONSE);
	while ((I2C_GetFlagStatus(I2C_FLAG_STARTDETECTION) == RESET) && TIME_OUT())
	;

	/*------------------ Address send ---------------------------*/
	I2C_Send7bitAddress(slave_address << 1, I2C_DIRECTION_TX);

	RELOAD_TIME_COUNT();
	SET_COMMUNICATIO_STATE(I2C_SLAVE_FINED);
	while ((I2C_GetFlagStatus(I2C_FLAG_ADDRESSSENTMATCHED) == RESET)
			&& TIME_OUT())
	;

	I2C->SR3;

	RELOAD_TIME_COUNT();
	while ((I2C_GetFlagStatus(I2C_FLAG_TXEMPTY) == RESET) && TIME_OUT())
	;

	/*--------------- Register/Command send ---------------------*/
	I2C_SendData(reg_address);

	RELOAD_TIME_COUNT();
	while ((I2C_GetFlagStatus(I2C_FLAG_TXEMPTY) == RESET) && TIME_OUT())
	;

	/*--------------- Restart communication ---------------------*/
	I2C_GenerateSTART(ENABLE);

	RELOAD_TIME_COUNT();
	while ((I2C_GetFlagStatus(I2C_FLAG_STARTDETECTION) == RESET) && TIME_OUT())
	;

	/*------------------ Address send ---------------------------*/
	I2C_Send7bitAddress(slave_address << 1, I2C_DIRECTION_RX);

	RELOAD_TIME_COUNT();
	SET_COMMUNICATIO_STATE(I2C_COMMUNICATIO);
	while ((I2C_GetFlagStatus(I2C_FLAG_ADDRESSSENTMATCHED) == RESET)
			&& TIME_OUT())
	;

	/*------------------- Data Receive --------------------------*/
	if (TIME_OUT()) {
		switch (read_len) {
			case 1:
			I2C_AcknowledgeConfig (I2C_ACK_NONE);
			I2C->SR3;
			I2C_GenerateSTOP(ENABLE);
			RELOAD_TIME_COUNT();
			while ((I2C_GetFlagStatus(I2C_FLAG_RXNOTEMPTY) == RESET)
					&& TIME_OUT())
			;
			*buff_point = I2C_ReceiveData();
			break;
			case 2:
			I2C_AcknowledgeConfig (I2C_ACK_NEXT);
			I2C->SR3;
			I2C_AcknowledgeConfig(I2C_ACK_NONE);
			RELOAD_TIME_COUNT();
			while ((I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET)
					&& TIME_OUT())
			;
			I2C_GenerateSTOP(ENABLE);
			*buff_point++ = I2C_ReceiveData();
			*buff_point = I2C_ReceiveData();
			break;
			default:
			I2C->SR3;
			RELOAD_TIME_COUNT();
			while ((read_len > 3) && TIME_OUT()) {
				RELOAD_TIME_COUNT();
				while ((I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET)
						&& TIME_OUT())
				;
				*buff_point++ = I2C_ReceiveData();
				--read_len;
			}
			RELOAD_TIME_COUNT();
			while ((I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET)
					&& TIME_OUT())
			;
			I2C_AcknowledgeConfig(I2C_ACK_NONE);
			*buff_point++ = I2C_ReceiveData();
			I2C_GenerateSTOP(ENABLE);
			*buff_point++ = I2C_ReceiveData();
			RELOAD_TIME_COUNT();
			while ((I2C_GetFlagStatus(I2C_FLAG_RXNOTEMPTY) == RESET)
					&& TIME_OUT())
			;
			*buff_point++ = I2C->DR;
		}
	}

	/*--------------- All Data Received -----------------------*/
	RELOAD_TIME_COUNT();
	while ((I2C->CR2 & I2C_CR2_STOP) && TIME_OUT())
	;    		// Wait until stop is performed (STOPF = 1)
#else
	START_TIME_COUNT();
	SET_COMMUNICATIO_STATE(I2C_BUYS);

	/*--------------- Wait I2C bus free -------------------------*/
	while ((I2C->SR3 & I2C_SR3_BUSY) && TIME_OUT()) {
		I2C->CR2 |= I2C_CR2_STOP;
	}

	I2C->CR2 |= I2C_CR2_ACK;

	/*--------------- Start communication -----------------------*/
	I2C->CR2 |= I2C_CR2_START;

	RELOAD_TIME_COUNT();
	SET_COMMUNICATIO_STATE(I2C_RESPONSE);
	while (((I2C->SR1 & I2C_SR1_SB) == 0) && TIME_OUT())
		;
	/*------------------ Address send ---------------------------*/

	I2C->DR = (slave_address << 1);

	RELOAD_TIME_COUNT();
	SET_COMMUNICATIO_STATE(I2C_SLAVE_FINED);
	while ((!(I2C->SR1 & I2C_SR1_ADDR)) && TIME_OUT())
		;

	I2C->SR3;
	RELOAD_TIME_COUNT();
	while ((!(I2C->SR1 & I2C_SR1_TXE)) && TIME_OUT())
		;

	/*--------------- Register/Command send ---------------------*/
	I2C->DR = reg_address;

	RELOAD_TIME_COUNT();
	while (((I2C->SR1 & (I2C_SR1_TXE | I2C_SR1_BTF))
			!= (I2C_SR1_TXE | I2C_SR1_BTF)) && TIME_OUT())
		;

	/*--------------- Restart communication ---------------------*/
	I2C->CR2 |= I2C_CR2_START;
	RELOAD_TIME_COUNT();
	while (((I2C->SR1 & I2C_SR1_SB) == 0) && TIME_OUT())
		;
	/*------------------ Address send ---------------------------*/
	I2C->DR = (slave_address << 1) | 1;

	RELOAD_TIME_COUNT();
	SET_COMMUNICATIO_STATE(I2C_COMMUNICATIO);
	while ((!(I2C->SR1 & I2C_SR1_ADDR)) && TIME_OUT())
		;
	/*------------------- Data Receive --------------------------*/
	if (TIME_OUT()) {
		switch (read_len) {
		case 1:
			I2C->CR2 &= ~I2C_CR2_ACK;
			// Clear ADDR Flag
			I2C->SR3;
			I2C->CR2 |= I2C_CR2_STOP;
			RELOAD_TIME_COUNT();
			while ((!(I2C->SR1 & I2C_SR1_RXNE)) && TIME_OUT())
				;
			// Read Data byte
			*buff_point = I2C->DR;
			break;
		case 2:
			I2C->CR2 |= I2C_CR2_POS;
			// Clear ADDR Flag
			I2C->SR3;
			I2C->CR2 &= ~I2C_CR2_ACK;
			RELOAD_TIME_COUNT();
			while ((!(I2C->SR1 & I2C_SR1_BTF)) && TIME_OUT())
				;
			I2C_GenerateSTOP (ENABLE);
			// Read 1st Data byte
			*buff_point++ = I2C->DR;
			// Read 2nd Data byte
			*buff_point = I2C->DR;
			break;
		default:
			// ADDR clearing sequence
			I2C->SR3;
			// not last three bytes?
			while (read_len > 3 && TIME_OUT()) {
				RELOAD_TIME_COUNT();
				while ((!(I2C->SR1 & I2C_SR1_BTF)) && TIME_OUT())
					;
				// Reading next data byte
				*buff_point++ = I2C->DR;
				--read_len;
			}
			//last three bytes should be read
			RELOAD_TIME_COUNT();
			while ((!(I2C->SR1 & I2C_SR1_BTF)) && TIME_OUT())
				; // Wait for BTF
			I2C->CR2 &= ~I2C_CR2_ACK;
			// Read 1st byte
			*buff_point++ = I2C->DR;
			I2C->CR2 |= I2C_CR2_STOP;
			// Read 2nd byte
			*buff_point++ = I2C->DR;
			RELOAD_TIME_COUNT();
			while ((!(I2C->SR1 & I2C_SR1_RXNE)) && TIME_OUT())
				;
			// Read 3th byte
			*buff_point++ = I2C->DR;
			break;
		}
	}
	/*--------------- All Data Received -----------------------*/
	RELOAD_TIME_COUNT();
	while ((I2C->CR2 & I2C_CR2_STOP) && TIME_OUT())
		;
#endif

#if defined DEBUG || defined I2C_AUTO_RELEASE_DEADLOCK
	if (!TIME_OUT()) {
#if defined DEBUG
		Printf_I2C_Error();
#endif
#if defined I2C_AUTO_RELEASE_DEADLOCK
		I2C_Release_deadlock();
#endif
	}
#endif

	return (TIME_OUT() ? 0 : -1);
}
/**
 * @brief  multiple write I2C by Poll.
 * @param  slave_address : slave address.
 * @param  reg_address : register address.
 * @param  buff_point ： want to read buffer point
 * @param  data_len : write data lenght
 * @retval communication state 0 is ok
 */
uint8_t I2C_Multiple_Write_Poll(uint8_t slave_address, uint8_t reg_address,
		uint8_t *buff_point, uint8_t data_len) {
#ifdef USE_LIB
	START_TIME_COUNT();
	SET_COMMUNICATIO_STATE(I2C_BUYS);

	/*--------------- Wait I2C bus free -------------------------*/
	while ((I2C_GetFlagStatus(I2C_FLAG_BUSBUSY) == SET) && TIME_OUT()) {
		I2C_GenerateSTOP (ENABLE);
	}

	/*--------------- Start communication -----------------------*/
	I2C_GenerateSTART (ENABLE);

	RELOAD_TIME_COUNT();
	SET_COMMUNICATIO_STATE(I2C_RESPONSE);
	while ((I2C_GetFlagStatus(I2C_FLAG_STARTDETECTION) == RESET) && TIME_OUT())
	;

	/*------------------ Address send ---------------------------*/
	I2C_Send7bitAddress(slave_address << 1, I2C_DIRECTION_TX);

	RELOAD_TIME_COUNT();
	SET_COMMUNICATIO_STATE(I2C_SLAVE_FINED);
	while ((I2C_GetFlagStatus(I2C_FLAG_ADDRESSSENTMATCHED) == RESET)
			&& TIME_OUT())
	;

	I2C->SR3;

	RELOAD_TIME_COUNT();
	while ((I2C_GetFlagStatus(I2C_FLAG_TXEMPTY) == RESET) && TIME_OUT())
	;

	/*--------------- Register/Command send ---------------------*/
	I2C_SendData(reg_address);

	/*------------------- Send Receive --------------------------*/
	RELOAD_TIME_COUNT();
	SET_COMMUNICATIO_STATE(I2C_COMMUNICATIO);
	if ((data_len) &&TIME_OUT()) {
		while (data_len-- && TIME_OUT()) {
			RELOAD_TIME_COUNT();
			while ((I2C_GetFlagStatus(I2C_FLAG_TXEMPTY) == RESET) && TIME_OUT())
			;
			I2C->DR = *buff_point++;
		}
	}

	RELOAD_TIME_COUNT();
	while ((I2C_GetFlagStatus(I2C_FLAG_TXEMPTY) == RESET) && TIME_OUT())
	;

	/*--------------- Finish communication ----------------------*/
	I2C_GenerateSTOP(ENABLE);

	RELOAD_TIME_COUNT();
	while ((I2C->CR2 & I2C_CR2_STOP) && TIME_OUT())
	;
#else
	START_TIME_COUNT();
	SET_COMMUNICATIO_STATE(I2C_BUYS);
	/*--------------- Wait I2C bus free -------------------------*/
	while ((I2C->SR3 & I2C_SR3_BUSY) && TIME_OUT())
		;

	/*--------------- Start communication -----------------------*/
	I2C->CR2 |= I2C_CR2_START;
	RELOAD_TIME_COUNT();

	while (((I2C->SR1 & I2C_SR1_SB) == 0) && TIME_OUT())
		;

	/*------------------ Address send ---------------------------*/
	I2C->DR = (slave_address << 1);

	RELOAD_TIME_COUNT();
	while ((!(I2C->SR1 & I2C_SR1_ADDR)) && TIME_OUT())
		;
	I2C->SR3;

	RELOAD_TIME_COUNT();
	while ((!(I2C->SR1 & I2C_SR1_TXE)) && TIME_OUT())
		;
	I2C->DR = reg_address;

	// write data loop start
	if (data_len) {
		while (data_len--) {
			RELOAD_TIME_COUNT();
			while ((!(I2C->SR1 & I2C_SR1_TXE)) && TIME_OUT())
				;
			I2C->DR = *buff_point++;
		}
	}

	RELOAD_TIME_COUNT();
	while (((I2C->SR1 & (I2C_SR1_BTF | I2C_SR1_TXE))
			!= (I2C_SR1_BTF | I2C_SR1_TXE)) && TIME_OUT())
		;

	I2C->CR2 |= I2C_CR2_STOP;
	while ((I2C->CR2 & I2C_CR2_STOP) && TIME_OUT())
		;
#endif

#if defined DEBUG || defined I2C_AUTO_RELEASE_DEADLOCK
	if (!TIME_OUT()) {
#if defined DEBUG
		Printf_I2C_Error();
#endif
#if defined I2C_AUTO_RELEASE_DEADLOCK
		I2C_Release_deadlock();
#endif
	}
#endif

	return (TIME_OUT() ? 0 : -1);
}

/**
 * @brief release I2C dead lock
 */
void I2C_Release_deadlock() {
	uint8_t time = 255;
	I2C_DeInit();
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
	GPIO_Init(I2C_SDA_GPIO, I2C_SDA_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(I2C_SCL_GPIO, I2C_SCL_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	while (GPIO_ReadInputPin(I2C_SDA_GPIO, I2C_SDA_PIN) == RESET && --time) {
		GPIO_WriteReverse(I2C_SCL_GPIO, I2C_SCL_PIN);
		Delay_1_ms();
	}
#ifndef AUTO_RESET_WHEN_I2C_DEAD_LOCK
	if(!time) {
#endif
	IWDG_Enable();

	IWDG_WriteAccessCmd (IWDG_WriteAccess_Enable);

	IWDG_SetPrescaler (IWDG_Prescaler_4);

	IWDG_WriteAccessCmd (IWDG_WriteAccess_Disable);
#if defined DEBUG
	printf("Waiting restart!!! \n\r\n");
#endif
	while (TRUE)
		;
#ifndef AUTO_RESET_WHEN_I2C_DEAD_LOCK
}
#endif
}

/**
 * @brief Timer Count
 * @return time out state : 0 is time out
 */
uint8_t TIME_OUT() {
	if (g_TIME_COUNT)
		return --g_TIME_COUNT;
	else
		return 0;
}
/**
 * @brief Reload Timer Count
 */
void RELOAD_TIME_COUNT() {
	if (g_TIME_COUNT)
		g_TIME_COUNT = 255;
}
#if defined DEBUG
/**
 * @brief Printf I2C Error Message
 */
void Printf_I2C_Error() {
	if (!g_I2C_CONFIG_STATE) {
		printf("I2C ERROR:I2C NOT CONFIG \n\r\n");
		return;
	}
	switch (g_COMMUNICATIO_STATE) {
	case I2C_BUYS:
		printf("I2C ERROR:I2C WAS BUSY,PLEASE CHECK LINE COMMUNITION! \n\r\n");
		break;
	case I2C_RESPONSE:
		printf(
				"I2C ERROR:I2C WAS NO RESPONSE,PLEASE CHECK LINE COMMUNITION! \n\r\n");
		break;
	case I2C_SLAVE_FINED:
		printf(
				"I2C ERROR:I2C NO FINE SLAVE,PLEASE CHECK SLAVE ADDRESS! \n\r\n");
		break;
	case I2C_COMMUNICATIO:
		printf("I2C ERROR:I2C COMUNITION TIME OUT \n\r\n");
		break;
	}
}
#endif
