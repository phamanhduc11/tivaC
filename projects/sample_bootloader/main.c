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

#define BOOTLOADER	1
#define APP			0

volatile uint8_t gRemainInBoot = false;
volatile uint32_t g_ui32Counter = 0;

void
SysTickIntHandler(void)
{
    //
    // Update the Systick interrupt counter.
    //
    g_ui32Counter++;
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
    MAP_IntMasterEnable();

    //
    // Enable the UART interrupt.
    //
    MAP_IntEnable(INT_UART0);
    MAP_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    //
    // Enable the SysTick Interrupt.
    //
    SysTickIntEnable();

    //
    // Enable SysTick.
    //
    SysTickEnable();
}

void InterruptEnd(void) {
    // disable intmaster
    // disable uart
    // disable systick
}

int main(void)
{
    // Set 80MHz
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	UARTConsoleInit();

    InterruptInit();
    

	while(1)
    {
        if(gRemainInBoot) {
            ConsoleHandler();
        }
    }
}
