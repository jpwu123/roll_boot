#pragma once

#include "gcp.h"
#include "protocol.h"

#include <stdint.h>
#include <string.h>

void upgrade_enter(void);
void upgrade_start(gcp_msg_t *msg);
void upgrade_write_fw_block(gcp_msg_t *msg);
void upgrade_end(gcp_msg_t *msg);
void upgrade_set_fw_version(gcp_msg_t *msg);
void upgrade_exit(gcp_msg_t *msg);
