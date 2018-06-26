
#ifndef __SOFTLED_H
#define __SOFTLED_H

#include <iocc2541.h>
#include "common.h"
#include "hal_types.h"

void softled_init(void);
void softled_mode_set(uint8 mode);
void softled_enable_set(uint8 flag);
void softled_kill_set(uint8 flag);
uint8 softled_get_key(void);

#endif

