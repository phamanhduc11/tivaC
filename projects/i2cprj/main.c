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

#if defined(gcc)
#if DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif
#endif

// void PAD_SysClockSet(void) {
//     uint32_t u32Delay = 0;
//     // uint32_t u32RCC = SYSCTL_RCC_REG;
//     // uint32_t u32RCC2 = SYSCTL_RCC2_REG;

//     // Clear PLL status in Interrupt register
//     SYSCTL_MISC_REG |= (BIT6);

//     // Bypass PLL and system clock divider
//     SET_MASK_VAL(SYSCTL_RCC_REG, RCC_BYPASS_MASK, RCC_BYPASS_SET);
//     SET_MASK_VAL(SYSCTL_RCC2_REG, RCC2_BYPASS2_MASK, RCC_BYPASS_SET);
//     SET_MASK_VAL(SYSCTL_RCC_REG, RCC_USESYSDIV_MASK, RCC_USESYSDIV_CLR);

//     // Select XTAL and OSC source and enable PLL
//     SET_MASK_VAL(SYSCTL_RCC2_REG, RCC2_USERRCC2_MASK, 0x1);

//     SET_MASK_VAL(SYSCTL_RCC_REG, RCC_XTAL_MASK, RCC_XTAL_16_M);
//     SET_MASK_VAL(SYSCTL_RCC_REG, RCC_OSCSRC_MASK, RCC_OSCSRC_MAIN);
//     SET_MASK_VAL(SYSCTL_RCC2_REG, RCC2_OSCSRC2_MASK, RCC_OSCSRC_MAIN);
//     SET_MASK_VAL(SYSCTL_RCC_REG, RCC_PWRDN_MASK, RCC_PWRDN_CLR);
//     SET_MASK_VAL(SYSCTL_RCC2_REG, RCC2_PWRDN2_MASK, RCC_PWRDN_CLR);

//     //Set SYSDIV and USESYSDIV
//     SET_MASK_VAL(SYSCTL_RCC_REG, RCC_SYSDIV_MASK, RCC_SYSDIV_4);
//     SET_MASK_VAL(SYSCTL_RCC2_REG, RCC2_DIV400_MASK, 0x1);
//     SET_MASK_VAL(SYSCTL_RCC2_REG, RCC2_SYSDIV2_MASK, 0x2);
//     SET_MASK_VAL(SYSCTL_RCC2_REG, RCC2_SYSDIV2LSB_MASK, 0x0);

//     SET_MASK_VAL(SYSCTL_RCC_REG, RCC_USESYSDIV_MASK, RCC_USESYSDIV_SET);

//     // Wait PLL lock
//     for (u32Delay = 0x8000; u32Delay == 0; u32Delay--){
//         // if(SYSCTL_PLLSTAT_REG)
//         if(SYSCTL_RIS_REG & BIT6)
//         {
//             break;
//         }
//     }

//     // Enable PLL by clear BYPASS
//     SET_MASK_VAL(SYSCTL_RCC_REG, RCC_BYPASS_MASK, RCC_BYPASS_CLR);
//     SET_MASK_VAL(SYSCTL_RCC2_REG, RCC2_BYPASS2_MASK, RCC_BYPASS_CLR);
// }

void InitConsole()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_SYSTEM);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioConfig(0, 115200, 80000000);
}

void main(void){
    uint8_t testdata = 0x71;
    PAD_SysClockSet();
    I2C_Init();
    InitConsole();
    I2C_WriteBytes(0x13, 1, &testdata);
    UARTprintf("Test 1\n");
    UARTprintf("Test 2\n");
    UARTprintf("Clock=%d\r\n", SysCtlClockGet());
}