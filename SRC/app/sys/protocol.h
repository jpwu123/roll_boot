#pragma once

#include "gcp.h"
#include <stdint.h>

// [FLASH]
// APP偏移地址
#define APP_ADDR_OFC_CAMERA   0x2800 // 10K
#define APP_ADDR_OFC_MOTOR    0x2800 // 10K
#define APP_ADDR_OFC_HANDLE   0x2800 // 10K
// 参数偏移地址
#define PARAM_ADDR_OFC_CAMERA 0x01F800
#define PARAM_ADDR_OFC_MOTOR  0x00F800
#define PARAM_ADDR_OFC_HANDLE 0x01F800

// [PARAM]
// 参数码
#define PC_01_PROGRAM      0x01 // 程序选项(BOOT/APP)
#define PC_02_BOOT_SRC     0x02 // 固件升级源模块(MOBILE/PC)
#define PC_03_HW_VERSION   0x03 // 固件版本(x.x.x)
#define PC_04_FW_VERSION   0x04 // 硬件版本(x.x.x)
// PC_01_PROGRAM
#define PROGRAM_00_NONE    0x00
#define PROGRAM_01_BOOT    0x01 // 运行BOOT程序
#define PROGRAM_02_APP     0x02 // 运行APP程序
// PC_02_BOOT_SRC
#define BOOT_SRC_00_NONE   0x00
#define BOOT_SRC_01_MOBILE 0x01
#define BOOT_SRC_02_PC     0x02

// [GCP]
// MLG模块定义
#define GCP_MODULE_01_CAMERA    0x01 // 相机控制器
#define GCP_MODULE_02_PIT       0x02 // 俯仰电机
#define GCP_MODULE_03_ROL       0x03 // 横滚电机
#define GCP_MODULE_04_HANDLE    0x04 // 手柄(航向电机)
#define GCP_MODULE_05_MOBILE    0x05 // 移动端(App)
#define GCP_MODULE_06_PC        0x06 // PC端(上位机)
#define GCP_MODULE_07_GIMBAL    0x07 // 云台(相机控制器+俯仰电机+横滚电机+手柄)
#define GCP_MODULE_08_MOTOR     0x08 // 云台电机(俯仰电机+横滚电机+航向电机)
// MLG功能码定义
#define GCP_FC_00_DEVICE_INFO   0x00 // 设备信息
#define GCP_FC_01_BOOT_ENTER    0x01 // 进入固件升级模式
#define GCP_FC_02_BOOT_START    0x02 // 开始固件升级
#define GCP_FC_03_BOOT_FW_BLOCK 0x03 // 固件块烧录
#define GCP_FC_04_BOOT_END      0x04 // 结束固件升级
#define GCP_FC_05_BOOT_EXIT     0x05 // 退出固件升级模式
#define GCP_FC_06_FW_VERSION    0x06 // 固件版本
#define GCP_FC_0B_SHUTDOWN      0x0B // 关机

// [GCP_FC_00_DEVICE_INFO]
// device_id
#define GCP_DEVICE_ID_01_MLG       0x01
#define GCP_DEVICE_ID_02_MC5       0x02
#define GCP_DEVICE_ID_03_M21       0x03
// program_status
#define GCP_PROGRAM_STATUS_01_BOOT 0x01
#define GCP_PROGRAM_STATUS_02_APP  0x02
#define GCP_PROGRAM_STATUS_03_TEST 0x03
typedef union {
    struct {
        uint8_t device_id;      // 设备ID
        uint8_t program_status; // 程序状态
        uint8_t hw_version[3];  // 硬件版本
        uint8_t fw_version[3];  // 固件版本
        uint8_t uid[12];        // 设备UID最多12字节
    } notify;
} gcp_content_device_info_t;

// [GCP_FC_01_BOOT_ENTER]
// rsp
#define GCP_BOOT_ENTER_RSP_00_SUCCESS 0x00 // 成功
#define GCP_BOOT_ENTER_RSP_01_FAILURE 0x01 // 失败
typedef union {
    struct {
        uint8_t rsp;
    } ack;
} gcp_content_boot_enter_t;

// [GCP_FC_02_BOOT_START]
// rsp
#define GCP_BOOT_START_RSP_00_SUCCESS   0x00 // 成功
#define GCP_BOOT_START_RSP_01_ADDR_ERR  0x01 // 地址错误
#define GCP_BOOT_START_RSP_02_ERASE_ERR 0x02 // 擦除错误
typedef union {
    struct {
        uint32_t base_addr;
        uint32_t fw_size;
    } cmd;
    struct {
        uint8_t rsp;
    } ack;
} gcp_content_boot_start_t;

// [GCP_FC_03_BOOT_FW_BLOCK]
// rsp
#define GCP_BOOT_FW_BLOCK_RSP_00_SUCCESS     0x00 // 成功
#define GCP_BOOT_FW_BLOCK_RSP_01_ADDR_ERR    0x01 // 地址错误
#define GCP_BOOT_FW_BLOCK_RSP_02_PROGRAM_ERR 0x02 // 编程失败
typedef union {
    struct {
        uint32_t offset_addr; // 偏移地址
        uint8_t block[128];   // 固件块数据
        uint8_t size;         // 固件块大小
    } cmd;
    struct {
        uint32_t offset_addr;
        uint8_t rsp;
    } ack;
} gcp_content_boot_fw_block_t;

// [GCP_FC_04_BOOT_END]
// rsp
#define GCP_BOOT_END_RSP_00_SUCCESS    0x00 // 成功
#define GCP_BOOT_END_RSP_01_VERIFY_ERR 0x01 // 校验错误
typedef union {
    struct {
        uint16_t checksum;
    } cmd;
    struct {
        uint8_t rsp;
    } ack;
} gcp_content_boot_end_t;

// [GCP_FC_05_BOOT_EXIT]
// rsp
#define GCP_BOOT_EXIT_RSP_00_SUCCESS 0x00 // 成功
#define GCP_BOOT_EXIT_RSP_01_FAILURE 0x01 // 失败
typedef union {
    struct {
        uint8_t rsp;
    } ack;
} gcp_content_boot_exit_t;

// [GCP_FC_06_FW_VERSION]
typedef union {
    struct {
        uint8_t version[3];
    } set;
    struct {
        uint8_t version[3];
    } notify;
} gcp_content_fw_version_t;

// [GCP_FC_0B_SHUTDOWN]
// notify
