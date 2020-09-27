/**
 ******************************************************************************
 * @file     src/SYSTEM/I2C.h
 * @author   luzhipeng
 * @version  V1.1
 * @date	 6-Dec-2015
 * @brief    This file is functions prototype for I2C peripheral drive.
 ******************************************************************************
 */
#ifndef __I2C_H__
#define __I2C_H__

unsigned char I2C_Config_Poll(unsigned long OutputClockFrequencyHz);

unsigned char I2C_Multiple_Read_Poll(unsigned char slave_address,
		unsigned char reg_address, unsigned char * buff_point,
		unsigned char read_len);

unsigned char I2C_Multiple_Write_Poll(unsigned char slave_address,
		unsigned char reg_address, unsigned char *buff_point,
		unsigned char data_len);

unsigned char I2C_Multiple_Read_Poll_In_Interrupt(unsigned char slave_address,
		unsigned char reg_address, unsigned char *buff_point,
		unsigned char read_len);

#endif
