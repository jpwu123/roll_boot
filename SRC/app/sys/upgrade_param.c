/**
 ********************************************************************
 * @file      upgrade_param.c
 * @brief     升级参数相关
 * @attention
 *
 * Change Logs:
 * Date              Author              Notes
 * 2023/01/28        wujingping          initial version
 ********************************************************************
 */
#include "upgrade_param.h"
#include "bsp.h"

#define PARAM_BUF_SIZE 8

upgrade_param_t upgrade_param;

static void upgrade_param_write(upgrade_param_t *upgrade_param);

/**
 * @brief  upgrade_param_init
 * 升级参数初始化
 * @param  None
 * @retval None
 */
void upgrade_param_init(void)
{
    uint8_t sum = 0;
    uint8_t *ptr_param = (uint8_t *)PARAM_ADDR_OFC;

    for (uint8_t i = 0; i < PARAM_BUF_SIZE - 1; i++)
        sum += ptr_param[i];

    if (ptr_param[PARAM_BUF_SIZE - 1] != sum)
    {
        upgrade_param.fw_version[0] = 1;
        upgrade_param.fw_version[1] = 0;
        upgrade_param.fw_version[2] = 0;

        upgrade_param.hw_version[0] = 1;
        upgrade_param.hw_version[1] = 0;
        upgrade_param.hw_version[2] = 0;

        upgrade_param.upgrade_state = UPGRADE_STATE_BOOT;

        upgrade_param_save();
    }
    else
        memcpy(&upgrade_param, (void *)PARAM_ADDR_OFC, sizeof(upgrade_param));
}

/**
 * @brief  upgrade_param_save
 * 升级参保存
 * @param  None
 * @retval None
 */
void upgrade_param_save(void)
{
    upgrade_param_calc_checksum(&upgrade_param);

    upgrade_param_write(&upgrade_param);
}

/**
 * @brief  upgrade_param_calc_checksum
 * 升级参数和检验计算
 * @param  upgrade_param 升级参数指针
 * @retval None
 */
void upgrade_param_calc_checksum(upgrade_param_t *upgrade_param)
{
    uint8_t sum = 0;

    for (uint16_t i = 0; i < 3; i++)
        sum += upgrade_param->fw_version[i];
    for (uint16_t i = 0; i < 3; i++)
        sum += upgrade_param->hw_version[i];
    sum += upgrade_param->upgrade_state;

    upgrade_param->param_check = sum;
}

/**
 * @brief  upgrade_param_write
 * 升级参数写入flash
 * @param  upgrade_param 升级参数指针
 * @retval None
 */
static void upgrade_param_write(upgrade_param_t *upgrade_param)
{
    flash_write_start();

    flash_page_erase(PARAM_ADDR_OFC);

    flash_write(PARAM_ADDR_OFC, (uint8_t *)upgrade_param, PARAM_BUF_SIZE);

    flash_write_finish();
}
