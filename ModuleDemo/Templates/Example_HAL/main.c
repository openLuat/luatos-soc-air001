#include "air001xx_hal.h"

int main(void)
{
	HAL_Init();
	GPIO_InitTypeDef GPIO_LED = {
		.Pin = GPIO_PIN_0,
		.Mode = GPIO_MODE_OUTPUT_PP,
	};
	__HAL_RCC_GPIOB_CLK_ENABLE();
	HAL_GPIO_Init(GPIOB, &GPIO_LED);

	while (1)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
		HAL_Delay(500);
	}
}
