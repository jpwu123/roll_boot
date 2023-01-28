/**
 ********************************************************************
 * @file      bsp.c
 * @brief     bsp driver
 * @attention
 *
 * Change Logs:
 * Date              Author              Notes
 * 2023/01/28        wujingping          initial version
 ********************************************************************
 */
#include "bsp.h"

static void interrupt_close(void);
static void periph_reset(void);
static void SystemClock_Config(void);

/**
 * @brief  bsp_init
 * @param  None
 * @retval None
 */
void bsp_init(void)
{
    SystemClock_Config();

    gpio_config();

    led_off();
}

/**
 * @brief  boot_jump_to_app
 * @param  addr_offset  偏移地址
 * @retval 错误码(-1  jump to app error)
 */
int boot_jump_to_app(uint32_t addr_offset)
{
    uint32_t app_addr = addr_offset + FLASH_BASE;

    void (*jump_to_app)(void);

    interrupt_close();
    periph_reset();

    if (((*(uint32_t *)app_addr) > SRAM_BASE) && ((*(uint32_t *)app_addr) <= (SRAM_BASE + SRAM_SIZE_MAX)))
    {
        jump_to_app = (void (*)(void))(*((uint32_t *)(app_addr + 4)));
        __set_MSP(*(uint32_t *)app_addr);
        jump_to_app();
    }
    return -1;
}

/**
 * @brief  interrupt_close
 * @param  None
 * @retval None
 */
static void interrupt_close(void)
{

    /* 关闭全局中断 */
    __disable_irq();

    /* 关闭所有中断，清除所有中断挂起标志 */
    NVIC->ICER[0] = 0xFFFFFFFF;
    NVIC->ICPR[0] = 0xFFFFFFFF;
}

/**
 * @brief  periph_reset
 * @param  None
 * @retval None
 */
static void periph_reset(void)
{
    /* 设置所有时钟到默认状态，使用HSI时钟 */
    LL_RCC_DeInit();

    /* 关闭滴答定时器，复位到默认值 */
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;
}

/**
 * @brief  SystemClock_Config
 *   HSI - 16MHz(SYS时钟源)
 *   SYS - 16MHz
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void)
{
    LL_RCC_HSI_Enable();
    while (LL_RCC_HSI_IsReady() != 1)
    {
    }
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
    {
    }

    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

    LL_Init1msTick(16000000);

    LL_SetSystemCoreClock(16000000);
}
