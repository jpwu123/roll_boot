#pragma once

#include <stdint.h>

// gcp消息头
#define GCP_MAGIC0 0xAA
#define GCP_MAGIC1 0x55

/**
 * gcp消息类型
 * QUERY:  用于查询指定数据，接收方需使用NOTIFY类型回复该数据；
 * SET:    用于设置指定数据，接收方需使用NOTIFY类型回复该数据的最新值；
 * NOTIFY: 除了用于回复QUERY和SET类型外，当数据发生改变时可以使用NOTIFY类型主动发送给对方；
 * CMD:    用于命令接收方执行某种操作，接收方需使用ACK类型回复执行结果；
 * ACK:    用于回复CMD类型的执行结果；
 */
#define GCP_TYPE_01_QUERY  0x01 // 查询
#define GCP_TYPE_02_SET    0x02 // 设置
#define GCP_TYPE_03_NOTIFY 0x03 // 通知
#define GCP_TYPE_04_CMD    0x04 // 命令
#define GCP_TYPE_05_ACK    0x05 // 应答

#define GCP_MSG_LEN(msg)                      ((msg)->content_len + 8)                // 消息包长度
#define GCP_CONTENT_CAST(content_t, gcp_type) (((content_t *)msg->content)->gcp_type) // content类型转换

// gcp消息包结构
typedef struct {
    uint8_t magic0;       // 0xAA
    uint8_t magic1;       // 0x55
    uint8_t check;        // 校验
    uint8_t type;         // 消息类型
    uint8_t src_module;   // 源模块ID
    uint8_t dst_module;   // 目的模块ID
    uint8_t fc;           // 功能码
    uint8_t content_len;  // 内容长度
    uint8_t content[255]; // 内容
} gcp_msg_t;

uint8_t gcp_calc_checksum(gcp_msg_t *msg);
int gcp_msg_check(uint8_t *buf, uint16_t len);
uint16_t gcp_msg_pack(gcp_msg_t *gcp_msg,
                      uint8_t type,
                      uint8_t src_module,
                      uint8_t dst_module,
                      uint8_t fc,
                      uint8_t *content,
                      uint8_t content_len);
