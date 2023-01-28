#pragma once

#include "stm32g0xx.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_usart.h"
#include "stm32g0xx_ll_utils.h"

#include "gpio.h"
#include "uart.h"

#include <stdint.h>
#include <stdio.h>

#define APP_ADDR_OFC    APP_ADDR_OFC_HANDLE
#define PARAM_ADDR_OFC  PARAM_ADDR_OFC_CAMERA
#define GCP_SELF_MOFULE GCP_MODULE_03_ROL

#define led_on()     gpio_pin_set(GPIOC, LL_GPIO_PIN_13)
#define led_off()    gpio_pin_reset(GPIOC, LL_GPIO_PIN_13)
#define led_toggle() gpio_pin_toggle(GPIOC, LL_GPIO_PIN_13)

void bsp_init(void);
