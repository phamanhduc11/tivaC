#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_ssi.h"
#include "inc/hw_i2c.h"
#include "inc/hw_can.h"
#include "inc/hw_nvic.h"
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

#include "INC/Global.h"
#if defined(ccs)
 #include "INC/console.h"
#include "utils/uartstdio.c"
#else
#include "utils/uartstdio.h"
#include "console.c"
#include "xmodem.c"
#endif



#define BOOTLOADER	1
#define APP			0

volatile uint8_t gRemainInBoot = false;
volatile uint32_t g_u321msSystick = 0;
volatile uint32_t g_u321sSystick = 0;

void
SysTickIntHandler(void)
{
    //
    // Update the Systick interrupt counter.
    //
    g_u321msSystick++;

    if(g_u321msSystick % 1000 == 0) {
        g_u321sSystick++;
    }
}

void UARTConsoleInit(void) {
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
	// 					(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
	// 					UART_CONFIG_PAR_NONE));
    UARTStdioConfig(0, 921600, MAP_SysCtlClockGet());
	UARTprintf("---- Bootloader ----\r\n");
	UARTprintf("Clock Init: %dMhz\r\n", SysCtlClockGet() / 1000000);
}

void InterruptInit(void) {
    //
    // Enable processor interrupts.
    //
    ROM_IntMasterEnable();

    //
    // Enable the UART interrupt.
    //
    ROM_IntEnable(INT_UART0);
    ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    //
    // Enable the SysTick Interrupt.
    //
    ROM_SysTickPeriodSet(SysCtlClockGet() / 1000);
    ROM_SysTickIntEnable();
    ROM_SysTickEnable();
}

void InterruptEnd(void) {
    //
    // Disable processor interrupts.
    //
    ROM_IntMasterDisable();

    //
    // Disable the UART interrupt.
    //
    ROM_IntDisable(INT_UART0);
    ROM_UARTIntDisable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    //
    // Disable the SysTick Interrupt.
    //
    ROM_SysTickIntDisable();
    ROM_SysTickDisable();
}



void JumpToApp(uint32_t ui32StartAddr) {
    //
    // Set the vector table to the beginning of the app in flash.
    //
    HWREG(NVIC_VTABLE) = ui32StartAddr;

    //
    // Load the stack pointer from the application's vector table.
    //
    __asm("    ldr     r1, [r0]\n"
          "    mov     sp, r1\n");

    //
    // Load the initial PC from the application's vector table and branch to
    // the application's entry point.
    //
    __asm("    ldr     r0, [r0, #4]\n"
          "    bx      r0\n");
}

int main(void)
{
    // Set 80MHz
    uint8_t u8ConsolePollTime = 0;

    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	UARTConsoleInit();

    InterruptInit();

    u8ConsolePollTime = g_u321sSystick;

    while(((g_u321sSystick - u8ConsolePollTime) < 5) || gRemainInBoot) {
        if(gRemainInBoot){
            UARTprintf("Console Running\r\n");
            while(true) {
                ConsoleHandler();
            }
        }
    }

    UARTprintf("Jump to App\r\n");
    if ((*(uint32_t *) (APP_ADDR+4) != 0xffffffff)) {
        InterruptEnd();
        JumpToApp(APP_ADDR);
    }
    else {
        UARTprintf("Invalid App\r\n");
    }
	while(1);
}
