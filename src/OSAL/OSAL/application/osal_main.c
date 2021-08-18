/****************************************************************************************
 * �ļ���  ��osal_main.c
 * ����    ��osal����ϵͳ��������������������ڴ��ļ������
 * ����ƽ̨��STM8S003F3
 * ��汾  ����
 ***************************************************************************************/
#include "application.h"

int main(void)
{    
	//ϵͳӲ��������ȳ�ʼ��
    CLK_CKDIVR = 0x00;			//ϵͳʱ�Ӳ���Ƶ�������ڲ�����ʱ��16MHz

	//��ֹ�ж�
    HAL_DISABLE_INTERRUPTS();

    //osal����ϵͳ��ʼ��
    osal_init_system();

	//�������
	osal_add_Task(Serial_Task_Init,Serial_Task_EventProcess,1);	
	//��ӵ�����ͳһ���г�ʼ��
	osal_Task_init();
    
	osal_mem_kick();

	//�����ж�
	HAL_ENABLE_INTERRUPTS();	

	//���ó�ʼ�����¼����ϵ����Ҫ�Զ���ѯ�������¼����ڴ����
	// osal_start_reload_timer(Serial_TaskID,LED_FLICKER,1000);

	//����osalϵͳ�������ٷ���
	osal_start_system();
}
