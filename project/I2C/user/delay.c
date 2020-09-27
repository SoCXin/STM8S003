/*
 ******************************************************************************
 * @file    src\SYSTEM\delay.c
 * @author  bluebear233
 * @version V1.0
 * @date    2015-12-2
 * @brief   This file is functions for system delay
 ******************************************************************************
 */
#include "delay.h"
#include "stm8s.h"

/* Private define -- ---------------------------------------------------------*/
#define IS_CONFIG_DELAY (fac_us?1:0)

/* Private variables ---------------------------------------------------------*/
//us延时倍乘数
volatile static unsigned char fac_us = 0;

/* Public functions _---------------------------------------------------------*/
void Delay_Config(void);
void Delay_us(unsigned short nus);
void Delay_ms(unsigned long nms);

/**
 * @brief Configure delay
 */
void Delay_Config(void) {
	uint32_t ClockFreq = CLK_GetClockFreq();
	uint8_t clk = 0;
	switch (ClockFreq) {
	case 2000000:
		clk = 2;
		break;
	case 4000000:
		clk = 4;
		break;
	case 8000000:
		clk = 8;
		break;
	case 16000000:
		clk = 16;
		break;
	default:
		break;
	}
	if (clk > 16)
		fac_us = (16 - 4) / 4; //24Mhz时,stm8大概19个周期为1us
	else if (clk > 4)
		fac_us = (clk - 4) / 4;
	else
		fac_us = 1;
}

/**
 * @brief delay us
 * @param nus : delay time
 * @note
 * 		延时时间=(fac_us*4+4)*nus*(T)
 * 		其中,T为CPU运行频率(Mhz)的倒数,单位为us.
 * 		准确度:
 * 			92%  @24Mhz
 * 			98%  @16Mhz
 * 			98%  @12Mhz
 * 			86%  @8Mhz
 */
void Delay_us(unsigned short nus) {
	/*----------------------*/
	/* Check the parameters */
	/*----------------------*/
	assert_param(IS_CONFIG_DELAY);

	__asm(
			"PUSH A          \n"  //1T,压栈
			"DELAY_XUS:      \n"
			"LD A,fac_us     \n"//1T,fac_us加载到累加器A
			"DELAY_US_1:     \n"
			"NOP             \n"//1T,nop延时
			"DEC A           \n"//1T,A--
			"JRNE DELAY_US_1 \n"//不等于0,则跳转(2T)到DELAY_US_1继续执行,若等于0,则不跳转(1T).
			"NOP             \n"//1T,nop延时
			"DECW X          \n"//1T,x--
			"JRNE DELAY_XUS  \n"//不等于0,则跳转(2T)到DELAY_XUS继续执行,若等于0,则不跳转(1T).
			"POP A           \n"//1T,出栈
	);
}
/**
 * @brief delay ms
 * @param nms : delay time
 * @note
 * 		为保证准确度,nms不要大于16640.
 */
void Delay_ms(unsigned long nms) {
	u8 t;

	/*----------------------*/
	/* Check the parameters */
	/*----------------------*/
	assert_param(IS_CONFIG_DELAY);

	if (nms > 65) {
		t = nms / 65;
		while (t--)
			Delay_us(65000);
		nms = nms % 65;
	}
	Delay_us(nms * 1000);
}
