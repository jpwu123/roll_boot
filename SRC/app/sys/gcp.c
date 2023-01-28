/**
 ********************************************************************
 * @file      gcp.c
 * @brief     gcp软件包
 *  gudsen communication protocol固胜通讯协议
 * @attention 
 *  1、0x00~0x1F为公共模块ID，0x20~0xFF为自定义模块ID。
 *  2、0x00~0x1F为公共功能码，0x20~0xFF自定义功能码。
 *  3、消息包字节顺序为小端模式。
 *
 * Change Logs:
 * Date              Author              Notes
 * 2021/04/14        xuyafei             initial version
 ********************************************************************
 */
#include "gcp.h"
#include <string.h>

/**
 * @brief  计算校验和
 * @param  msg gcp消息指针
 * @retval 校验
 */
uint8_t gcp_calc_checksum(gcp_msg_t *msg)
{
    uint8_t sum = 0;

    sum += msg->type;
    sum += msg->src_module;
    sum += msg->dst_module;
    sum += msg->fc;
    sum += msg->content_len;

    for (uint16_t i = 0; i < msg->content_len; i++)
        sum += msg->content[i];

    return sum;
}

/**
 * @brief  gcp消息格式检查
 * @param  msg gcp消息指针
 * @retval 错误码
 */
int gcp_msg_check(uint8_t *buf, uint16_t len)
{
    gcp_msg_t *msg = (gcp_msg_t *)buf;

    if (len < 8)
        return -1;

    if (msg->magic0 != GCP_MAGIC0)
        return -2;

    if (msg->magic1 != GCP_MAGIC1)
        return -3;

    if (len < msg->content_len + 8)
        return -4;

    if (msg->check != gcp_calc_checksum(msg))
        return -5;

    return 0;
}

/**
 * @brief  装载gcp消息包
 * @param  msg 消息包指针
 * @param  type 消息类型
 * @param  src_module 源模块ID
 * @param  dst_module 目的模块ID
 * @param  fc 功能码
 * @param  content 消息内容指针
 * @param  content_len 消息内容长度
 * @retval 消息包长度
 */
uint16_t gcp_msg_pack(gcp_msg_t *msg,
                      uint8_t type,
                      uint8_t src_module,
                      uint8_t dst_module,
                      uint8_t fc,
                      uint8_t *content,
                      uint8_t content_len)
{
    msg->magic0 = GCP_MAGIC0;
    msg->magic1 = GCP_MAGIC1;
    msg->type = type;
    msg->src_module = src_module;
    msg->dst_module = dst_module;
    msg->fc = fc;
    msg->content_len = content_len;
    memcpy(msg->content, content, content_len);
    msg->check = gcp_calc_checksum(msg);

    return (msg->content_len + 8);
}
