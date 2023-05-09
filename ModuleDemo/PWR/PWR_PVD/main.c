#include "main.h"
static void RCC_Configuration(void);  // 配置时钟
static void GPIO_Configuration(void); // 配置GPIO
static void PVD_Configuration(void);  // 配置PVD
int main(void)
{
  /* 配置系统时钟 */
  RCC_Configuration();
  // GPIO初始化
  GPIO_Configuration();
  // PVD初始化
  PVD_Configuration();
  while (1)
  {
  }
}

// 初始化GPIO
static void GPIO_Configuration(void)
{
  // 使能GPIOB时钟
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  // 开发板上中间的灯连接在PB0，初始化PB0为推挽输出
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
}
// 初始化PVD
static void PVD_Configuration(void)
{
  LL_EXTI_InitTypeDef EXTI_InitStruct;
  EXTI_InitStruct.Line = LL_EXTI_LINE_16;            // 外部中断线
  EXTI_InitStruct.LineCommand = ENABLE;              // 使能外部中断线
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;            // 中断模式
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING; // 下降沿触发
  LL_EXTI_Init(&EXTI_InitStruct);                    // 初始化外部中断线
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR); // 使能PWR时钟
  LL_PWR_EnablePVD();                                // 使能PVD
  LL_PWR_SetPVDLevel(LL_PWR_PVDLEVEL_2V8);           // 设置PVD电压为2.8V
  LL_PWR_EnablePVDFilter();                          // 使能PVD滤波
  LL_PWR_SetPVDFilter(LL_PWR_PVD_FILTER_1024CLOCK);  // 设置PVD滤波时间为1024个时钟,即约17ms,滤波时间越长,灵敏度越低,但是误报率越低
  NVIC_SetPriority(PVD_IRQn, 1);                     // 设置中断优先级
  NVIC_EnableIRQ(PVD_IRQn);                          // 使能中断
}
void PVD_IRQHandler(void)
{
  if (LL_PWR_IsActiveFlag_PVDO()) // 判断PVD输出电平,如果为低电平,则点亮PB0,否则熄灭PB0
  {
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0); // 点亮PB0
  }
  else
  {
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0); // 熄灭PB0
  }
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
