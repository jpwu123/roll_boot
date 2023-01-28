#pragma once

#include <stdint.h>

#define gpio_pin_set(gpio, pin)    LL_GPIO_SetOutputPin(gpio, pin)
#define gpio_pin_reset(gpio, pin)  LL_GPIO_ResetOutputPin(gpio, pin)
#define gpio_pin_toggle(gpio, pin) LL_GPIO_TogglePin(gpio, pin)
#define gpio_pin_read(gpio, pin)   LL_GPIO_IsInputPinSet(gpio, pin)

void gpio_config(void);
