#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
// TivaWare
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

// Self-Drivers
#include "Global/Include.h"
#include "INC/sys.h"
#include "INC/i2c.h"
#include "INC/spi.h"
#include "INC/timer.h"
#include "INC/device/eeprom.h"
#include "INC/debug.h"

uint8_t cData[8192] = {0};
SPIInterface *spi;
TimerModule *timer;


void InitConsole()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_SYSTEM);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
//    UARTStdioConfig(0, 115200, 80000000);
    UARTStdioConfig(0, 921600, MAP_SysCtlClockGet());
}

void eepromDump(uint32_t size, uint8_t * dData) {
    uint32_t x;
    uint8_t startAddr[2] = {0,0};
    printf("Start of eepromDump\r\n");
    printf("      00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f\r\n");
    for (x = 0; x < size; x++) {
        if ((x%16 == 0)) {
            if(x) printf("\r\n");
            printf("%04x: ", x);
        }
        printf("%02x ", dData[x]);
    }
    printf("\r\nEnd of eepromDump\r\n");
}

extern "C" {
void SSIInterruptHandler(void) {
    spi->interruptHandler();
}

void Timer0InterruptHandler(void) {
    uint32_t intSts = timer->getIntStatus();
    printf("[PAD] test timer interrupt! %08x\r\n", intSts);
    timer->clearInt(intSts);
}
}

#include <vector>

int main(void){
    uint8_t testbuff[8] = {0xaa,0xaa};
    uint8_t mulbytes[8] = {0x8,0x9,0x0a,0xb,0xc,0xd,0xe,0xf};
    uint32_t i = 256;
    // while(--i) cData[i] = 0xAA;

#if defined(gcc)
    PAD_SysClockSet();
#else
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_INT | SYSCTL_XTAL_16MHZ);
#endif
    InitConsole();
    std::vector<uint8_t> test = {1,2,3,4};
    //uint8_t *testMalloc = new uint8_t[9];
    uint8_t *test2Malloc = (uint8_t *) malloc(1 * sizeof(uint8_t));
    printf("testMalloc = %x %x\r\n", test2Malloc);
    SPIInterface spi0(SPI0_DEV, SPI_Mode_0, 500000, 8);
    TimerModule timer0(GPT_Module_0, GPTM_MODE_PERIOD, 1);
    spi = &spi0;
    timer = &timer0;
    SystemDebug.setDebugLevel(DEBUG_LV0);
    SystemDebug.log(DEBUG_ERR, "This is for testing %d" , 5050);
    SystemDebug.log(DEBUG_WRN, "This is for testing %d" , 5050);

    MAP_IntMasterEnable();
    MAP_IntEnable(INT_TIMER0A);
    MAP_IntEnable(INT_TIMER0B);
    MAP_IntEnable(INT_SSI0);

    timer->setTimerBreakpointDebug(true);
    //timer->enable();

    spi0.setUseModuleFssPin(false);
    spi0.clearCSPin();
    spi0.interruptMaskSet(SSI_RORMIS | SSI_RXMIS);

    setEEPROMProtocol(SPIMode);

    memset(cData, 0xAA, sizeof(uint8_t)*8192);
    // SPI Eeprom
    // eepromDump(8192, cData);
    spi0.setCSPin();
    eepromRead(0x20, 3, testbuff);
    spi0.clearCSPin();

    i = 8192*10;
    while(--i);
    spi0.setCSPin();
    i = 8192*10;
    while(--i);

    eepromWriteEnable();

    i = 8192*10;
    while(--i);
    spi0.clearCSPin();
    i = 8192*10;
    while(--i);
    spi0.setCSPin();
    i = 8192*10;
    while(--i);

    eepromWrite(0x20, 8, mulbytes);

    i = 8192*10;
    while(--i);
    spi0.clearCSPin();
    i = 8192*10;
    while(--i);
    spi0.setCSPin();
    i = 8192*10;
    while(--i);

    eepromRead(0x20, 3, testbuff);

    printf("---------------CHECK----------------");
//    eepromDump(8192, cData);
    //

    UARTprintf("Clock=%d\r\n", SysCtlClockGet());
    while(1)
        {
        };
    return 0;
}
