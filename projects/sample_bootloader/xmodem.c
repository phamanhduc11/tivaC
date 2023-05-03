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

#include "INC/console.h"
#include "INC/Global.h"
#include "INC/flasher.h"

// #define BUFFERSIZE 2*1024
#define BUFFERSIZE 2*1024
#define TOP_RAM_ADDR 0x20008000
#define XMODEM_BUFFER_ADDR (TOP_RAM_ADDR - BUFFERSIZE)

#define XMODEM_DATABLOCK_SIZE 128

#define SOH 0x01
#define EOT 0x04
#define ACK 0x06
#define NACK 0x15
#define ETB 0x17
#define CAN 0x18

// #if defined(ccs)
// extern uint32_t __STACK_TOP;
// //uint8_t *xmodemBuffers = (uint8_t*) (((uint32_t)&__STACK_TOP + 0x400) & 0x3ff);
// uint8_t *xmodemBuffers = (uint8_t*) (((uint32_t)&__STACK_TOP + 0x400));
// #else
// extern unsigned long pulStack[64];
// // uint8_t *xmodemBuffers = (uint8_t*) (((unsigned long) pulStack + sizeof(pulStack) +0x400) & 0x3ff);
// uint8_t *xmodemBuffers = (uint8_t*) (((unsigned long) pulStack + sizeof(pulStack) +0x400) & 0x3ff);
// // uint8_t *xmodemBuffers = 0x12345678 & 0x3ff;
// #endif

volatile bool newData = false;
volatile uint16_t modemRxIndex = 0;
volatile uint8_t *modemBuffer = XMODEM_BUFFER_ADDR;
volatile uint8_t xmodemHeader[4] = {0};
volatile uint8_t xmodemBufferBlockNum = 0;

static uint32_t u32ModemTimer = 0;
bool bModemRunning = false;

volatile uint32_t debugIndex = 0;
uint8_t *modemDebugBuffer = XMODEM_BUFFER_ADDR - 1024;


// | SOH | PACK_NUM | ~PACK_NUM | DATA[128] | CSUM |
void xmodemReceiveHandler(uint8_t rcvData) {
    static bool isValidXmodem = false;

    //debug
    // modemDebugBuffer[debugIndex++] = rcvData;
    // if(debugIndex > 132) debugIndex = 0;

    if((rcvData == SOH) && !isValidXmodem) 
        isValidXmodem = true;

    if((rcvData == EOT) && !isValidXmodem)
        xmodemHeader[0] = rcvData;

    if(isValidXmodem) {
        switch (modemRxIndex) {
        case 0: // 1st data
        case 1: // 2nd data
        case 2: // 3rd data
            xmodemHeader[modemRxIndex] = rcvData;
            break;
        case 131: // last data
            xmodemHeader[3] = rcvData;
            newData = true;
            isValidXmodem = false;
            break;
        default:
            modemBuffer[xmodemBufferBlockNum*128 + (modemRxIndex - 3)] = rcvData;
            break;
        }

        ++modemRxIndex >= 132 ? modemRxIndex = 0 : modemRxIndex;

        // modemBuffer[modemRxIndex++] = rcvData;
        // if(modemRxIndex == (1 + 1 + 1 + 128 + 1)) {
        //     newData = true;
        //     isValidXmodem = false;
        // }
    }
}

uint8_t checkSum(uint8_t *pData, uint32_t u32Size) {
    uint8_t crc_ret = 0;
    uint32_t index = 0;
    while(index < u32Size) {
        crc_ret += pData[index++];
    }
    return crc_ret;
}

void cleanData(void) {
    uint32_t index = TOP_RAM_ADDR - (XMODEM_BUFFER_ADDR - 1024) - 1;
    while(index != 0) {
        modemDebugBuffer[index] = 0xff;
        index--;
    }
}

void clearHeader(void) {
    xmodemHeader[0] = 0xa0;
    xmodemHeader[1] = 0x0a;
    xmodemHeader[2] = 0xa0;
    xmodemHeader[3] = 0x0a;

}

void xmodemReceive(void) {
    uint8_t pollingFlag = true;
    uint8_t pollTime = 0;
    bool    isValidPacket = false;
    uint32_t blockOffset = 0;
    uint32_t xmodemBlockNum = 1;

    cleanData();
    xmodemBufferBlockNum = 0;
    bModemRunning = true;

    while(pollingFlag) {
        u32ModemTimer = g_u321sSystick;
        UARTSend(NACK);

        while(g_u321sSystick - u32ModemTimer < 10) {
            if(newData == true) {
                bool isValidData = false;
                uint8_t checksum = 0;

                isValidPacket = ((xmodemHeader[0] == SOH) && (xmodemHeader[1] + xmodemHeader[2] == 0xff) && (xmodemHeader[1] == xmodemBlockNum)) ? true : false;

                if(isValidPacket) {
                    checksum = checkSum(modemBuffer + xmodemBufferBlockNum*128, 128);
                    if(checksum == xmodemHeader[3])
                        isValidData = true;

                    if(true == isValidData) {
                        // program

                        u32ModemTimer = g_u321sSystick;
                        ++xmodemBufferBlockNum ;
                        ++xmodemBlockNum >= 256 ? xmodemBlockNum = 0 : xmodemBlockNum;
                        if(xmodemBufferBlockNum == 8) {
                            flashUpdate(modemBuffer , APP_ADDR + (blockOffset*1024), 1024);

                            blockOffset++;
                            xmodemBufferBlockNum = 0;
                        }
                    }
                }

//                clearHeader();
                newData = false;

                if(true == isValidData) {
                    UARTSend(ACK);
                }
                else {
                    UARTSend(NACK);
                }
            }

            if(xmodemHeader[0] == EOT) {
                // End of Transmit
//                clearHeader();
                UARTSend(ACK);


                // left data to flash
                if(xmodemBufferBlockNum) {
                    flashUpdate(modemBuffer , APP_ADDR + (blockOffset*1024), xmodemBufferBlockNum*128);
                }
                goto exit_xmodem;
            }
        }

        pollTime++;
        if(pollTime >= 6) {
            break;
        }
    }

exit_xmodem:
    bModemRunning = false;
}
