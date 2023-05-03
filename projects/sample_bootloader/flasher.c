#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_ssi.h"
#include "inc/hw_i2c.h"
#include "inc/hw_can.h"
#include "driverlib/debug.h"
#include "driverlib/can.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/ssi.h"
#include "driverlib/i2c.h"
#include "driverlib/flash.h"

#include "INC/console.h"
#include "INC/Global.h"

#define BLOCKSIZE 1024

uint8_t flashUpdate(void *src, uint32_t dst, uint32_t nSize) {
    uint8_t sts = 0x0;
    uint32_t blockNum = (((nSize-1) & ~0x3ff) >> 10) + 1;
    uint32_t blockIndex = 0;


    if((dst < 0x00008000) || (dst > 0x00040000)) {
        sts = 0x1;
        goto ret;
    }

    while(blockIndex < blockNum) {
        FlashErase(dst + blockIndex*1024);
        blockIndex++;
    }

    FlashProgram((uint32_t *) src, dst, nSize);

    ret:
    return sts;
}
