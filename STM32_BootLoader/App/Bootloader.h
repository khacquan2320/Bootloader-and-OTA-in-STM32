#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include "stm32f1xx.h"
#include "Bootloader_command.h"


void bootloader_handle(void);
void bootloader_init(void);
void bootloader_request_update();
#endif
