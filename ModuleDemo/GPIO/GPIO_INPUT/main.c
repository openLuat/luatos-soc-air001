#include "main.h"
static void RCC_Configuration(void);         // 配置时钟
static void GPIO_Configuration(void);        // 配置GPIO
LL_UTILS_ClkInitTypeDef UTILS_ClkInitStruct; // 系统时钟结构体

// 外部中断示例代码，使用GPIOF4作为外部中断输入，上升沿触发中断，中断服务函数中将GPIOB0输出反转

int main(void)
{
  // 配置时钟
  RCC_Configuration();
  // 配置GPIO
  GPIO_Configuration();
  while (1)
  {
  }
}
// 配置时钟
static void RCC_Configuration(void)
{
  LL_RCC_HSI_Enable();                                  // 使能HSI
  LL_RCC_HSI_SetCalibFreq(LL_RCC_HSICALIBRATION_24MHz); // 设置HSI为24MHz
  while (LL_RCC_HSI_IsReady() != 1)                     // 等待HSI就绪
  {
  }

  LL_PLL_ConfigSystemClock_HSI(&UTILS_ClkInitStruct); // 配置HSI为系统时钟

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1); // 设置AHB分频

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);                   // 设置系统时钟源为PLL
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) // 等待系统时钟源切换为PLL
  {
  }

  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1); // 设置APB1分频

  SystemCoreClockUpdate(); // 更新系统时钟，更新后的系统时钟存放在SystemCoreClock中
}
// 初始化GPIO
static void GPIO_Configuration(void)
{
  // 使能GPIOB时钟
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);
  // 开发板上中间的灯连接在PB0，初始化PB0为推挽输出
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
  // 初始化PF4为输入，外部有下拉电阻
  LL_GPIO_SetPinMode(GPIOF, LL_GPIO_PIN_4, LL_GPIO_MODE_INPUT);

  LL_EXTI_InitTypeDef EXTI_InitStruct;              // 外部中断结构体
  EXTI_InitStruct.Line = LL_EXTI_LINE_4;            // 外部中断线
  EXTI_InitStruct.LineCommand = ENABLE;             // 使能外部中断线
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;           // 中断模式
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING; // 上升沿触发
  LL_EXTI_Init(&EXTI_InitStruct);                   // 初始化外部中断
  LL_EXTI_EnableIT(LL_EXTI_LINE_4);                 // 使能外部中断

  LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTF, LL_EXTI_CONFIG_LINE4); // 设置外部中断线的源为PF4
  NVIC_SetPriority(EXTI4_15_IRQn, 0);                                // 设置中断优先级
  NVIC_EnableIRQ(EXTI4_15_IRQn);                                     // 使能中断
}
// 外部中断服务函数
void EXTI4_15_IRQHandler(void)
{
  if (LL_EXTI_IsActiveFlag(LL_EXTI_LINE_4)) // 判断中断来源
  {
    LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_0); // 翻转LED
    LL_EXTI_ClearFlag(LL_EXTI_LINE_4);       // 清空中断标志
  }
}
// 以下为错误处理函数
void APP_ErrorHandler(void)
{
  // 死等
  while (1)
  {
  }
}
