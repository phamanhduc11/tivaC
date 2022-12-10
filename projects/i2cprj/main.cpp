#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
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
#include "INC/device/eeprom.h"
#include "INC/debug.h"

uint8_t cData[8192] = {0};



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
    I2C_WriteBytes(AT24C64_ADDR, 2, startAddr); // mulbytes Eeprom read ok
    I2C_ReadBytes(AT24C64_ADDR, 8192, cData);   // Eeprom read ok
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
    I2C_Init();
    setEEPROMProtocol(I2CMode);
    memset(cData, 0xAA, sizeof(uint8_t)*8192);
    // I2C Eeprom
    eepromDump(8192, cData);
    eepromRead(0x20, 3, testbuff);
    i = 8192*10;
    while(--i);
    // eepromWrite(0x1000, 8, mulbytes); // Common in case of too many write times due to reboot
    i = 8192*10;
    while(--i);
    printf("---------------CHECK----------------\r\n");
    eepromDump(8192, cData);
    //



    UARTprintf("Clock=%d\r\n", SysCtlClockGet());
    while(1);
    return 0;
}
