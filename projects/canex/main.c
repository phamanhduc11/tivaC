<<<<<<< HEAD
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
#include "utils/uartstdio.h"

volatile uint32_t g_ui32MsgCount = 0;
volatile bool g_bErrFlag = 0;

void
InitConsole(float testvar)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioConfig(0, 115200, 16000000);
}
void
SimpleDelay(void)
{
    SysCtlDelay(16000000 / 3);
}
=======
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/can.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/debug.h"
#include "main.h"

// tx
volatile uint32_t g_ui32IntCount = 0;

volatile uint32_t g_ui32Msg1Count = 0;
volatile uint32_t g_ui32Msg2Count = 0;
volatile uint32_t g_ui32Msg3Count = 0;
volatile bool g_bMsgObj3Sent = 0;
volatile bool g_bErrFlag = 0;
tCANMsgObject g_sCANMsgObject1;
tCANMsgObject g_sCANMsgObject2;
tCANMsgObject g_sCANMsgObject3;
uint8_t g_pui8Msg1[4] = { 0, 0, 0, 0 };
uint8_t g_pui8Msg2[5] = { 2, 2, 2, 2, 2 };
uint8_t g_pui8Msg3[6] = { 3, 3, 3, 3, 3, 3 };
uint8_t g_pui8Msg4[8] = { 4, 4, 4, 4, 5, 5, 5, 5 };

// rw
volatile uint32_t g_ui32MsgRecvCount = 0;
volatile bool g_bRXFlag1 = 0;
volatile bool g_bRXFlag2 = 0;
volatile bool g_bRXFlag3 = 0;
//volatile bool g_bErrFlag = 0;

>>>>>>> ac7f5bf2e1a5e94e3365253217efd271569c0882
void
CANIntHandler(void)
{
    uint32_t ui32Status;
<<<<<<< HEAD
    ui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);
    if(ui32Status == CAN_INT_INTID_STATUS)
    {
        ui32Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
        g_bErrFlag = 1;
    }
    else if(ui32Status == 1)
    {
        g_ui32MsgCount++;
        g_bErrFlag = 0;
    }
    else
    {
        UARTprintf("\nAlways get here?\n");
    }
}

void CAN0Initialize(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);
    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
    CANInit(CAN0_BASE);
    #if defined(TARGET_IS_TM4C129_RA0) ||                                         \
    defined(TARGET_IS_TM4C129_RA1) ||                                         \
    defined(TARGET_IS_TM4C129_RA2)
    CANBitRateSet(CAN0_BASE, ui32SysClock, 500000);
#else
    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);
#endif
    CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
    IntEnable(INT_CAN0);
    CANEnable(CAN0_BASE);
}


int main()
{
#if defined(TARGET_IS_TM4C129_RA0) ||                                         \
    defined(TARGET_IS_TM4C129_RA1) ||                                         \
    defined(TARGET_IS_TM4C129_RA2)
    uint32_t ui32SysClock;
#endif

    tCANMsgObject sCANMessage;
    uint32_t ui32MsgData;
    uint8_t *pui8MsgData;
    volatile float testvar = 0;

    pui8MsgData = (uint8_t *)&ui32MsgData;

#if defined(TARGET_IS_TM4C129_RA0) ||                                         \
    defined(TARGET_IS_TM4C129_RA1) ||                                         \
    defined(TARGET_IS_TM4C129_RA2)
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                       SYSCTL_OSC_MAIN |
                                       SYSCTL_USE_OSC)
                                       25000000);
#else
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
#endif
    testvar = 1.2222;
    testvar += 13.424;

    InitConsole(testvar);
    CAN0Initialize();
    UARTprintf("Print only one time \n");
    ui32MsgData = 0;
    sCANMessage.ui32MsgID = 1;
    sCANMessage.ui32MsgIDMask = 0;
    sCANMessage.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    sCANMessage.ui32MsgLen = sizeof(pui8MsgData);
    sCANMessage.pui8MsgData = pui8MsgData;
    while(1)
    {
        UARTprintf("Sending msg: 0x%02X %02X %02X %02X",
                   pui8MsgData[0], pui8MsgData[1], pui8MsgData[2],
                   pui8MsgData[3]);

        CANMessageSet(CAN0_BASE, 1, &sCANMessage, MSG_OBJ_TYPE_TX);

        SimpleDelay();

=======

    //
    // Read the CAN interrupt status to find the cause of the interrupt
    //
    ui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

    //
    // If the cause is a controller status interrupt, then get the status
    //
    if(ui32Status == CAN_INT_INTID_STATUS)
    {
        //
        // Read the controller status.  This will return a field of status
        // error bits that can indicate various errors.  Error processing
        // is not done in this example for simplicity.  Refer to the
        // API documentation for details about the error status bits.
        // The act of reading this status will clear the interrupt.  If the
        // CAN peripheral is not connected to a CAN bus with other CAN devices
        // present, then errors will occur and will be indicated in the
        // controller status.
        //
        ui32Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);

        //
        // Set a flag to indicate some errors may have occurred.
        //
        g_bErrFlag = 1;
    }

    //
    // Check if the cause is message object 1, which is used for sending
    // message 1.
    //
    else if(ui32Status == 1)
    {
        //
        // Getting to this point means that the TX interrupt occurred on
        // message object 1, and the message TX is complete.  Clear the
        // message object interrupt.
        //
        CANIntClear(CAN0_BASE, 1);

        //
        // Increment a counter to keep track of how many messages have been
        // sent.  In a real application this could be used to set flags to
        // indicate when a message is sent.
        //
        g_ui32Msg1Count++;

        //
        // Since the message was sent, clear any error flags.
        //
        g_bErrFlag = 0;
    }

    //
    // Check if the cause is message object 2, which is used for sending
    // message 2.
    //
    else if(ui32Status == 2)
    {
        //
        // Getting to this point means that the TX interrupt occurred on
        // message object 2, and the message TX is complete.  Clear the
        // message object interrupt.
        //
        CANIntClear(CAN0_BASE, 2);

        //
        // Increment a counter to keep track of how many messages have been
        // sent.  In a real application this could be used to set flags to
        // indicate when a message is sent.
        //
        g_ui32Msg2Count++;

        //
        // Since the message was sent, clear any error flags.
        //
        g_bErrFlag = 0;
    }

    //
    // Check if the cause is message object 3, which is used for sending
    // messages 3 and 4.
    //
    else if(ui32Status == 3)
    {
        //
        // Getting to this point means that the TX interrupt occurred on
        // message object 3, and a message TX is complete.  Clear the
        // message object interrupt.
        //
        CANIntClear(CAN0_BASE, 3);

        //
        // Increment a counter to keep track of how many messages have been
        // sent.  In a real application this could be used to set flags to
        // indicate when a message is sent.
        //
        g_ui32Msg3Count++;

        //
        // Set the flag indicating that a message was sent using message
        // object 3.  The program main loop uses this to know when to send
        // another message using message object 3.
        //
        g_bMsgObj3Sent = 1;

        //
        // Since the message was sent, clear any error flags.
        //
        g_bErrFlag = 0;
    }
    else if(ui32Status == 4)
    {
        CANIntClear(CAN0_BASE, 4);
        g_ui32MsgRecvCount++;
        g_bRXFlag1= 1;
        g_bErrFlag = 0;
    }
    else if(ui32Status == 5)
    {
        CANIntClear(CAN0_BASE, 5);
        g_ui32MsgRecvCount++;
        g_bRXFlag2= 1;
        g_bErrFlag = 0;
    }
    else if(ui32Status == 6)
    {
        CANIntClear(CAN0_BASE, 6);
        g_ui32MsgRecvCount++;
        g_bRXFlag3= 1;
        g_bErrFlag = 0;
    }

    //
    // Otherwise, something unexpected caused the interrupt.  This should
    // never happen.
    //
    else
    {
        //
        // Spurios interrupt handling can go here.
        //
    }
}

void
InitConsole(void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Enable UART0 so that we can configure the clock.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Select the alternate (UART) function for these pins.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}
void
PrintCANMessageInfo(tCANMsgObject *psCANMsg, uint32_t ui32MsgObj, uint32_t txrx)
{
    if(txrx == CAN_TX){
        unsigned int uIdx;

        UARTprintf("Sending msg: obj=%d ID=0x%04X msg=0x", ui32MsgObj,
                psCANMsg->ui32MsgID);
        for(uIdx = 0; uIdx < psCANMsg->ui32MsgLen; uIdx++)
        {
            UARTprintf("%02X ", psCANMsg->pui8MsgData[uIdx]);
        }
        UARTprintf("\n");
    }
    else if(txrx == CAN_RX){
        unsigned int uIdx;

        //
        // Check to see if there is an indication that some messages were
        // lost.
        //
        if(psCANMsg->ui32Flags & MSG_OBJ_DATA_LOST)
        {
            UARTprintf("CAN message loss detected on message object %d\n",
                    ui32MsgObj);
        }

        //
        // Print out the contents of the message that was received.
        //
        UARTprintf("Msg Obj=%u ID=0x%05X len=%u data=0x", ui32MsgObj,
                psCANMsg->ui32MsgID, psCANMsg->ui32MsgLen);
        for(uIdx = 0; uIdx < psCANMsg->ui32MsgLen; uIdx++)
        {
            UARTprintf("%02X ", psCANMsg->pui8MsgData[uIdx]);
        }
        UARTprintf("\n");
    }
}
void
SimpleDelay(void)
{
    //
    // Delay cycles for 1 second
    //
    SysCtlDelay(16000000 / 3);
}

int main(){    

    tCANMsgObject sCANMessage;
    uint8_t pui8MsgData[8];
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);

    InitConsole();
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    UARTprintf("Debug 1 \n");
    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);

    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

    CANInit(CAN0_BASE);

    //
    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);
    CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);

    IntEnable(INT_CAN0);

    CANEnable(CAN0_BASE);
    // tx
    g_sCANMsgObject1.ui32MsgID = 0x1001;
    g_sCANMsgObject1.ui32MsgIDMask = 0;
    g_sCANMsgObject1.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    g_sCANMsgObject1.ui32MsgLen = sizeof(g_pui8Msg1);
    g_sCANMsgObject1.pui8MsgData = g_pui8Msg1;

    g_sCANMsgObject2.ui32MsgID = 0x2001;
    g_sCANMsgObject2.ui32MsgIDMask = 0;
    g_sCANMsgObject2.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    g_sCANMsgObject2.ui32MsgLen = sizeof(g_pui8Msg2);
    g_sCANMsgObject2.pui8MsgData = g_pui8Msg2;
    // rx
    sCANMessage.ui32MsgID = 0x4001;
    sCANMessage.ui32MsgIDMask = 0xfffff;
    sCANMessage.ui32Flags = (MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER |
                             MSG_OBJ_EXTENDED_ID);
    sCANMessage.ui32MsgLen = 8;
    CANMessageSet(CAN0_BASE, 4, &sCANMessage, MSG_OBJ_TYPE_RX);
    sCANMessage.ui32MsgID = 0x5001;
    CANMessageSet(CAN0_BASE, 5, &sCANMessage, MSG_OBJ_TYPE_RX);
    sCANMessage.ui32MsgID = 0x6001;
    CANMessageSet(CAN0_BASE, 6, &sCANMessage, MSG_OBJ_TYPE_RX);

    for(;;)
    {
        //
        // Send message 1 using CAN controller message object 1.  This is
        // the only message sent using this message object.  The
        // CANMessageSet() function will cause the message to be sent right
        // away.
        //
        PrintCANMessageInfo(&g_sCANMsgObject1, 1,CAN_TX);
        CANMessageSet(CAN0_BASE, 1, &g_sCANMsgObject1, MSG_OBJ_TYPE_TX);

        //
        // Send message 2 using CAN controller message object 2.  This is
        // the only message sent using this message object.  The
        // CANMessageSet() function will cause the message to be sent right
        // away.
        //
        PrintCANMessageInfo(&g_sCANMsgObject2, 2, CAN_TX);
        CANMessageSet(CAN0_BASE, 2, &g_sCANMsgObject2, MSG_OBJ_TYPE_TX);

        //
        // Load message object 3 with message 3.  This is needs to be done each
        // time because message object 3 is being shared for two different
        // messages.
        //
        g_sCANMsgObject3.ui32MsgID = 0x3001;
        g_sCANMsgObject3.ui32MsgIDMask = 0;
        g_sCANMsgObject3.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
        g_sCANMsgObject3.ui32MsgLen = sizeof(g_pui8Msg3);
        g_sCANMsgObject3.pui8MsgData = g_pui8Msg3;

        //
        // Clear the flag that indicates that message 3 has been sent.  This
        // flag will be set in the interrupt handler when a message has been
        // sent using message object 3.
        //
        g_bMsgObj3Sent = 0;

        //
        // Now send message 3 using CAN controller message object 3.  This is
        // the first message sent using this message object.  The
        // CANMessageSet() function will cause the message to be sent right
        // away.
        //
        PrintCANMessageInfo(&g_sCANMsgObject3, 3, CAN_TX);
        CANMessageSet(CAN0_BASE, 3, &g_sCANMsgObject3, MSG_OBJ_TYPE_TX);

        //
        // Wait for the indication from the interrupt handler that message
        // object 3 is done, because we are re-using it for another message.
        //
        while(!g_bMsgObj3Sent)
        {
        }

        //
        // Load message object 3 with message 4.  This is needed because
        // message object 3 is being shared for two different messages.
        //
        g_sCANMsgObject3.ui32MsgID = 0x3002;
        g_sCANMsgObject3.ui32MsgIDMask = 0;
        g_sCANMsgObject3.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
        g_sCANMsgObject3.ui32MsgLen = sizeof(g_pui8Msg4);
        g_sCANMsgObject3.pui8MsgData = g_pui8Msg4;

        //
        // Now send message 4 using CAN controller message object 3.  This is
        // the second message sent using this message object.  The
        // CANMessageSet() function will cause the message to be sent right
        // away.
        //
        PrintCANMessageInfo(&g_sCANMsgObject3, 3, CAN_TX);
        CANMessageSet(CAN0_BASE, 3, &g_sCANMsgObject3, MSG_OBJ_TYPE_TX);
        // rx
        if(g_bRXFlag1)
        {
            sCANMessage.pui8MsgData = pui8MsgData;
            CANMessageGet(CAN0_BASE, 4, &sCANMessage, 0);
            g_bRXFlag1 = 0;
            PrintCANMessageInfo(&sCANMessage, 1, CAN_RX);
        }
        if(g_bRXFlag2)
        {
            sCANMessage.pui8MsgData = pui8MsgData;
            CANMessageGet(CAN0_BASE, 5, &sCANMessage, 0);
            g_bRXFlag2 = 0;
            PrintCANMessageInfo(&sCANMessage, 2, CAN_RX);
        }
        if(g_bRXFlag3)
        {
            sCANMessage.pui8MsgData = pui8MsgData;
            CANMessageGet(CAN0_BASE, 6, &sCANMessage, 0);
            g_bRXFlag3 = 0;
            PrintCANMessageInfo(&sCANMessage, 3, CAN_RX);
        }

        //
        // Wait 1 second before continuing
        //
        SimpleDelay();

        //
        // Check the error flag to see if errors occurred
        //
>>>>>>> ac7f5bf2e1a5e94e3365253217efd271569c0882
        if(g_bErrFlag)
        {
            UARTprintf(" error - cable connected?\n");
        }
        else
        {
<<<<<<< HEAD
            UARTprintf(" total count = %u\n", g_ui32MsgCount);
        }

        ui32MsgData++;
    }

    return 0;

}
=======
            //
            // If no errors then print the count of message sent
            //
            UARTprintf(" total count = %u\n",
                       g_ui32Msg1Count + g_ui32Msg2Count + g_ui32Msg3Count);
        }

        //
        // Change the value in the message data for each of the messages.
        //
        (*(uint32_t *)g_pui8Msg1)++;
        (*(uint32_t *)g_pui8Msg2)++;
        (*(uint32_t *)g_pui8Msg3)++;
        (*(uint32_t *)&g_pui8Msg4[0])++;
        (*(uint32_t *)&g_pui8Msg4[4])--;
    }

    return 0;
}
>>>>>>> ac7f5bf2e1a5e94e3365253217efd271569c0882
