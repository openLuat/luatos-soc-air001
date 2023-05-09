#include "air001xx_ll_gpio.h"
#include "air001xx_ll_usart.h"
#include "main.h"
static void RCC_Configuration(void);               // 配置时钟
static void UART_Configuration(uint32_t bandrate); // 配置UART
LL_UTILS_ClkInitTypeDef UTILS_ClkInitStruct;       // 系统时钟结构体
static USART_TypeDef *USARTx = USART1;

int main(void)
{
  // 配置时钟
  RCC_Configuration();
  // 配置SysTick
  LL_Init1msTick(SystemCoreClock);
  // 配置UART
  UART_Configuration(115200);

  while (1)
  {
    APP_UartSendString("uart test!\r\n");
    LL_mDelay(1000);
  }
}
// 配置时钟
static void RCC_Configuration(void)
{
  LL_RCC_HSI_Enable();                                  // 使能HSI
  LL_RCC_HSI_SetCalibFreq(LL_RCC_HSICALIBRATION_24MHz); // 设置HSI为24MHz
  while (LL_RCC_HSI_IsReady() != 1);                     // 等待HSI就绪

  LL_PLL_ConfigSystemClock_HSI(&UTILS_ClkInitStruct); // 配置HSI为系统时钟

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1); // 设置AHB分频

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);                   // 设置系统时钟源为PLL
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL); // 等待系统时钟源切换为PLL

  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1); // 设置APB1分频

  SystemCoreClockUpdate(); // 更新系统时钟，更新后的系统时钟存放在SystemCoreClock中
}

// 初始化GPIO
static void UART_Configuration(uint32_t bandrate)
{
  // 使能GPIOA时钟
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

  // 使能USART1时钟
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_USART1);

  // GPIOA配置
  LL_GPIO_InitTypeDef GPIO_InitStruct;
  // 选择2号引脚
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
  // 选择复用模式
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  // 选择输出速度
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  // 选择输出模式
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  // 选择上拉
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  // 复用为USART1功能
  GPIO_InitStruct.Alternate = LL_GPIO_AF1_USART1;
  // GPIOA初始化
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // PA3
  GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
  // 复用为USART1功能
  GPIO_InitStruct.Alternate = LL_GPIO_AF1_USART1;
  // GPIOA初始化
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 设置USART1中断优先级
  NVIC_SetPriority(USART1_IRQn, 0);
  // 使能USART1中断
  NVIC_EnableIRQ(USART1_IRQn);

  // 配置USART功能
  LL_USART_InitTypeDef USART_InitStruct;
  // 设置波特率
  USART_InitStruct.BaudRate = bandrate;
  // 设置数据长度
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  // 停止位
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  // 设置校验位
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  // USART初始化
  LL_USART_Init(USARTx, &USART_InitStruct);

  // 配置为全双工异步模式
  LL_USART_ConfigAsyncMode(USARTx);

  // 使能UART模块
  LL_USART_Enable(USARTx);

  // 使能接收奇偶校验错误中断
  LL_USART_EnableIT_PE(USARTx);
  // 使能接收错误中断
  LL_USART_EnableIT_ERROR(USARTx);
  // 使能接收数据寄存器非空中断
  LL_USART_EnableIT_RXNE(USARTx);
}

// 发送一个字节
void APP_UartSendByte(unsigned char d)
{
  LL_USART_TransmitData8(USARTx, d);
  while (!LL_USART_IsActiveFlag_TXE(USARTx))
    ;
}
// 发送字符串
void APP_UartSendString(char *s)
{
  while (*s)
    APP_UartSendByte(*s++);
}

// 串口中断
void APP_UsartIRQCallback()
{
  uint32_t errorflags = (LL_USART_IsActiveFlag_PE(USARTx) | LL_USART_IsActiveFlag_FE(USARTx) |
                         LL_USART_IsActiveFlag_ORE(USARTx) | LL_USART_IsActiveFlag_NE(USARTx));
  if (errorflags == RESET)
  {
    // 接收数据寄存器不为空
    if ((LL_USART_IsActiveFlag_RXNE(USARTx) != RESET) && (LL_USART_IsEnabledIT_RXNE(USARTx) != RESET))
    {
      // 接收到数据
      uint8_t RxBuff = LL_USART_ReceiveData8(USARTx);
      APP_UartSendByte(RxBuff); // 发回去
      return;
    }
  }

  // 接收错误
  if (errorflags != RESET)
  {
    APP_ErrorHandler();
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
