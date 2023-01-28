#pragma once

#include <stdint.h>

void uart_tx_init(void);
void send_device_info_notifiy_msg(void);
void send_boot_enter_ack_msg(uint8_t rsp);
void send_boot_start_ack_msg(uint8_t rsp);
void send_boot_fw_block_ack_msg(uint8_t rsp, uint32_t offset_addr);
void send_boot_end_ack_msg(uint8_t rsp);
void send_boot_fw_version_notify_msg(uint8_t version[3]);
void send_boot_exit_ack_msg(uint8_t rsp);
