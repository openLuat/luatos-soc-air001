#include "main.h"
static void RCC_Configuration(void);   // 配置时钟
static void GPIO_Configuration(void);  // 配置GPIO
static void LPTIM_Configuration(void); // 配置LPTIM


int main(void)
{
  /* 配置系统时钟 */
  RCC_Configuration();

  /* 使能LPTIM、PWR时钟 */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_LPTIM1);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  // GPIO初始化
  GPIO_Configuration();
  /* 配置并使能LPTIM */
  LPTIM_Configuration();

  // 等待按键按下
  while (LL_GPIO_IsInputPinSet(GPIOF, LL_GPIO_PIN_4) == 0)
  {
  }
  // 关闭GPIOB0
  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0);

  while (1)
  {
    LL_PWR_EnableLowPowerRunMode(); // 使能低功耗运行模式
    LL_LPTIM_Disable(LPTIM1);       // 关闭LPTIM
    LL_LPTIM_Enable(LPTIM1);        // 使能LPTIM
    LL_uDelay(70);                  // 延时70us

    LL_LPTIM_StartCounter(LPTIM1, LL_LPTIM_OPERATING_MODE_ONESHOT); // 启动LPTIM

    LL_LPM_EnableDeepSleep(); // 进入STOP模式，等待中断唤醒
    __WFI();                  // 进入低功耗模式
  }
}

static void LPTIM_Configuration(void)
{
  LL_RCC_LSI_Enable();              // 使能LSI时钟
  while (LL_RCC_LSI_IsReady() == 0) // 等待LSI时钟就绪
  {
  }
  LL_RCC_SetLPTIMClockSource(LL_RCC_LPTIM1_CLKSOURCE_LSI);          // LPTIM时钟源选择LSI
  LL_LPTIM_SetPrescaler(LPTIM1, LL_LPTIM_PRESCALER_DIV32);          // LPTIM预分频选择32分频, 32KHz / 32 = 1KHz, 也就是1ms
  LL_LPTIM_SetUpdateMode(LPTIM1, LL_LPTIM_UPDATE_MODE_ENDOFPERIOD); // LPTIM更新模式选择周期结束更新
  LL_LPTIM_EnableIT_ARRM(LPTIM1);                                   // 使能LPTIM自动重装载匹配中断

  NVIC_EnableIRQ(LPTIM1_IRQn);      // 使能LPTIM中断
  NVIC_SetPriority(LPTIM1_IRQn, 0); // 设置LPTIM中断优先级

  LL_LPTIM_Enable(LPTIM1); // 使能LPTIM

  LL_LPTIM_SetAutoReload(LPTIM1, 1000 + 1); // 设置LPTIM自动重装载值为1000, 也就是1s
}

void LPTIM1_IRQHandler(void)
{
  if (LL_LPTIM_IsActiveFlag_ARRM(LPTIM) == 1) // 判断LPTIM自动重装载匹配中断标志是否置位
  {
    LL_LPTIM_ClearFLAG_ARRM(LPTIM); // 清除LPTIM自动重装载匹配中断标志

    LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_0); // GPIOB0输出反转
  }
}
// GPIO配置
static void GPIO_Configuration(void)
{
  // 使能GPIOB和GPIOF时钟
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);
  // 配置GPIOB0为输出模式
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
  // GPIOB0输出高电平
  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0);
  // 配置GPIOF4为输入模式
  LL_GPIO_SetPinMode(GPIOF, LL_GPIO_PIN_4, LL_GPIO_MODE_INPUT);
}

static void RCC_Configuration(void)
{
  LL_RCC_HSI_Enable();                                  // 使能HSI
  LL_RCC_HSI_SetCalibFreq(LL_RCC_HSICALIBRATION_24MHz); // 设置HSI为24MHz
  while (LL_RCC_HSI_IsReady() != 1)                     // 等待HSI就绪
  {
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);                           // 设置AHB分频
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSISYS);                   // 设置系统时钟源为HSI
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSISYS) // 等待系统时钟源切换为HSI
  {
  }
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1); // 设置APB1分频

  SystemCoreClockUpdate(); // 更新系统时钟，更新后的系统时钟存放在SystemCoreClock中
}

// 以下为错误处理函数
void APP_ErrorHandler(void)
{
  // 死等
  while (1)
  {
  }
}
