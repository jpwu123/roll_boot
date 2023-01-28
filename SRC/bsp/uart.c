/**
 ********************************************************************
 * @file      uart.c
 * @brief     uart driver
 * @attention
 *
 * Change Logs:
 * Date              Author              Notes
 * 2023/01/04        wujingping          initial version
 ********************************************************************
 */
#include "uart.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_usart.h"

#define uart_putc(uartx, ch)                          \
    do                                                \
    {                                                 \
        while (LL_USART_IsActiveFlag_TXE(uartx) == 0) \
            ;                                         \
        LL_USART_TransmitData8(uartx, ch);            \
    } while (0)

/**
 * @brief  uart1 config
 *  UART1_TX: PA9
 *  UART1_RX: PA10
 * @param  None
 * @retval None
 */
void uart1_config(void)
{
    LL_USART_InitTypeDef USART_InitStruct = {0};
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
    GPIO_InitStruct.Pin = LL_GPIO_PIN_9 | LL_GPIO_PIN_10;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    LL_USART_StructInit(&USART_InitStruct);
    USART_InitStruct.BaudRate = 115200;
    LL_USART_Init(USART1, &USART_InitStruct);

    LL_USART_ConfigAsyncMode(USART1);
    LL_USART_Enable(USART1);
}

/**
 * @brief  uart1_putc
 * @param  ch 待发送字符
 * @retval None
 */
void uart1_putc(uint8_t ch)
{
    uart_putc(USART1, ch);
}

/**
 * @brief  uart2 config
 *  UART2_TX: PA2
 *  UART2_RX: PA3
 * @param  None
 * @retval None
 */
void uart2_config(void)
{
    LL_USART_InitTypeDef USART_InitStruct = {0};
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
    GPIO_InitStruct.Pin = LL_GPIO_PIN_2 | LL_GPIO_PIN_3;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    LL_USART_StructInit(&USART_InitStruct);
    USART_InitStruct.BaudRate = 115200;
    LL_USART_Init(USART2, &USART_InitStruct);

    LL_USART_ConfigAsyncMode(USART2);
    LL_USART_Enable(USART2);
}

/**
 * @brief  uart2_putc
 * @param  ch 待发送字符
 * @retval None
 */
void uart2_putc(uint8_t ch)
{
    uart_putc(USART2, ch);
}
