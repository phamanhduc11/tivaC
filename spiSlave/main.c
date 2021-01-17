#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_ssi.h"
#include "inc/hw_i2c.h"
#include "driverlib/debug.h"
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
#include "driverlib/debug.h"
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif

void
ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
    MAP_GPIOPinConfigure(GPIO_PA1_U0TX);
    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}

//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
void
UART0IntHandler(void)
{
    uint32_t ui32Status;

    //
    // Get the interrrupt status.
    //
    ui32Status = MAP_UARTIntStatus(UART0_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    MAP_UARTIntClear(UART0_BASE, ui32Status);

    //
    // Loop while there are characters in the receive FIFO.
    //
    while(MAP_UARTCharsAvail(UART0_BASE))
    {
        //
        // Read the next character from the UART and write it back to the UART.
        //
        MAP_UARTCharPutNonBlocking(UART0_BASE,
                                   MAP_UARTCharGetNonBlocking(UART0_BASE));

        //
        // Blink the LED to show a character transfer is occuring.
        //
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

        //
        // Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.
        //
        SysCtlDelay(SysCtlClockGet() / (1000 * 3));

        //
        // Turn off the LED
        //
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);

    }
}

//*****************************************************************************
//
// This function sets up SPI2 to be used as slave in freescale mode.
//
//*****************************************************************************
void
InitSPI2(void)
{
    //
    // The SSI2 peripheral must be enabled for use.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);

    //
    // For this example SSI2 is used with PortB[7:4].  GPIO port H needs to be
    // enabled so these pins can be used.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //
    // Configure the pin muxing for SSI2 functions on port H4, H5, H6 and H7.
    // This step is not necessary if your part does not support pin muxing.
    //
    GPIOPinConfigure(GPIO_PB4_SSI2CLK);
    GPIOPinConfigure(GPIO_PB5_SSI2FSS);
    GPIOPinConfigure(GPIO_PB6_SSI2RX);
    GPIOPinConfigure(GPIO_PB7_SSI2TX);

    //
    // Configure the GPIO settings for the SSI pins.  This function also gives
    // control of these pins to the SSI hardware.  Consult the data sheet to
    // see which functions are allocated per pin.
    // The pins are assigned as follows:
    //      PH7 - SSI2Tx
    //      PH6 - SSI2Rx
    //      PH5 - SSI2Fss
    //      PH4 - SSI2CLK
    //
    GPIOPinTypeSSI(GPIO_PORTB_BASE, GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5 |
                   GPIO_PIN_4);

    //
    // Configure and enable the SSI2 port for SPI slave mode.
    //
    SSIConfigSetExpClk(SSI2_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3,
    				   SSI_MODE_SLAVE, 5000, 8);

    //
    // Enable the SSI2 module.
    //
    SSIEnable(SSI2_BASE);
}
//*****************************************************************************
//
// Global variables used in interrupt handler and the main loop.
//
//*****************************************************************************
#define NUM_SSI_DATA 20
volatile unsigned long g_ulSSI2RXTO = 0;
unsigned long g_ulDataRx2[NUM_SSI_DATA];

//*****************************************************************************
//
// Interrupt handler for SSI2 peripheral in slave mode.  It reads the interrupt
// status and if the interrupt is fired by a RX time out interrupt it reads the
// SSI2 RX FIFO and increments a counter to tell the main loop that RX timeout
// interrupt was fired.
//
//*****************************************************************************
void
SSI2IntHandler(void)
{
	unsigned long ulStatus, ulIndex;

//    UARTprintf("In SSI Int\n");
	//
	// Read interrupt status.
	//
	ulStatus = SSIIntStatus(SSI2_BASE, 0);
//	UARTprintf("%x\n",ulStatus);
//	ulStatus = SSIIntStatus(SSI2_BASE, 1);
	//
	// Check the reason for the interrupt.
	//
	if(ulStatus & SSI_RXTO)
	{
		//
		// Interrupt is because of RX time out.  So increment counter to tell
		// main loop that RX timeout interrupt occurred.
		//
		g_ulSSI2RXTO = true;

		//
		// Read NUM_SSI_DATA bytes of data from SSI2 RX FIFO.
		//
		// for(ulIndex = 0; ulIndex < NUM_SSI_DATA; ulIndex++)
		for(ulIndex = 0; ulIndex < NUM_SSI_DATA; ulIndex++)
		{
			SSIDataGet(SSI2_BASE, &g_ulDataRx2[ulIndex]);
		}
	}

	//
	// Clear interrupts.
	//
	SSIIntClear(SSI2_BASE, ulStatus);
}
void
InitSPI0(void)
{
    //
    // The SSI0 peripheral must be enabled for use.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);

    //
    // For this example SSI0 is used with PortA[5:2].  GPIO port A needs to be
    // enabled so these pins can be used.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for SSI0 functions on port A2, A3, A4, and A5.
    // This step is not necessary if your part does not support pin muxing.
    //
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);

    //
    // Configure the GPIO settings for the SSI pins.  This function also gives
    // control of these pins to the SSI hardware.  Consult the data sheet to
    // see which functions are allocated per pin.
    // The pins are assigned as follows:
    //      PA5 - SSI0Tx
    //      PA4 - SSI0Rx
    //      PA3 - SSI0Fss
    //      PA2 - SSI0CLK
    //
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 |
                   GPIO_PIN_2);

    //
    // Configure and enable the SSI0 port for SPI master mode.
    //
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3,
    				   SSI_MODE_MASTER, 660000, 8);

    //
    // Enable the SSI0 module.
    //
    SSIEnable(SSI0_BASE);
}

#define SLAVE_ADDRESS 0x4D
static uint32_t g_ui32DataRx;
static bool g_bIntFlag = false;
void
I2C0SlaveIntHandler(void)
{
    UARTprintf("\nInt I2C\n");
    //
    // Clear the I2C0 interrupt flag.
    //
    I2CSlaveIntClear(I2C0_BASE);

    //
    // Read the data from the slave.
    //
    g_ui32DataRx = I2CSlaveDataGet(I2C0_BASE);

    //
    // Set a flag to indicate that the interrupt occurred.
    //
    g_bIntFlag = true;
}

void I2CInit(void){

    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
    //
    // Enable loopback mode.  Loopback mode is a built in feature that helps
    // for debug the I2Cx module.  It internally connects the I2C master and
    // slave terminals, which effectively lets you send data as a master and
    // receive data as a slave.  NOTE: For external I2C operation you will need
    // to use external pull-ups that are faster than the internal pull-ups.
    // Refer to the datasheet for more information.
    //
//    HWREG(I2C0_BASE + I2C_O_MCR) |= 0x01;
    IntEnable(INT_I2C0);
    I2CSlaveIntEnableEx(I2C0_BASE, I2C_SLAVE_INT_DATA);
//    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);

    I2CSlaveEnable(I2C0_BASE);
    I2CSlaveInit(I2C0_BASE, SLAVE_ADDRESS);
//    I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, false);

}

int main(){
    unsigned long ulDataTx0[NUM_SSI_DATA];
    unsigned long ulDataRx0[NUM_SSI_DATA];
    unsigned long ulindex;
    uint32_t ui32DataTx;
    MAP_FPULazyStackingEnable();
    MAP_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                    SYSCTL_OSC_MAIN);
    ConfigureUART();

    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Enable the GPIO pins for the LED (PF2).
    //
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    //
    // Enable processor interrupts.
    //
    MAP_IntMasterEnable();
    //
    // Enable the UART interrupt.
    //
    MAP_IntEnable(INT_UART0);
    MAP_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    InitSPI0();
    while(SSIDataGetNonBlocking(SSI0_BASE, &ulDataRx0[0]))
    {
    }
    //
    // Init SPI2 as slave.
    //
    InitSPI2();

    SSIIntEnable(SSI2_BASE, SSI_RXTO);
    UARTprintf("Int mask %x\n",HWREG(SSI2_BASE + SSI_O_IM));
    while(SSIDataGetNonBlocking(SSI2_BASE, &g_ulDataRx2[0]))
    {
    }
    SSIIntClear(SSI2_BASE, SSI_RXTO);

    // Hello!
    //
    UARTprintf("Hello, world!\n");
    
    I2CInit();
//    ui32DataTx = 'I';
//    UARTprintf("Transferring from: Master -> Slave\n");

    //
    // Display the data that I2C0 is transferring.
    //
//    UARTprintf("  Sending: '%c'", ui32DataTx);

    //
    // Place the data to be sent in the data register.
    //
//    UARTprintf("\n Debug 1\n");
//    I2CMasterDataPut(I2C0_BASE, ui32DataTx);
//
//    //
//    // Initiate send of single piece of data from the master.  Since the
//    // loopback mode is enabled, the Master and Slave units are connected
//    // allowing us to receive the same data that we sent out.
//    //
//    UARTprintf("\n Debug 2\n");
//    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
//    UARTprintf("\n Debug 3\n");

    //
    // Wait for interrupt to occur.
    //


    //
    // Display that interrupt was received.
    //
//    UARTprintf("\n  Slave Interrupt Received!\n");

    //
    // Display the data that the slave has received.
    //
//    UARTprintf("  Received: '%c'\n\n", g_ui32DataRx);

    // ulDataTx0[0] = 's';
    // ulDataTx0[1] = 'p';
    // ulDataTx0[2] = 'i';

    // for(ulindex = 0; ulindex < NUM_SSI_DATA; ulindex++)
    // {
    //     //
    //     // Display the data that SSI is transferring.
    //     //
    //     UARTprintf("'%d' ", ulDataTx0[ulindex]);

    //     //
    //     // Send the data using the "blocking" put function.  This function
    //     // will wait until there is room in the send FIFO before returning.
    //     // This allows you to assure that all the data you send makes it into
    //     // the send FIFO.
    //     //
    //     SSIDataPut(SSI0_BASE, ulDataTx0[ulindex]);
    // }

    // //
    // // Wait until SSI0 is done transferring all the data in the transmit FIFO.
    // //
    // while(SSIBusy(SSI0_BASE))
    // {
    // }

    //
    // Enable the SSI2 interrupts to ARM core.  This has to be done here,
    // otherwise the RX timeout interrupt will fire before all the data has
    // been transferred.  This is specific to this example as both the SSI
    // master and slave are on the same microcontroller.
    //
    IntEnable(INT_SSI2);

    //
    // Wait for the SSI2 RXTO interrupt to fire and data read from RXFIFO.
    //
    // while(g_ulSSI2RXTO == 0)
    // {
    // }

    // //
    // // Display indication that salve has receiving data.
    // //
    // UARTprintf("\nReceived:\n  ");

    // //
    // // Display the 3 bytes of data that were read from RX FIFO.
    // //
    // for(ulindex = 0; ulindex < NUM_SSI_DATA; ulindex++)
    // {
    //     UARTprintf("'%c' ", g_ulDataRx2[ulindex]);
    // }

    // //
    // // Check that the data sent was the same as the data received.
    // //
    // for(ulindex = 0; ulindex < NUM_SSI_DATA; ulindex++)
    // {
    //     if(ulDataTx0[ulindex] != g_ulDataRx2[ulindex])
    //     {
    //         //
    //         // Tell the user that the test failed.
    //         //
    //         UARTprintf("\n\nError: Data does not exactly match.\n");
    //         UARTprintf("Check that Tx & Rx are connected correctly.\n\n");

    //         //
    //         // Wait in infinite loop for debugging.
    //         //
    //         while(1)
    //         {
    //         }
    //     }
    // }

    // if(g_ulSSI2RXTO > 1)
    // {
	// 	//
	// 	// Tell the user that the test failed and the reason.
	// 	//
	// 	UARTprintf("\n\nError: %d interrupt(s) fired when expecting only one."
	// 			   "\n", g_ulSSI2RXTO);
    // }
    // else
    // {
	// 	//
	// 	// Tell the user that the test passed.
	// 	//
	// 	UARTprintf("\n\nTest Passed.\n\n");
    // }
     for(ulindex = 0; ulindex < NUM_SSI_DATA; ulindex++)
     {
         //
         // Display the data that SSI is transferring.
         //
         ulDataTx0[ulindex] = (1+ulindex);
         UARTprintf("'%d' ", ulDataTx0[ulindex]);

         //
         // Send the data using the "blocking" put function.  This function
         // will wait until there is room in the send FIFO before returning.
         // This allows you to assure that all the data you send makes it into
         // the send FIFO.
         //
         SSIDataPut(SSI2_BASE, ulDataTx0[ulindex]);
     }
    int i;
    UARTprintf("\nEnd Write\n ");
    while(1){
        if (g_ulSSI2RXTO > 0){
            UARTprintf("Start print\n");
            for(i=0;i<NUM_SSI_DATA;i++){
                UARTprintf("%d",g_ulDataRx2[i]%10);
            }
            UARTprintf("\n");
            UARTprintf("End Print\n");
            g_ulSSI2RXTO = false;

        }
    }
    return 0;
}
