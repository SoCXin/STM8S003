#ifndef APPLICATION_H
#define APPLICATION_H

#include "osal.h"
#include "osal_timer.h"
#include "osal_event.h"
#include "osal_memory.h"
#include "osal_msg.h"

//全局变量声明

//所有任务的任务ID、初始化函数、事件处理函数、任务事件都统一在此文件声明或定义
/*****************************************************************************/

//任务ID声明
extern uint8 Serial_TaskID;
//任务初始化函数声明
extern void Serial_Task_Init(uint8 task_id);
//任务事件处理函数声明
extern uint16 Serial_Task_EventProcess(uint8 task_id,uint16 task_event);

//任务事件定义
//系统消息事件
#define SYS_EVENT_MSG		0x8000

//串口通信任务事件定义
#define	PRINTF_STR			0X0001 		//打印字符串事件
#define LED_FLICKER			0x0002 		//LED闪烁事件
//串口通信任务系统消息事件定义
#define OSAL_PRINTF			0X01		//系统打印

/*****************************************************************************/

#endif
