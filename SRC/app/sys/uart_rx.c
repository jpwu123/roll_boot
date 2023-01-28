/**
 ********************************************************************
 * @file      uart_rx.c
 * @brief     串口接收处理
 * @attention
 *
 * Change Logs:
 * Date              Author              Notes
 * 2023/01/28        wujingping          initial version
 ********************************************************************
 */
#include "uart_rx.h"
#include "uart_tx.h"
#include "upgrade.h"
#include "upgrade_param.h"

#include "bsp.h"

#define RX_BUFFER_SIZE 64

typedef enum {
    RECV_DATA_MAGIC0,
    RECV_DATA_MAGIC1,
    RECV_DATA_CHECK,
    RECV_DATA_TYPE,
    RECV_DATA_SRC_MODULE,
    RECV_DATA_DST_MODULE,
    RECV_DATA_FC,
    RECV_DATA_CONTENT_LEN,
    RECV_DATA_CONTENT,
} recv_gcp_state_t;

typedef struct {
    uint8_t sum;
    uint16_t content_index;

    recv_gcp_state_t state;

    gcp_msg_t *msg;

    void (*callback)(gcp_msg_t *);
} gcp_recv_t;

gcp_recv_t recv;

uint8_t rx_buffer[RX_BUFFER_SIZE];

static void uart_rx_data_process(gcp_recv_t *recv, uint8_t dat);
static void uart_rx_data_process_init(gcp_recv_t *recv, uint8_t *buf, void (*callback)(gcp_msg_t *));
static void uart_rx_handler(gcp_msg_t *msg);
static void gcp_recv_reset(gcp_recv_t *recv);

/**
 * @brief  uart_rx_init
 * 串口接收数据初始化
 * @param  None
 * @retval None
 */
void uart_rx_init(void)
{
    uart_rx_data_process_init(&recv, rx_buffer, uart_rx_handler);
}

/**
 * @brief  uart_rx_byte_process
 * @param  data  接收到的数据
 * @retval None
 */
void uart_rx_byte_process(uint8_t data)
{
    uart_rx_data_process(&recv, data);
}

/**
 * @brief  uart_rx_handler
 * @param  msg 消息包指针
 * @retval None
 */
static void uart_rx_handler(gcp_msg_t *msg)
{
    if (msg->dst_module != GCP_SELF_MOFULE)
    {
        // 转发
    }

    switch (msg->fc)
    {
    case GCP_FC_00_DEVICE_INFO:
        if (msg->type == GCP_TYPE_01_QUERY)
        {
            send_device_info_notifiy_msg();
        }
        break;

    case GCP_FC_01_BOOT_ENTER:
        if (msg->type == GCP_TYPE_04_CMD)
        {
            upgrade_enter();
        }
        break;

    case GCP_FC_02_BOOT_START:
        if (msg->type == GCP_TYPE_04_CMD)
        {
            upgrade_start(msg);
        }
        break;

    case GCP_FC_03_BOOT_FW_BLOCK:
        if (msg->type == GCP_TYPE_04_CMD)
        {
            upgrade_write_fw_block(msg);
        }
        break;

    case GCP_FC_04_BOOT_END:
        if (msg->type == GCP_TYPE_04_CMD)
        {
            upgrade_end(msg);
        }
        break;

    case GCP_FC_06_FW_VERSION:
        if (msg->type == GCP_TYPE_02_SET)
        {
            upgrade_set_fw_version(msg);
        }
        break;

    case GCP_FC_05_BOOT_EXIT:
        if (msg->type == GCP_TYPE_04_CMD)
        {
            upgrade_exit(msg);
        }
        break;

    default:
        break;
    }
}

/**
 * @brief  uart_rx_data_process_init
 * @param  recv 解析指针
 * @param  buf  解析数据缓存
 * @param  callback 回调函数指针
 * @retval None
 */
static void uart_rx_data_process_init(gcp_recv_t *recv, uint8_t *buf, void (*callback)(gcp_msg_t *))
{
    recv->msg = (gcp_msg_t *)buf;
    recv->callback = callback;

    gcp_recv_reset(recv);
}

/**
 * @brief  uart_rx_data_process
 * @param  recv 解析指针
 * @param  dat  待解析数据
 * @retval None
 */
static void uart_rx_data_process(gcp_recv_t *recv, uint8_t dat)
{
    switch (recv->state)
    {
    case RECV_DATA_MAGIC0:
        if (dat == GCP_MAGIC0)
        {
            recv->sum = 0;
            recv->msg->magic0 = dat;
            recv->state = RECV_DATA_MAGIC1;
        }
        break;

    case RECV_DATA_MAGIC1:
        if (dat == GCP_MAGIC1)
        {
            recv->msg->magic1 = dat;
            recv->state = RECV_DATA_CHECK;
        }
        else
        {
            gcp_recv_reset(recv);
        }
        break;

    case RECV_DATA_CHECK:
        recv->msg->check = dat;
        recv->state = RECV_DATA_TYPE;
        break;

    case RECV_DATA_TYPE:
        recv->sum += dat;
        recv->msg->type = dat;
        recv->state = RECV_DATA_SRC_MODULE;
        break;

    case RECV_DATA_SRC_MODULE:
        recv->sum += dat;
        recv->msg->src_module = dat;
        recv->state = RECV_DATA_DST_MODULE;
        break;

    case RECV_DATA_DST_MODULE:
        recv->sum += dat;
        recv->msg->dst_module = dat;
        recv->state = RECV_DATA_FC;
        break;

    case RECV_DATA_FC:
        recv->sum += dat;
        recv->msg->fc = dat;
        recv->state = RECV_DATA_CONTENT_LEN;
        break;

    case RECV_DATA_CONTENT_LEN:
        recv->sum += dat;
        recv->msg->content_len = dat;

        if (recv->msg->content_len == 0)
        {
            if (recv->sum == recv->msg->check)
                recv->callback(recv->msg);
            gcp_recv_reset(recv);
        }
        else
        {
            recv->state = RECV_DATA_CONTENT;
        }
        break;

    case RECV_DATA_CONTENT:
        recv->msg->content[recv->content_index++] = dat;
        recv->sum += dat;

        if (recv->content_index == recv->msg->content_len)
        {
            if (recv->msg->check == recv->sum)
            {
                if (recv->sum == recv->msg->check)
                    recv->callback(recv->msg);
                gcp_recv_reset(recv);
            }
        }
        break;
    }
}

/**
 * @brief  gcp_recv_reset
 * 重置解析器
 * @param  recv 解析指针
 * @retval None
 */
static void gcp_recv_reset(gcp_recv_t *recv)
{
    recv->state = RECV_DATA_MAGIC0;
    recv->content_index = 0;
    recv->sum = 0;
}
