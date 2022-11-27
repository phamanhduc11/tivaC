#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
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

#if DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    UARTprintf("[DEBUG] %s:%d\r\n", pcFilename, ui32Line);
//    while(1);
}
#endif

void InitConsole()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_SYSTEM);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
//    UARTStdioConfig(0, 115200, 80000000);
    UARTStdioConfig(0, 115200, MAP_SysCtlClockGet());
}

void main(void){
    uint8_t testdata = 0x71;
#if defined(gcc)
    PAD_SysClockSet();
#else
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_INT |
                       SYSCTL_XTAL_16MHZ);
#endif
    I2C_Init();
    InitConsole();
    // I2C_WriteBytes(0x13, 1, &testdata);
    UARTprintf("Test 1\n");
    UARTprintf("Test 2\n");
    UARTprintf("Clock=%d\r\n", SysCtlClockGet());
}
