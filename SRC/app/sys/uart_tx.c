/**
 ********************************************************************
 * @file      uart_tx.c
 * @brief     串口发送消息包
 * @attention
 *
 * Change Logs:
 * Date              Author              Notes
 * 2023/01/28        wujingping          initial version
 ********************************************************************
 */
#include "uart_tx.h"
#include "bsp.h"
#include "gcp.h"
#include "protocol.h"
#include "upgrade_param.h"

#define RX_BUFFER_SIZE 64

uint8_t tx_buffer[RX_BUFFER_SIZE];

void uart_tx_init(void)
{
    memset(tx_buffer, 0, sizeof(tx_buffer));
}

/**
 * @brief  send_device_info_notifiy_msg
 * @param  None
 * @retval None
 */
void send_device_info_notifiy_msg(void)
{
    uint16_t content_size = 0;

    gcp_content_device_info_t content;

    content.notify.device_id = GCP_DEVICE_ID_03_M21;
    content.notify.program_status = GCP_PROGRAM_STATUS_01_BOOT;
    content.notify.hw_version[0] = upgrade_param.fw_version[0];
    content.notify.hw_version[1] = upgrade_param.fw_version[1];
    content.notify.hw_version[2] = upgrade_param.fw_version[2];
    content.notify.fw_version[0] = upgrade_param.hw_version[0];
    content.notify.fw_version[1] = upgrade_param.hw_version[1];
    content.notify.fw_version[2] = upgrade_param.hw_version[2];
    memcpy(content.notify.uid, (void *)(UID_BASE), 12);

    content_size = gcp_msg_pack((gcp_msg_t *)tx_buffer, GCP_TYPE_03_NOTIFY, GCP_MODULE_03_ROL, GCP_MODULE_06_PC, GCP_FC_00_DEVICE_INFO, (uint8_t *)&content, sizeof(content.notify));

    if (content_size != 0)
        uart_send_pack(tx_buffer, content_size);
}

/**
 * @brief  send_device_info_ack_msg
 * @param  rsp 应答状态
 * @retval None
 */
void send_boot_enter_ack_msg(uint8_t rsp)
{
    uint16_t content_size = 0;

    gcp_content_boot_enter_t content;

    content.ack.rsp = rsp;

    content_size = gcp_msg_pack((gcp_msg_t *)tx_buffer, GCP_TYPE_05_ACK, GCP_MODULE_03_ROL, GCP_MODULE_06_PC, GCP_FC_01_BOOT_ENTER, (uint8_t *)&content, sizeof(content.ack));

    if (content_size != 0)
        uart_send_pack(tx_buffer, content_size);
}

/**
 * @brief  send_boot_start_ack_msg
 * @param  rsp 应答状态
 * @retval None
 */
void send_boot_start_ack_msg(uint8_t rsp)
{
    uint16_t content_size = 0;

    gcp_content_boot_start_t content;

    content.ack.rsp = rsp;

    content_size = gcp_msg_pack((gcp_msg_t *)tx_buffer, GCP_TYPE_05_ACK, GCP_MODULE_03_ROL, GCP_MODULE_06_PC, GCP_FC_02_BOOT_START, (uint8_t *)&content, sizeof(content.ack));

    if (content_size != 0)
        uart_send_pack(tx_buffer, content_size);
}

/**
 * @brief  send_boot_fw_block_ack_msg
 * @param  rsp 应答状态
 * offset_addr  固件块偏移地址
 * @retval None
 */
void send_boot_fw_block_ack_msg(uint8_t rsp, uint32_t offset_addr)
{
    uint16_t content_size = 0;

    gcp_content_boot_fw_block_t content;

    content.ack.rsp = rsp;
    content.ack.offset_addr = offset_addr;

    content_size = gcp_msg_pack((gcp_msg_t *)tx_buffer, GCP_TYPE_05_ACK, GCP_MODULE_03_ROL, GCP_MODULE_06_PC, GCP_FC_03_BOOT_FW_BLOCK, (uint8_t *)&content, sizeof(content.ack));

    if (content_size != 0)
        uart_send_pack(tx_buffer, content_size);
}

/**
 * @brief  send_boot_end_ack_msg
 * @param  rsp 应答状态
 * @retval None
 */
void send_boot_end_ack_msg(uint8_t rsp)
{
    uint16_t content_size = 0;

    gcp_content_boot_end_t content;

    content.ack.rsp = rsp;

    content_size = gcp_msg_pack((gcp_msg_t *)tx_buffer, GCP_TYPE_05_ACK, GCP_MODULE_03_ROL, GCP_MODULE_06_PC, GCP_FC_04_BOOT_END, (uint8_t *)&content, sizeof(content.ack));

    if (content_size != 0)
        uart_send_pack(tx_buffer, content_size);
}

/**
 * @brief  send_boot_end_ack_msg
 * @param  version 固件版本
 * @retval None
 */
void send_boot_fw_version_notify_msg(uint8_t version[3])
{
    uint16_t content_size = 0;

    gcp_content_fw_version_t content;

    content.notify.version[1] = version[1];
    content.notify.version[2] = version[3];
    content.notify.version[2] = version[3];

    content_size = gcp_msg_pack((gcp_msg_t *)tx_buffer, GCP_TYPE_03_NOTIFY, GCP_MODULE_03_ROL, GCP_MODULE_06_PC, GCP_FC_06_FW_VERSION, (uint8_t *)&content, sizeof(content.notify));

    if (content_size != 0)
        uart_send_pack(tx_buffer, content_size);
}

/**
 * @brief  send_boot_exit_ack_msg
 * @param  rsp 应答状态
 * @retval None
 */
void send_boot_exit_ack_msg(uint8_t rsp)
{
    uint16_t content_size = 0;

    gcp_content_boot_exit_t content;

    content.ack.rsp = rsp;

    content_size = gcp_msg_pack((gcp_msg_t *)tx_buffer, GCP_TYPE_05_ACK, GCP_MODULE_03_ROL, GCP_MODULE_06_PC, GCP_FC_05_BOOT_EXIT, (uint8_t *)&content, sizeof(content.ack));

    if (content_size != 0)
        uart_send_pack(tx_buffer, content_size);
}
