/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_gpio.h"
#include "stm8s_clk.h"
#include "stm8s_exti.h"
#include "stm8s_tim2.h"

#include "uart.h"

/* define --------------------------------------------------------------------*/
#define SCR_CONTROL_PORT                                        GPIOD
#define ZERO_DETECT_PORT                                        GPIOD
#define SCR_CONTROL_PIN					        GPIO_PIN_2
#define ZERO_DETECT_PIN 					GPIO_PIN_3
#define	SCECLOSETIMEMAXVALUE					7500
/* variables -----------------------------------------------------------------*/
uint8_t ScrCloseTimeValue	= 50;				//unit : 99us.range:0~100(>=0 ,<=100);
uint32_t TimerValue;
/**
  * @brief None.
  * @param  None
  * @retval None
  */
void SetLampLightValue( uint8_t LightValue )
{
    if( ( LightValue >= 0 ) && ( LightValue <= 100 ) )
    {
        ScrCloseTimeValue = 100 - LightValue;
    }
}
/**
  * @brief None.
  * @param  None
  * @retval None
  */
void LampAdjInit( void )
{
    GPIO_Init( SCR_CONTROL_PORT , SCR_CONTROL_PIN , GPIO_MODE_OUT_OD_LOW_FAST ); //SCR Pin.
    GPIO_Init( ZERO_DETECT_PORT , ZERO_DETECT_PIN , GPIO_MODE_IN_PU_IT );        //ZERO Detect Pin.
    
      /* Initialize the Interrupt sensitivity */
    EXTI_SetExtIntSensitivity( EXTI_PORT_GPIOD , EXTI_SENSITIVITY_RISE_FALL );
    
    TIM2_TimeBaseInit( TIM2_PRESCALER_8 , 0 );
    TIM2_ARRPreloadConfig( DISABLE );
    TIM2_ClearITPendingBit( TIM2_IT_UPDATE );
    TIM2_ITConfig( TIM2_IT_UPDATE , ENABLE );
    ITC_SetSoftwarePriority( ITC_IRQ_TIM2_OVF , ITC_PRIORITYLEVEL_2 );               //Interrput priority is 2.
    TIM2_Cmd( DISABLE );
}
/**
  * @brief None.
  * @param  None
  * @retval None
  */
void ZeroCrossingDetectHandler( void )
{
    if( GPIO_ReadInputPin( ZERO_DETECT_PORT , ZERO_DETECT_PIN ) != RESET )                   // rising edge.
    {
        if( ( ScrCloseTimeValue > 0 ) && ( ScrCloseTimeValue < 100 ) )
        {
            TimerValue = (uint32_t)(ScrCloseTimeValue )* SCECLOSETIMEMAXVALUE;
            TimerValue =  TimerValue / 100;
            
            TIM2_SetCounter( 0 );
            TIM2_SetAutoreload( TimerValue );
            TIM2_Cmd( ENABLE );
        }
        else if( ScrCloseTimeValue == 0 )
        {
            GPIO_WriteLow( SCR_CONTROL_PORT , SCR_CONTROL_PIN );
        }
        
//        uart_sendbyte( 0xAA );
    }
    else                                                                        // falling edge.
    {
        GPIO_WriteHigh( SCR_CONTROL_PORT , SCR_CONTROL_PIN );
        
//       uart_sendbyte( 0xBB );
    }
}
/**
  * @brief None.
  * @param  None
  * @retval None
  */
void Timer2UpdateIntHandl( void )
{
    GPIO_WriteLow( SCR_CONTROL_PORT , SCR_CONTROL_PIN );
    
    while( TIM2_GetCounter( ) < 30 );
    
    GPIO_WriteHigh( SCR_CONTROL_PORT , SCR_CONTROL_PIN );
    
    TIM2_Cmd( DISABLE );
    
    TIM2_ClearITPendingBit( TIM2_IT_UPDATE );
    
//    uart_sendbyte( 0xCC );
}
