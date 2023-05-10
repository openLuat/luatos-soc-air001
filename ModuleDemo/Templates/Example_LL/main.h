#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "air001xx_ll_rcc.h"
#include "air001xx_ll_bus.h"
#include "air001xx_ll_system.h"
#include "air001xx_ll_cortex.h"
#include "air001xx_ll_utils.h"
#include "air001xx_ll_pwr.h"
#include "air001xx_ll_dma.h"
#include "air001xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "air001_assert.h"
#endif
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif
