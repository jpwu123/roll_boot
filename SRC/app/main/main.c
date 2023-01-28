/**
 ********************************************************************
 * @file      main.c
 * @brief
 * @attention
 *
 * Change Logs:
 * Date              Author              Notes
 * 2023/01/28        wujingping          initial version
 ********************************************************************
 */
#include "bsp.h"

/**
 * @brief  main
 * @param  None
 * @retval 错误码
 */
int main(void)
{
    bsp_init();

    while (1)
    {
        led_toggle();
        LL_mDelay(100);
    }
}
