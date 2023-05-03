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
#include "INC/xmodem.h"
#include "utils/uartstdio.h"

volatile uint8_t UARTRxBuffers[128] = {0};
volatile uint8_t g_rxIndex = 0;

uint32_t preMenuLevel = CONSOLE_MENU_LEVEL;
uint32_t curMenuLevel = CONSOLE_MENU_LEVEL;

void consoleMenu(void);
void consoleFlashMenu(Void);

void consoleMenuHandler(uint8_t command);
void consoleFlashMenuHandler(uint8_t command);

uint32_t CommandList[CONSOLE_MENU_LEVEL_MAX][3] = {
    // Type, Menu, Handler
    {CONSOLE_MENU_LEVEL                   , consoleMenu, consoleMenuHandler},
    {CONSOLE_MENU_FLASH_LEVEL             , consoleFlashMenu, consoleFlashMenuHandler},
};

void consoleFlashMenuHandler(uint8_t command) {
    switch (command)
    {
    case 1:
        UARTprintf("XMODEM Receiving!\r\n");
        xmodemReceive();
        UARTprintf("\r\nXMODEM Flash Done!\r\n");
        break;
    case 2: 
    {
        UARTprintf("Empty command!\r\n");
        break;
    }
    case 3:
    {
        FlashErase(0x00030000);
        UARTprintf("Erase\r\n");
    }
    case 9:
        preMenuLevel = CONSOLE_MENU_LEVEL;
        curMenuLevel = CONSOLE_MENU_LEVEL;
        break;
    default:
        break;
    }
}

void consoleFlashMenu(void) {
    UARTprintf("1. Load FW\r\n");
    UARTprintf("2. Program FW\r\n");
    UARTprintf("3. Erase Flash");
}

void consoleMenuHandler(uint8_t command) {
    switch (command) {
        case 1:
            // boot to app
            UARTprintf("Jump to App\r\n");
            InterruptEnd();
            JumpToApp(APP_ADDR);
            break;
        case 2:
            preMenuLevel = curMenuLevel;
            curMenuLevel = CONSOLE_MENU_FLASH_LEVEL;
            break;
    }

}

void consoleMenu(void) {
    UARTprintf("1.Boot\r\n");
    UARTprintf("2.Flash\r\n");
}

void UART0IntHandler(void) {
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

        if(true == bModemRunning) {
            xmodemReceiveHandler(rxData);
        }
    }
}

void UARTSend (uint8_t data) {
    UARTCharPut(UART0_BASE, data);
}

void ConsolePrint(void) {
    void (*MenuFn)(void) = (void (*)(void)) CommandList[curMenuLevel][1];
    MenuFn();
    UARTprintf("Current level %d\r\n", curMenuLevel);
    UARTprintf(">");
}

void ConsoleHandler(void) {
    static uint8_t sRxIndex = 0;
    static uint8_t preRxIndex = 0;
    uint8_t u8RcvRx = 0;
    uint8_t u8PreRcvRx = 0;
    while (sRxIndex != g_rxIndex) {
        if ((20 < UARTRxBuffers[sRxIndex]) || (UARTRxBuffers[sRxIndex] < 127)) {
            UARTSend(UARTRxBuffers[sRxIndex]);
            // u8RcvRx = UARTRxBuffers[sRxIndex];

        if('\r' == UARTRxBuffers[sRxIndex])
            UARTSend('\n');
        }

        u8PreRcvRx = UARTRxBuffers[preRxIndex];
        u8RcvRx = UARTRxBuffers[sRxIndex];
        preRxIndex = sRxIndex;
        ++sRxIndex >= 128 ? sRxIndex = 0 : sRxIndex;
    }

    if('\r' == u8RcvRx && ((48 < u8PreRcvRx) && (u8PreRcvRx < 58))) {
        void (*MenuHandleFn)(uint8_t) = (void (*)(uint8_t)) CommandList[curMenuLevel][2];
        MenuHandleFn(u8PreRcvRx - 48);
    }

    if('\r' == u8RcvRx) {
        ConsolePrint();
    }

}
