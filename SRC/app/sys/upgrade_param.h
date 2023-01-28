#pragma once

#include "gcp.h"
#include "protocol.h"

#include <stdint.h>
#include <string.h>

#define UPGRADE_STATE_BOOT    0X00
#define UPGRADE_STATE_APP     0X01
#define UPGRADE_STATE_UPGRADE 0X02

typedef struct
{
    uint8_t fw_version[3];
    uint8_t hw_version[3];
    uint8_t upgrade_state;
    uint8_t param_check;
} upgrade_param_t;

extern upgrade_param_t upgrade_param;

void upgrade_param_save(void);
void upgrade_param_init(void);
void upgrade_param_calc_checksum(upgrade_param_t *upgrade_param);
