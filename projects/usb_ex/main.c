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

#include "INC/usb/usbd_framework.h"


void UART0IntHandler(void){
}

volatile uint32_t g_ui32Counter = 0;

void
SysTickIntHandler(void)
{
    //
    // Update the Systick interrupt counter.
    //
    g_ui32Counter++;
    UARTprintf("Number of interrupts: %d\r", g_ui32Counter);
}

void
USB0IntHandler(void) {
    USBInterruptStatus();
}

void
InitConsole()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_SYSTEM);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // UARTStdioConfig(0, 115200, 16000000);
    UARTStdioConfig(0, 921600, MAP_SysCtlClockGet());
}
void
SimpleDelay(void)
{
    SysCtlDelay(50000000);
}

USBDevice usbdevice;
uint8_t usbIN[256]  = {0,};
uint8_t usbOUT[256] = {0,};

void USBDeviceDataInit(void) {
    usbdevice.device_state = 0;
    usbdevice.control_transfer_stage = 0;
    usbdevice.configuration_value = 0;
    // for ptr_in_buffer, we may not need real buffer for this
    usbdevice.ptr_in_buffer = (unsigned char*) 0;
    usbdevice.ptr_out_buffer = usbOUT;
    usbdevice.in_data_size = 0;
    usbdevice.out_data_size = 0;
}

int main()
{
    uint32_t ui32PrevCount = 0;
    // Set 80MHz
    MAP_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_INT | SYSCTL_XTAL_16MHZ);

    InitConsole();
    USBDeviceDataInit();
    SysTickPeriodSet(10000000); //16777215
    //
    // Enable interrupts to the processor.
    //
    IntMasterEnable();

    //
    // Enable the SysTick Interrupt.
    //
    SysTickIntEnable();

//    IntPrioritySet(FAULT_SYSTICK,0x40);
//    IntPrioritySet(INT_USB0,0x80);
    UARTprintf("Interrupt priority Systick = %d USB = %d\r\n", IntPriorityGet(FAULT_SYSTICK), IntPriorityGet(INT_USB0));
    //
    // Enable SysTick.
    //
    SysTickEnable();

    USBDInit(&usbdevice);
    UARTprintf("Test 2\n");
    UARTprintf("Sysclock = %d\r\n", SysCtlClockGet());
    
    while(1)
    {
        if(ui32PrevCount != g_ui32Counter)
        {
            //
            // Print the interrupt counter.
            //
            UARTprintf("Number of interrupts: %d\r", g_ui32Counter);
            ui32PrevCount = g_ui32Counter;
        }
    }

}
