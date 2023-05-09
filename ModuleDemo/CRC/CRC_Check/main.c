#include "main.h"
static void RCC_Configuration(void);         // 配置时钟
static void GPIO_Configuration(void);        // 配置GPIO
static void UART_Configuration(void);        // 配置UART
static void CRC_Configuration(void);         // 配置CRC
LL_UTILS_ClkInitTypeDef UTILS_ClkInitStruct; // 系统时钟结构体
// 初始化一个uint32_t类型100大小的数组，用于存放CRC校验的数据，数组中的数据可以随意设置，这里只是举例
uint32_t uwCRCData[100] = {
    0x00000000, 0x00000001, 0x00000002, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007, 0x00000008, 0x00000009,
    0x0000000A, 0x0000000B, 0x0000000C, 0x0000000D, 0x0000000E, 0x0000000F, 0x00000010, 0x00000011, 0x00000012, 0x00000013,
    0x00000014, 0x00000015, 0x00000016, 0x00000017, 0x00000018, 0x00000019, 0x0000001A, 0x0000001B, 0x0000001C, 0x0000001D,
    0x0000001E, 0x0000001F, 0x00000020, 0x00000021, 0x00000022, 0x00000023, 0x00000024, 0x00000025, 0x00000026, 0x00000027,
    0x00000028, 0x00000029, 0x0000002A, 0x0000002B, 0x0000002C, 0x0000002D, 0x0000002E, 0x0000002F, 0x00000030, 0x00000031,
    0x00000032, 0x00000033, 0x00000034, 0x00000035, 0x00000036, 0x00000037, 0x00000038, 0x00000039, 0x0000003A, 0x0000003B,
    0x0000003C, 0x0000003D, 0x0000003E, 0x0000003F, 0x00000040, 0x00000041, 0x00000042, 0x00000043, 0x00000044, 0x00000045,
    0x00000046, 0x00000047, 0x00000048, 0x00000049, 0x0000004A, 0x0000004B, 0x0000004C, 0x0000004D, 0x0000004E, 0x0000004F,
    0x00000050, 0x00000051, 0x00000052, 0x00000053, 0x00000054, 0x00000055, 0x00000056, 0x00000057, 0x00000058, 0x00000059,
    0x0000005A, 0x0000005B, 0x0000005C, 0x0000005D, 0x0000005E, 0x0000005F, 0x00000060, 0x00000061, 0x00000062, 0x00000063,
};
// 初始化一个32位的变量，用于存放CRC校验的结果
uint32_t uwCRCValue = 0;
int main(void)
{
  // 配置时钟
  RCC_Configuration();
  // 配置SysTick
  LL_Init1msTick(SystemCoreClock);
  UART_Configuration();
  // 配置GPIO
  GPIO_Configuration();
  // 配置CRC
  CRC_Configuration();
  printf("CRC Check Demo\r\n");
  // 计算CRC校验值
  for (int i = 0; i < 100; i++)
  {
    LL_CRC_FeedData32(CRC, uwCRCData[i]);
  }
  uwCRCValue = LL_CRC_ReadData32(CRC);
  printf("CRC Value:0x%08X\r\n", uwCRCValue);

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
// 配置CRC
static void CRC_Configuration(void)
{
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_CRC); // 使能CRC时钟
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
void UART_Configuration(void)
{
  // 使能USART2时钟
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
  // 配置USART2
  LL_USART_SetBaudRate(USART2, SystemCoreClock, LL_USART_OVERSAMPLING_16, 115200); // 配置波特率为115200
  LL_USART_SetDataWidth(USART2, LL_USART_DATAWIDTH_8B);                            // 配置数据位为8位
  LL_USART_SetStopBitsLength(USART2, LL_USART_STOPBITS_1);                         // 配置停止位为1位
  LL_USART_SetParity(USART2, LL_USART_PARITY_NONE);                                // 配置校验位为无校验
  LL_USART_SetHWFlowCtrl(USART2, LL_USART_HWCONTROL_NONE);                         // 配置硬件流控制为无
  LL_USART_SetTransferDirection(USART2, LL_USART_DIRECTION_TX_RX);                 // 配置传输方向为收发
  LL_USART_Enable(USART2);                                                         // 使能USART2
  LL_USART_ClearFlag_TC(USART2);                                                   // 清除发送完成标志

  // 配置USART2的GPIO
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA); // 使能GPIOA时钟

  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE);        // 配置PA2为复用功能
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_2, LL_GPIO_SPEED_FREQ_VERY_HIGH); // 配置PA2为高速
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_2, LL_GPIO_PULL_UP);               // 配置PA2为上拉
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_4);                // 配置PA2为复用功能4

  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);        // 配置PA3为复用功能
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_VERY_HIGH); // 配置PA3为高速
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_3, LL_GPIO_PULL_UP);               // 配置PA3为上拉
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_3, LL_GPIO_AF_4);                // 配置PA3为复用功能4
}
int fgetc(FILE *f)
{
  int ch;
  while (!LL_USART_IsActiveFlag_RXNE(USART2))
    ;
  ch = LL_USART_ReceiveData8(USART2);
  return (ch);
}
int fputc(int ch, FILE *f)
{
  /* Send a byte to USART */
  LL_USART_TransmitData8(USART2, ch);
  while (!LL_USART_IsActiveFlag_TC(USART2))
    ;
  LL_USART_ClearFlag_TC(USART2);

  return (ch);
}
// 以下为错误处理函数
void APP_ErrorHandler(void)
{
  // 死等
  while (1)
  {
  }
}
