#include "main.h"
static void RCC_Configuration(void);         // 配置时钟
static void GPIO_Configuration(void);        // 配置GPIO
LL_UTILS_ClkInitTypeDef UTILS_ClkInitStruct; // 系统时钟结构体

int main(void)
{
  // 配置时钟
  RCC_Configuration();
  // 配置SysTick
  LL_Init1msTick(SystemCoreClock);
  // 配置GPIO
  GPIO_Configuration();

  while (1)
  {
    // 流水灯
    LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_1);
    LL_mDelay(200);
    LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_0);
    LL_mDelay(200);
    LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_3);
    LL_mDelay(200);
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

  // 开发板上的三个灯为PB1，PB0，PB3
  // 分别初始化为推挽输出
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_1, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_3, LL_GPIO_MODE_OUTPUT);

  // 或者用这种方式初始化
  //  LL_GPIO_InitTypeDef GPIO_InitStruct;                                 // GPIO结构体
  //  GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_3; // 配置PB0，PB1，PB3
  //  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;                          // 推挽输出
  //  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;                // 高速
  //  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;                // 推挽
  //  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;                              // 不带上下拉
  //  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);                               // 初始化GPIOB
}
// 以下为错误处理函数
void APP_ErrorHandler(void)
{
  // 死等
  while (1)
  {
  }
}
