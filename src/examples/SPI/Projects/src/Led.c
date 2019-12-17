/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_gpio.h"
#include "stm8s_clk.h"
#include "stm8s_exti.h"
#include "stm8s_tim1.h"
/* define --------------------------------------------------------------------*/
#define LED_PORT                                                GPIOD
#define LED_PIN					                GPIO_PIN_4
/* variables -----------------------------------------------------------------*/

/**
  * @brief None.
  * @param  None
  * @retval None
  */
void LedInit( void )
{
    GPIO_Init( LED_PORT , LED_PIN , GPIO_MODE_OUT_PP_LOW_FAST );        //LED PWM Pin.
    GPIO_WriteLow( LED_PORT , LED_PIN );
}
/**
  * @brief None.
  * @param  None
  * @retval None
  */
void LedToggle( void )
{
    GPIO_WriteReverse( LED_PORT , LED_PIN );
}