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

#include "INC/Global.h"
#include "INC/console.h"
#include "utils/uartstdio.h"

volatile uint8_t UARTRxBuffers[128] = {0};
volatile uint8_t g_rxIndex = 0;

uint32_t preMenuLevel = CONSOLE_MENU_LEVEL;
uint32_t curMenuLevel = CONSOLE_MENU_LEVEL;

void consoleMenu(void);
void consoleMenuHandler(uint8_t command);

uint32_t CommandList[CONSOLE_MENU_LEVEL_MAX][3] = {
    // Type, Menu, Handler
    {CONSOLE_MENU_LEVEL                   , consoleMenu, consoleMenuHandler},
    {CONSOLE_MENU_FLASH_LEVEL             , 0, 0},
};

void consoleMenuHandler(uint8_t command) {
    switch (command) {
        case 1:
            // boot to app
            break;
        case 2:
            preMenuLevel = curMenuLevel;
            curMenuLevel = CONSOLE_MENU_FLASH_LEVEL;
    }

}

void consoleMenu(void) {
    UARTprintf("1.Boot\r\n");
    UARTprintf("2.Flash\r\n");
}

void UARTHandler(void) {
    uint32_t ui32Status;
    int8_t  rxData = 0xff;
    //
    // Get the interrrupt status.
    //
    ui32Status = MAP_UARTIntStatus(UART0_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    MAP_UARTIntClear(UART0_BASE, ui32Status);

    while(MAP_UARTCharsAvail(UART0_BASE))
    {
        //
        // Read the next character from the UART and write it back to the UART.
        //
        rxData = MAP_UARTCharGetNonBlocking(UART0_BASE);

        if (rxData == '\r' && !gRemainInBoot) {
            gRemainInBoot = true;
            continue;
        }

        if ((true == gRemainInBoot) && (0xff != rxData)) {
            UARTRxBuffers[g_rxIndex] = rxData;

            g_rxIndex = (++g_rxIndex) % 128;
        }
    }
}

void UARTSend (uint8_t data) {
    UARTCharPut(UART0_BASE, data);
}

void ConsolePrint(void) {
    void (*MenuFn)(void) = (void (*)(void)) CommandList[curMenuLevel][1];
    MenuFn();
}

void ConsoleHandler(void) {
    static uint8_t sRxIndex = 0;
    uint8_t u8RcvRx = 0;
    while (sRxIndex != g_rxIndex) {
        if ((20 < UARTRxBuffers[sRxIndex]) || (UARTRxBuffers[sRxIndex] < 127)) {
            UARTSend(UARTRxBuffers[sRxIndex]);
            // u8RcvRx = UARTRxBuffers[sRxIndex];

        if('\r' == UARTRxBuffers[sRxIndex])
            UARTSend('\n');
        }

        u8RcvRx = UARTRxBuffers[sRxIndex];
        ++sRxIndex >= 128 ? sRxIndex = 0 : sRxIndex;
    }

    if('\r' == u8RcvRx) {
        ConsolePrint();
        UARTprintf(">");
    }

    if(0) {
        void (*MenuHandleFn)(void) = (void (*)(void)) CommandList[curMenuLevel][2];
    }
}
