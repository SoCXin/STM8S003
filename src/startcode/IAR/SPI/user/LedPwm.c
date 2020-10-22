/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_gpio.h"
#include "stm8s_clk.h"
#include "stm8s_exti.h"
#include "stm8s_tim1.h"

/* define --------------------------------------------------------------------*/
#define LED_PWM_PORT                                            GPIOC
#define LED_PWM_PIN					        GPIO_PIN_3
/* variables -----------------------------------------------------------------*/
/**
  * @brief TIM1_CLK : 100KHz,Timer1 Period : 1KHz.
  * @param  None
  * @retval None
  */
void LedPwmInit( void )
{
    GPIO_Init( LED_PWM_PORT , LED_PWM_PIN , GPIO_MODE_OUT_PP_LOW_FAST );        //LED PWM Pin.
    
    TIM1_DeInit( );
    
    TIM1_TimeBaseInit( 80 , TIM1_COUNTERMODE_UP , 99 , 0 );
    TIM1_ARRPreloadConfig( DISABLE );
    
    TIM1_OC3Init( TIM1_OCMODE_PWM1 , TIM1_OUTPUTSTATE_ENABLE , TIM1_OUTPUTNSTATE_DISABLE , 
                  50 , TIM1_OCPOLARITY_HIGH , TIM1_OCNPOLARITY_HIGH , 
                  TIM1_OCIDLESTATE_RESET ,
                  TIM1_OCNIDLESTATE_RESET );
    
    TIM1_Cmd( ENABLE );
    
    /* TIM1 Main Output Enable */
    TIM1_CtrlPWMOutputs( ENABLE );
}
/**
  * @brief  Nono.
  * @param  Duty : 0~99;
  * @retval None
  */
void LedPwmSetDuty( uint8_t Duty )
{
    if( Duty < 100 )
    {
        TIM1_SetCompare3( Duty );
    }
}