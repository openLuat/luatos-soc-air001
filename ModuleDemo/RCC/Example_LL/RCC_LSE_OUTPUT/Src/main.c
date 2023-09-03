/**
  ******************************************************************************
  * @file    main.c
  * @author  MCU Application Team
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) AirM2M.
  * All rights reserved.</center></h2>
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);
static void APP_McoGpioConfig(void);

/**
  * @brief  应用程序入口函数.
  * @retval int
  */
int main(void)
{
  /* 引脚输出初始化 */
  APP_McoGpioConfig();

  /* MCO输出时钟及分频初始化 */
  LL_RCC_ConfigMCO(LL_RCC_MCO1SOURCE_SYSCLK, LL_RCC_MCO1_DIV_1);
  
  /* LED灯初始化 */
  BSP_LED_Init(LED_RED);
  
  /* 按键初始化 */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
  
  /* 等待按键按下 */
  while (BSP_PB_GetState(BUTTON_KEY) != 1);
  
  /* 时钟初始化,配置系统时钟为LSE */
  APP_SystemClockConfig();
  
  /* 初始化SysTick */
  LL_Init1msTick(LSE_VALUE);

  while (1)
  {
    /* LED灯翻转 */
    BSP_LED_Toggle(LED_RED);
    
    /* 延迟500ms */
    LL_mDelay(500);
  }
}

/**
  * @brief  系统时钟配置函数
  * @param  无
  * @retval 无
  */
static void APP_SystemClockConfig(void)
{
  /* LSI使能及初始化 */
  LL_RCC_LSE_Enable();
  while (LL_RCC_LSE_IsReady() != 1)
  {
  }

  /* 设置AHB分频 */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* 配置LSI为系统时钟及初始化 */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_LSE);
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_LSE)
  {
  }

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  /* 设置APB1分频及初始化 */
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  /* 更新系统时钟全局变量SystemCoreClock(也可以通过调用SystemCoreClockUpdate函数更新) */
  LL_SetSystemCoreClock(LSE_VALUE);
}

/**
  * @brief  配置PA08为MCO复用功能
  * @param  无
  * @retval 无
  */
static void APP_McoGpioConfig(void)
{
  /* GPIOA时钟使能 */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

  /* 配置PA08为复用模式，并复用为MCO输出引脚 */
  LL_GPIO_InitTypeDef GPIO_InitStruct;
  /* 选择8号引脚 */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
  /* 配置为复用模式 */
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  /* 选择复用为AF5功能 */
  GPIO_InitStruct.Alternate = LL_GPIO_AF5_MCO;
  /* 输出速度选择 */
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  /* 输出模式选择 */
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  /* 无上下拉 */
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;

  /* 初始化GPIOA */
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/**
  * @brief  错误执行函数
  * @param  无
  * @retval 无
  */
void Error_Handler(void)
{
  /* 无限循环 */
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  输出产生断言错误的源文件名及行号
  * @param  file：源文件名指针
  * @param  line：发生断言错误的行号
  * @retval 无
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* 用户可以根据需要添加自己的打印信息,
     例如: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* 无限循环 */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT AirM2M *****END OF FILE******************/
