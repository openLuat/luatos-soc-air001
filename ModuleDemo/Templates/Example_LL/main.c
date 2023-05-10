#include "main.h"

static void SystemClock_Config(void);

int main(void)
{
  /* 开SYSCFG和PWR时钟 */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  // 配置时钟
  SystemClock_Config();

  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB); // 使能GPIOB时钟
  // 将PB0引脚配置为输出
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);

  while (1)
  {
    LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_0);
    LL_mDelay(1000);
  }
}

static void SystemClock_Config(void)
{
  // 使能HSI
  LL_RCC_HSI_Enable();
  while (LL_RCC_HSI_IsReady() != 1)
  {
  }

  // 打开AHB时钟
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  // 使用HSI作为系统时钟
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSISYS);
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSISYS)
  {
  }

  // 设置APB1时钟
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_Init1msTick(8000000);

  // 设置系统时钟
  LL_SetSystemCoreClock(8000000);
}

void Error_Handler(void)
{
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{

  while (1)
  {
  }
}
#endif
