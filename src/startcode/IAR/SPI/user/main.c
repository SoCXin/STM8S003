/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_clk.h"
#include "uart.h"
#include "stdio.h"
#include "stm8s_spi.h"
#include "LampAdj.h"
#include "LedPwm.h"
#include "Led.h"
/* Private variables ---------------------------------------------------------*/
uint8_t Data;
uint8_t SpiRegRefreshFlag = 0;
uint8_t SpiRegRefreshAddr = 0x00;

uint8_t SpiVirtualReg[5] = { 0x00 , 0x32 , 0x00 , 0x00 , 0x55 };

    uint16_t OPT2_Value;
    
int putchar (int c);

void delay(unsigned int x)
{
  unsigned int i=0,j=0;
  
  for(i=0;i<x;i++)
  {
    for(j=0;j<499;j++)
    {
      
    }
  }
}

void CLK_Config(void)
{
    CLK_DeInit();
    CLK_HSIPrescalerConfig( CLK_PRESCALER_HSIDIV2 );             // CPU Clock : 8MHz.
    
#if 0    
    CLK_HSECmd( ENABLE );
    CLK_ClockSwitchCmd( ENABLE );
    CLK_SYSCLKConfig( CLK_PRESCALER_CPUDIV2 );
    CLK_ClockSwitchConfig( CLK_SWITCHMODE_AUTO ,CLK_SOURCE_HSE,DISABLE ,CLK_CURRENTCLOCKSTATE_DISABLE );
    CLK_ClockSecuritySystemEnable( );
#endif
    
    CLK_PeripheralClockConfig( CLK_PERIPHERAL_SPI,ENABLE );       //SPI时钟使能
    CLK_PeripheralClockConfig( CLK_PERIPHERAL_TIMER2,ENABLE );
    CLK_PeripheralClockConfig( CLK_PERIPHERAL_TIMER1,ENABLE );
}

void Uart_Init(void)
{
  UART1_DeInit();
  UART1_Init((uint32_t)115200,UART1_WORDLENGTH_8D,UART1_STOPBITS_1,
  UART1_PARITY_NO,UART1_SYNCMODE_CLOCK_DISABLE,UART1_MODE_TX_ENABLE );
  UART1_Cmd(ENABLE);
}

static void SpiInit( void )
{
    SPI_Init( SPI_FIRSTBIT_MSB , SPI_BAUDRATEPRESCALER_8 , SPI_MODE_SLAVE ,
             SPI_CLOCKPOLARITY_HIGH , SPI_CLOCKPHASE_2EDGE ,SPI_DATADIRECTION_2LINES_FULLDUPLEX , SPI_NSS_HARD , 0x07 );
    
    SPI_CalculateCRCCmd( ENABLE );
      
    SPI_Cmd( ENABLE );
    
    SPI_ITConfig( SPI_IT_RXNE , ENABLE );
    
    ITC_SetSoftwarePriority( ITC_IRQ_SPI , ITC_PRIORITYLEVEL_3 );               //Interrput priority is 3.
}


/*发送哟个字节，查询发送。当发送寄存器为空即发送*/
void UART1_SendByte(uint8_t data)
{
    while(UART1_GetFlagStatus( UART1_FLAG_TXE ) == RESET );
    UART1_SendData8(data);
}

int main( void )
{
    uint32_t CycleCount = 0;
    
    CLK_Config( );
    
    OPT2_Value = FLASH_ReadOptionByte( (uint16_t )&OPT->OPT2 );
    
    if( ( OPT2_Value & 0x0202 ) != 0x0200 )
    {
        FLASH_Unlock( FLASH_MEMTYPE_DATA );
        FLASH_ProgramOptionByte( (uint16_t )&OPT->OPT2 , 0x02 );                //Port A3 alternate function = SPI_NSS.
    }
    
    Uart_Init( );
    
    LampAdjInit( );
    LedPwmInit( );
    LedInit( );
    SpiInit( );
    enableInterrupts();

    while(1)
    {
        if( SpiRegRefreshFlag == 1 )
        {
            SpiRegRefreshFlag = 0;
            
            if( SpiRegRefreshAddr == 0x01 )
            {
                SetLampLightValue( SpiVirtualReg[ SpiRegRefreshAddr ] );
                UART1_SendByte( SpiVirtualReg[ SpiRegRefreshAddr ] );
            }
            else if( SpiRegRefreshAddr == 0x02 )
            {
                LedPwmSetDuty( SpiVirtualReg[ SpiRegRefreshAddr ] );
                UART1_SendByte( SpiVirtualReg[ SpiRegRefreshAddr ] );
            }
        }
        
        if( ++ CycleCount >= 100000 )
        {
            CycleCount = 0;
            LedToggle( );
        }
        
    }
  
}
/**
  * @brief None.
  * @param  None
  * @retval None
  */
void SpiIrqHandle( void )
{
    static uint8_t RegAddrFlag = 0;
    static uint8_t RegAddr = 0x00;
    
    Data = SPI_ReceiveData( );
//    UART1_SendByte( Data );
    
    if( RegAddrFlag == 0 )
    {
        uint8_t temp;
        temp = Data & 0x80; 
        
        if( temp == 0x80  )              //Write Operate.
        {
            Data &= 0x7F;
            
            if( Data < 0x05 )
            {
                RegAddr = Data;
            }
            else
            {
                RegAddr = 0x00;
            }
            
            RegAddrFlag = 0x81;
        }
        else                                      //Read Operate.
        {
            Data &= 0x7F;
            
            if( Data < 0x05 )
            {
                SPI_SendData( SpiVirtualReg[ Data ] );
            }
            else
            {
                SPI_SendData( 0x00 );
            }
            
            RegAddrFlag = 0x01;
        }
                   
    }
    else if( RegAddrFlag == 0x81 )
    {
        SpiVirtualReg[ RegAddr ] = Data;
        RegAddrFlag = 0;
        
        SpiRegRefreshAddr = RegAddr;
        SpiRegRefreshFlag = 1;
    }
    else if( RegAddrFlag == 0x01 )
    {
        RegAddrFlag = 0x02;
    }
    else 
    {
        RegAddrFlag = 0;
    }
 
}
/**
  * @brief Retargets the C library printf function to the UART1.
  * @param[in] c Character to send
  * @retval char Character sent
  * @par Required preconditions:
  * - None
  * @par Library functions called:
  * - UART1_GetFlagStatus()
	* - UART1_SendData8()
  */

int putchar (int c)
{

    if (c == '\n')
    {
        /* put '\r' to hardware here */
        /* Wait transmission is completed : otherwise the first data is not sent */
        while (!UART1_GetFlagStatus(UART1_FLAG_TC));
        UART1_SendData8('\r');
        /* Wait transmission is completed */
        while (!UART1_GetFlagStatus(UART1_FLAG_TC));
    }

    /* put c to hardware here */
    /* Wait transmission is completed : otherwise the first data is not sent */
    while (!UART1_GetFlagStatus(UART1_FLAG_TC));
    UART1_SendData8(c);
    /* Wait transmission is completed */
    while (!UART1_GetFlagStatus(UART1_FLAG_TC));


    return (c);
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
