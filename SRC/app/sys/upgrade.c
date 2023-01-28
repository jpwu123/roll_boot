/**
 ********************************************************************
 * @file      upgrade.c
 * @brief     升级相关
 * @attention
 *
 * Change Logs:
 * Date              Author              Notes
 * 2023/01/28        wujingping          initial version
 ********************************************************************
 */

#include "upgrade.h"
#include "bsp.h"
#include "uart_tx.h"
#include "upgrade_param.h"

uint32_t flash_start_addr = 0;
uint32_t fw_size = 0;

/**
 * @brief  upgrade_enter
 * 进入升级模式
 * @param  None
 * @retval None
 */
void upgrade_enter(void)
{
    uint8_t rsp;

    upgrade_param.upgrade_state = UPGRADE_STATE_UPGRADE;
    upgrade_param_save();

    rsp = GCP_BOOT_ENTER_RSP_00_SUCCESS;
    send_boot_enter_ack_msg(rsp);
}

/**
 * @brief  upgrade_start
 * 升级开始
 * @param  msg 接收到的消息包
 * @retval None
 */
void upgrade_start(gcp_msg_t *msg)
{

    uint32_t flash_end_addr;
    uint8_t rsp;

    gcp_content_boot_start_t *ptr_content;

    ptr_content = (gcp_content_boot_start_t *)msg->content;

    flash_start_addr = ptr_content->cmd.base_addr;
    fw_size = ptr_content->cmd.fw_size;
    flash_end_addr = flash_start_addr + fw_size;

    if (flash_start_addr % FLASH_PAGE_SIZE == 0 && flash_start_addr >= (APP_ADDR_OFC + FLASH_BASE) && flash_end_addr < (PARAM_ADDR_OFC + FLASH_BASE))
    {
        flash_write_start();

        if (flash_pages_erase(flash_start_addr, flash_end_addr) == 0)
        {
            rsp = GCP_BOOT_START_RSP_00_SUCCESS;
        }
        else
        {
            rsp = GCP_BOOT_START_RSP_02_ERASE_ERR;
        }
    }
    else
    {
        rsp = GCP_BOOT_START_RSP_01_ADDR_ERR;
    }

    send_boot_start_ack_msg(rsp);
}

/**
 * @brief  upgrade_write_fw_block
 * 固件块写入
 * @param  msg 接收到的消息包
 * @retval None
 */
void upgrade_write_fw_block(gcp_msg_t *msg)
{
    uint32_t blcok_offset_addr;
    uint8_t rsp;

    gcp_content_boot_fw_block_t *ptr_content;

    ptr_content = (gcp_content_boot_fw_block_t *)msg->content;
    blcok_offset_addr = ptr_content->cmd.offset_addr;

    if (blcok_offset_addr >= fw_size)
    {
        rsp = GCP_BOOT_FW_BLOCK_RSP_01_ADDR_ERR;
    }

    if (flash_write(flash_start_addr + blcok_offset_addr, ptr_content->cmd.block, ptr_content->cmd.size) == 0)
    {
        rsp = GCP_BOOT_FW_BLOCK_RSP_00_SUCCESS;
    }
    else
    {
        rsp = GCP_BOOT_FW_BLOCK_RSP_02_PROGRAM_ERR;
    }

    send_boot_fw_block_ack_msg(rsp, blcok_offset_addr);
}

/**
 * @brief  upgrade_end
 * 写入数据和检验检查
 * @param  msg 接收到的消息包
 * @retval None
 */
void upgrade_end(gcp_msg_t *msg)
{
    uint16_t sum = 0;

    uint16_t checksum;
    uint8_t rsp;

    gcp_content_boot_end_t *ptr_content;

    ptr_content = (gcp_content_boot_end_t *)msg->content;
    checksum = ptr_content->cmd.checksum;

    for (uint32_t i = 0; i < fw_size; i++)
        sum += *(uint8_t *)(flash_start_addr + i);
    sum = sum & 0xFFFF;

    if (checksum == sum)
    {
        rsp = GCP_BOOT_END_RSP_00_SUCCESS;
    }
    else
    {
        rsp = GCP_BOOT_END_RSP_01_VERIFY_ERR;
    }

    flash_write_finish();

    send_boot_end_ack_msg(rsp);
}

/**
 * @brief  set_upgrade_fw_version
 * 设置固件版本
 * @param  msg 接收到的消息包
 * @retval None
 */
void upgrade_set_fw_version(gcp_msg_t *msg)
{
    uint8_t version[3] = {0, 0, 0};

    gcp_content_fw_version_t *ptr_content;

    ptr_content = (gcp_content_fw_version_t *)msg->content;

    memcpy(upgrade_param.fw_version, ptr_content->set.version, sizeof(upgrade_param.fw_version));
    upgrade_param_save();

    memcpy(&version, (void *)PARAM_ADDR_OFC, sizeof(version));

    send_boot_fw_version_notify_msg(version);
}

/**
 * @brief  upgrade_exit
 * 退出升级
 * @param  msg 接收到的消息包
 * @retval None
 */
void upgrade_exit(gcp_msg_t *msg)
{
    uint8_t rsp;

    upgrade_param.upgrade_state = UPGRADE_STATE_APP;
    upgrade_param_save();

    rsp = GCP_BOOT_EXIT_RSP_00_SUCCESS;

    send_boot_exit_ack_msg(rsp);
}
