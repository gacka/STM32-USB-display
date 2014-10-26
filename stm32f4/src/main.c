
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#include "stm32f4_discovery.h"
#include "lcdst7565r_unicode.h"
//#include "delay.h"
//#include "ascii.h"



static void SystemClock_Config(void);
static void Error_Handler(void);

int main()
{
SystemClock_Config();
HAL_Init();
char tablica[]={0x67,40,34,60,0x0a,0};

  /* Configure LED4, LED3, LED5 and LED6 */
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);

  BSP_LED_Toggle(LED3);
  lcd_init();
  clear_lcd();

  //displayText_GLCD(0,0,"sds",cambria9pt,1,1);
//  rectangle(9,9,40,40,2);
   //rectangle(20,4,40,20,2);
  displayText_unicode(5,50,"STM32F4 USB DISPLAY 0.1u",&leelawadee_7pt,1);
 //displayTextScroll_unicode(20,20,0,60,"PRZEMEK",&cambria12pt,1);
 //displayText_unicode(10, 2,"PRZEMEKabcd",&cambria12pt,1);
 //clear_lcd();
  //displayText_unicode(10, 2,"PRZEMEKabcd",&cambria12pt,1);
  //displayText_GLCD_I(10,10,"aabcdfg",1);
  lcd_updatewholedisplay(0);


  while(1);

 // GPIO_InitTypeDef GPIO_InitStructure;
 // GPIO_InitStructure.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
 // GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
 // GPIO_InitStructure.Pull = GPIO_NOPULL;
 // GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
 // GPIO_InitStructure.Alternate = GPIO_AF2_TIM4;
 // HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);


//SystemInit();



return 0;
}



/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable Power Control clock */
  __PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}


