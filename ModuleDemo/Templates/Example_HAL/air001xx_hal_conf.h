#ifndef __AIR001xx_HAL_CONF_DEFAULT_H
#define __AIR001xx_HAL_CONF_DEFAULT_H

#ifdef __cplusplus
extern "C" {
#endif

#define HAL_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED

#define HSE_VALUE            8000000U  
#define HSE_STARTUP_TIMEOUT  100U      
#define HSI_VALUE            24000000U 
#define HSI_STARTUP_TIMEOUT  5000U     
#define LSI_VALUE            32000U
#define LSE_VALUE            32768U    /*!< Value of the External Low Speed oscillator in Hz */
#define LSE_STARTUP_TIMEOUT  5000U     /*!< Time out for LSE start up, in ms */
#define  TICK_INT_PRIORITY            0x00U /*!< tick interrupt priority */


#include "air001xx_hal_rcc.h"
#include "air001xx_hal_gpio.h"
#include "air001xx_hal_flash.h"
#include "air001xx_hal_pwr.h"
#include "air001xx_hal_cortex.h"

#define assert_param(expr) ((void)0U)

#ifdef __cplusplus
}
#endif

#endif