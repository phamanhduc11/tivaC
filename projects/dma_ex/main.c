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
#if defined(ccs)
#include "utils/uartstdio.c"
#else
#include "utils/uartstdio.h"
#endif

#define SYS_DMA_ENABLE *(volatile unsigned int *)(0x400FE000 + 0x60C) = 0x1
#define DMACFG_REG *(volatile unsigned int *)(0x400FF000 +0x4)
#define DMACTLBASE_REG *(volatile unsigned int *)(0x400FF000 +0x8)

#define DMAPRIOSET_REG *(volatile unsigned int *)(0x400FF000 +0x38)
#define DMAALTCLR_REG *(volatile unsigned int *)(0x400FF000 +0x34)
#define DMAUSEBURSTCLR *(volatile unsigned int *)(0x400FF000 +0x1C)
#define DMAREQMASKCLR *(volatile unsigned int *)(0x400FF000 +0x24)
#define DMAENASET_REG *(volatile unsigned int *)(0x400FF000 + 0x28)
#define DMASWREQ_REG *(volatile unsigned int *)(0x400FF000 + 0x14)


volatile uint32_t g_ui32MsgCount = 0;
volatile bool g_bErrFlag = 0;

#if defined(ccs)
#pragma DATA_ALIGN(strCtlTable, 1024)
uint8_t strCtlTable[1024];
#else
uint8_t strCtlTable[1024] __attribute__ ((aligned(1024)));
#endif

unsigned char dstBuffer[1024];
unsigned char srcBuffer[1024];
unsigned char testBuffer[1024];

unsigned char srcSimpleTransmit[64] = "abcdefghiklmnopqrstuvwyz";

struct DMAControlStructure {
    unsigned int lastSrc;
    unsigned int lastDst;
    union {
        unsigned int data;
        struct {
        unsigned int xferMode           : 3;
        unsigned int nxtUseBurst        : 1;
        unsigned int xferSize           : 10;
        unsigned int arbSize            : 4;
        unsigned int reserved_b23_b18   : 6;
        unsigned int srcSize            : 2;
        unsigned int srcInc             : 2;
        unsigned int dstSize            : 2;
        unsigned int dstInc             : 2;
        } field;
    } Control;
    unsigned int Reserved;
};

void UART0IntHandler(void){

}
void
InitConsole()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioConfig(0, 115200, 16000000);
}
void
SimpleDelay(void)
{
    SysCtlDelay(16000000 / 3);
}

int main()
    {
    int i = 0;
    int delay = 0;
    struct DMAControlStructure *DMAStrPointer = (struct DMAControlStructure *) &strCtlTable;
#if defined(TARGET_IS_TM4C129_RA0) ||                                         \
    defined(TARGET_IS_TM4C129_RA1) ||                                         \
    defined(TARGET_IS_TM4C129_RA2)
    uint32_t ui32SysClock;
#endif


#if defined(TARGET_IS_TM4C129_RA0) ||                                         \
    defined(TARGET_IS_TM4C129_RA1) ||                                         \
    defined(TARGET_IS_TM4C129_RA2)
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                       SYSCTL_OSC_MAIN |
                                       SYSCTL_USE_OSC)
                                       25000000);
#else
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
#endif
    InitConsole();
    /*
        DMA Initialization
    */
    SYS_DMA_ENABLE;
    DMACFG_REG = 0x1;
    DMACTLBASE_REG = (unsigned int) &strCtlTable;
    /*
        Software 
        DMA Channel 30 (SW) Configuration (Mem-Mem)
    */
    DMAPRIOSET_REG = DMAPRIOSET_REG | 1 << 30; 
    DMAALTCLR_REG = DMAALTCLR_REG | 1 << 30;
    DMAUSEBURSTCLR  = DMAUSEBURSTCLR | 1 << 30;
    DMAREQMASKCLR = DMAREQMASKCLR | 1 << 30;
    // Configure DMA Structure of Channel 30
    DMAStrPointer[30].lastSrc           = (unsigned int) &srcBuffer + 0x3FC;
    DMAStrPointer[30].lastDst           = (unsigned int) &dstBuffer + 0x3FC;
    DMAStrPointer[30].Control.field.dstInc    = 2; // inc 4 adress 
    DMAStrPointer[30].Control.field.dstSize   = 2; // item size:  4 bytes
    DMAStrPointer[30].Control.field.srcInc    = 2;
    DMAStrPointer[30].Control.field.srcSize   = 2;
    DMAStrPointer[30].Control.field.arbSize   = 3; // Arbitrates after 8 transfers
    DMAStrPointer[30].Control.field.xferSize  = 255; // transfer 256 items
    DMAStrPointer[30].Control.field.nxtUseBurst = 0; // 
    DMAStrPointer[30].Control.field.xferMode  = 2; // Auto-request transfer

    // Do someshit for the srcBuffer before run trigger SW DMA
    for (i = 0; i< 1024; i++) {
        srcBuffer[i] = (unsigned char) i;
    }
    // Trigger
    DMAENASET_REG = DMAENASET_REG | 1 << 30;
    DMASWREQ_REG  = DMASWREQ_REG | 1 << 30;
//    while(DMAStrPointer[30].Control.field.xferMode){
//        delay++;
//    }
    // SimpleDelay();

    #if 1
    for (i = 0; i< 1024; i++) {
        UARTprintf("Index %d : %x %x %x \r\n", i, srcBuffer[i], dstBuffer[i], testBuffer[i]);
    }
    #endif

    UARTprintf("Print only one time %x \r\n", DMAStrPointer[30].Control.data);
    /*
        Simple Transmit
    */
    DMAPRIOSET_REG = DMAPRIOSET_REG | 1 << 7; 
    DMAALTCLR_REG = DMAALTCLR_REG | 1 << 7;
    DMAUSEBURSTCLR  = DMAUSEBURSTCLR | 1 << 7;
    DMAREQMASKCLR = DMAREQMASKCLR | 1 << 7;
    DMAStrPointer[7].lastSrc           = (unsigned int) srcSimpleTransmit+ 0x3F;
    DMAStrPointer[7].lastDst           = (unsigned int) 0x4000C000;
    DMAStrPointer[7].Control.field.dstInc    = 3;
    DMAStrPointer[7].Control.field.dstSize   = 0;
    DMAStrPointer[7].Control.field.srcInc    = 0;
    DMAStrPointer[7].Control.field.srcSize   = 0;
    DMAStrPointer[7].Control.field.arbSize   = 2;
    DMAStrPointer[7].Control.field.xferSize  = 63;
    DMAStrPointer[7].Control.field.nxtUseBurst = 0;
    DMAStrPointer[7].Control.field.xferMode  = 1;
    #include "inc/hw_types.h"
    while(!(HWREG(0x4000C000 + 0x18) & 0x00000080))
    {
        delay++;
    }
#define UARTIFLS_REG *(volatile unsigned int*)0x4000C034
#define DMACHMAP1_REG *(volatile unsigned int*)0x4000C514
    UARTIFLS_REG = (UARTIFLS_REG & ~0x7);
    DMAENASET_REG = DMAENASET_REG | 1 << 7;
//    DMASWREQ_REG  = DMASWREQ_REG | 1 << 7;
    UARTprintf("Done!\r\n");
    while(1)
    {
    }

    return 0;

}
