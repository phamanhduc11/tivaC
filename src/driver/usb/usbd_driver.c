#include "INC/usb/usbd_driver.h"
#include "INC/usb/usb_standards.h"

#define USB_BASE    0x40050000

#define USBFADDR        *(unsigned char*)   (USB_BASE + 0x000)
#define USBPOWER        *(unsigned char*)   (USB_BASE + 0x001)
#define USBTXIS         *(unsigned short*)  (USB_BASE + 0x002)
#define USBRXIS         *(unsigned short*)  (USB_BASE + 0x004)
#define USBTXIE         *(unsigned short*)  (USB_BASE + 0x006)
#define USBRXIE         *(unsigned short*)  (USB_BASE + 0x008)
#define USBIS           *(unsigned char*)   (USB_BASE + 0x00A)
#define USBIE           *(unsigned char*)   (USB_BASE + 0x00B)
#define USBFRAME        *(unsigned short*)  (USB_BASE + 0x00C)
#define USBEPIDX        *(unsigned char*)   (USB_BASE + 0x00E)
#define USBTEST         *(unsigned char*)   (USB_BASE + 0x00F)

#define USBFIFO_32(n)      *(unsigned int*)    (USB_BASE + 0x20 +n*0x4) //  0 <= n <= 7
#define USBFIFO_16(n)      *(unsigned short*)    (USB_BASE + 0x20 +n*0x4) //  0 <= n <= 7
#define USBFIFO_8(n)      *(unsigned char*)    (USB_BASE + 0x20 +n*0x4) //  0 <= n <= 7
#define USBDEVCTL       *(unsigned char*)   (USB_BASE + 0x060)
#define USBTXFIFOSZ     *(unsigned char*)   (USB_BASE + 0x062)
#define USBRXFIFOSZ     *(unsigned char*)   (USB_BASE + 0x063)
#define USBTXFIFOADD    *(unsigned short*)  (USB_BASE + 0x064)
#define USBRXFIFOADD    *(unsigned short*)  (USB_BASE + 0x066)
#define USBCONTIM       *(unsigned char*)   (USB_BASE + 0x07A)
#define USBVPLEN        *(unsigned char*)   (USB_BASE + 0x07B)
#define USBFSEOF        *(unsigned char*)   (USB_BASE + 0x07D)
#define USBLSEOF        *(unsigned char*)   (USB_BASE + 0x07E)

#define USBTXFUNCADDR(n)    *(unsigned char*)   (USB_BASE + 0x80 +n*0x8) //  0 <= n <= 7
#define USBTXHUBADDR(n)     *(unsigned char*)   (USB_BASE + 0x82 +n*0x8) //  0 <= n <= 7
#define USBTXHUBPORT(n)     *(unsigned char*)   (USB_BASE + 0x83 +n*0x8) //  0 <= n <= 7
#define USBRXFUNCADDR(n)    *(unsigned char*)   (USB_BASE + 0x8C +(n-1)*0x8) //  1 <= n <= 7
#define USBRXHUBPORT(n)     *(unsigned char*)   (USB_BASE + 0x8E +(n-1)*0x8) //  1 <= n <= 7
#define USBRXHUBADDR(n)     *(unsigned char*)   (USB_BASE + 0x8F +(n-1)*0x8) //  1 <= n <= 7

#define USBCSRL0            *(unsigned char*)   (USB_BASE + 0x102)
#define USBCSRH0            *(unsigned char*)   (USB_BASE + 0x103)
#define USBCOUNT0           *(unsigned char*)   (USB_BASE + 0x108)
#define USBTYPE0            *(unsigned char*)   (USB_BASE + 0x10A)
#define USBNAKLMT           *(unsigned char*)   (USB_BASE + 0x10B)

#define USBTXMAXP(n)        *(unsigned short*)  (USB_BASE + 0x110 +(n-1)*0x10) //  1 <= n <= 7
#define USBTXCSRL(n)        *(unsigned char*)   (USB_BASE + 0x112 +(n-1)*0x10) //  1 <= n <= 7
#define USBTXCSRH(n)        *(unsigned char*)   (USB_BASE + 0x113 +(n-1)*0x10) //  1 <= n <= 7
#define USBRXMAXP(n)        *(unsigned short*)  (USB_BASE + 0x114 +(n-1)*0x10) //  1 <= n <= 7
#define USBRXCSRL(n)        *(unsigned char*)   (USB_BASE + 0x116 +(n-1)*0x10) //  1 <= n <= 7
#define USBRXCSRH(n)        *(unsigned char*)   (USB_BASE + 0x117 +(n-1)*0x10) //  1 <= n <= 7
#define USBRXCOUNT(n)       *(unsigned short*)  (USB_BASE + 0x118 +(n-1)*0x10) //  1 <= n <= 7
#define USBTXTYPE(n)        *(unsigned char*)   (USB_BASE + 0x11A +(n-1)*0x10) //  1 <= n <= 7
#define USBTXINTERVAL(n)    *(unsigned char*)   (USB_BASE + 0x11B +(n-1)*0x10) //  1 <= n <= 7
#define USBRXTYPE(n)        *(unsigned char*)   (USB_BASE + 0x11C +(n-1)*0x10) //  1 <= n <= 7
#define USBRXINTERVAL(n)    *(unsigned char*)   (USB_BASE + 0x11D +(n-1)*0x10) //  1 <= n <= 7
#define USBRQPKTCOUNT(n)    *(unsigned short*)  (USB_BASE + 0x304 +(n-1)*0x4)  //  1 <= n <= 7

#define USBRXDPKTBUFDIS     *(unsigned short*)  (USB_BASE + 0x340)
#define USBTXDPKTBUFDIS     *(unsigned short*)  (USB_BASE + 0x342)
#define USBEPC              *(unsigned int*)    (USB_BASE + 0x400)
#define USBEPCRIS           *(unsigned int*)    (USB_BASE + 0x404)
#define USBEPCIM            *(unsigned int*)    (USB_BASE + 0x408)
#define USBEPCISC           *(unsigned int*)    (USB_BASE + 0x40C)
#define USBDRRIS            *(unsigned int*)    (USB_BASE + 0x410)
#define USBDRIM             *(unsigned int*)    (USB_BASE + 0x414)
#define USBDRISC            *(unsigned int*)    (USB_BASE + 0x418)
#define USBGPCS             *(unsigned int*)    (USB_BASE + 0x41C)
#define USBVDC              *(unsigned int*)    (USB_BASE + 0x430)
#define USBVDCRIS           *(unsigned int*)    (USB_BASE + 0x434)
#define USBVDCIM            *(unsigned int*)    (USB_BASE + 0x438)
#define USBVDCISC           *(unsigned int*)    (USB_BASE + 0x43C)
#define USBIDVRIS           *(unsigned int*)    (USB_BASE + 0x444)
#define USBIDVIM            *(unsigned int*)    (USB_BASE + 0x448)
#define USBIDVISC           *(unsigned int*)    (USB_BASE + 0x44C)
#define USBDMASEL           *(unsigned int*)    (USB_BASE + 0x450)
#define USBPP               *(unsigned int*)    (USB_BASE + 0xFC0)

#define RCGCUSB         *(unsigned int*)    (0x400FE000 + 0x628)
#define RCC2            *(unsigned int*)    (0x400FE000 + 0x070)
#define RCGCGPIO        *(unsigned int*)    (0x400FE000 + 0x608)

//#define USB_TIVA_GPIO_BASE  0x4005B000 // PORT D BASE AHB
#define USB_TIVA_GPIO_BASE  0x40007000 // PORT D BASE APB

#define USB_GPIODIR         *(unsigned int*)    (USB_TIVA_GPIO_BASE + 0x400)
#define USB_GPIOAFSEL       *(unsigned int*)    (USB_TIVA_GPIO_BASE + 0x420)
#define USB_GPIODR2R        *(unsigned int*)    (USB_TIVA_GPIO_BASE + 0x500)
#define USB_GPIODR4R        *(unsigned int*)    (USB_TIVA_GPIO_BASE + 0x504)
#define USB_GPIODR8R        *(unsigned int*)    (USB_TIVA_GPIO_BASE + 0x508)
#define USB_GPIOODR        *(unsigned int*)    (USB_TIVA_GPIO_BASE + 0x50C)
#define USB_GPIOAMSEL       *(unsigned int*)    (USB_TIVA_GPIO_BASE + 0x528)
#define USB_GPIOPCTL        *(unsigned int*)    (USB_TIVA_GPIO_BASE + 0x52C)

#define SCB_USB_INT_EN  *(unsigned int*) (0xE000E000 + 0x104)

#ifdef ASSERT(x)
#undef ASSERT(x)
#endif
#define ASSERT(x)  while(!x) {}
static void ConfigureTxRxFIFOSize (int EPNum, bool isTxEndpoint, struct endpointInfo epInfo);

static void USBPinInitialize(void) {
    volatile unsigned char ui8Delay;
    // Configure alternate function of pin and configure pin to use alt func - 7 steps of GPIO initialization
    RCGCGPIO = RCGCGPIO | (1 << 3);
    for(ui8Delay = 0; ui8Delay < 16; ui8Delay++){
    }
    USB_GPIODIR = USB_GPIODIR & ~(1 << 5 | 1 << 4);
    USB_GPIOAFSEL = USB_GPIOAFSEL | (1 << 5 | 1 << 4);
    USB_GPIOPCTL = USB_GPIOPCTL & (0xFF00FFFF);
        // using 2mA padcfg
    USB_GPIODR2R = USB_GPIODR2R | ( 1 << 5 | 1 << 4);
    USB_GPIODR4R = USB_GPIODR4R & ~( 1 << 5 | 1 << 4);
    USB_GPIODR8R = USB_GPIODR8R & ~( 1 << 5 | 1 << 4);
    USB_GPIOAMSEL = USB_GPIOAMSEL | ( 1 << 5 | 1 << 4);
}

// legacy usb reset
#define SRCR2_REG *(unsigned int*)(0x400FE000 + 0x48)
// or using peripherals specific (USB Peripheral) reset
#define SRUSB_REG *(unsigned int *)(0x400FE000 + 0x528)

static void USBBusConnect(void) {
    USBPOWER = USBPOWER | (1 << 6);
}

static void USBBusDisconnect(void) {
    USBPOWER = USBPOWER & ~(1 << 6);
}

static void USBSetDeviceAdress(unsigned char address) {
    USBFADDR = address;
}

static void USBInterruptInitialize(void){
    // Enable general interrupt
    USBIE = USBIE | ( 1 << 5 |1 << 3 | 1 << 2 | 1 << 1 | 1 << 0);
    // Enable recv and trans interrupts
    USBRXIE = USBRXIE | (1 << 1);
    USBTXIE = USBTXIE | (1 << 1);
    // Clear all interrupts
    USBIS;
    USBTXIS;
    USBRXIS;

    // __asm("    mov r0, #0x1\n"
    //       "   msr control, r0\n"
    //       "   ISB\n");
    SCB_USB_INT_EN = SCB_USB_INT_EN | (1 << 12);

}

static void USBDeviceInit(void) {
    volatile unsigned char ui8Delay;
    // Enable USB PLL Clock
    RCC2 = RCC2 & ~(1 << 14);
    // Enable Clock of USB Module
    RCGCUSB = RCGCUSB | 1;
    // Reset DMA module b4 s/w Host -> Device or vice versa
    SRCR2_REG = SRCR2_REG | 1 << 16;
    for(ui8Delay = 0; ui8Delay < 16; ui8Delay++){

    }
    SRCR2_REG = SRCR2_REG & ~(1 << 16);
    // set DEVICE and DEVICEOTG mode <-> Force USBDevice Mode, workaround for test
    // since this register affect USBVBUS and USBID internally in USB module.
    // TODO: check timing
    for(ui8Delay = 0; ui8Delay < 16; ui8Delay++){

    }
    USBGPCS = USBGPCS | (1 << 1 | 1 << 0);
    

}

static void FlushRxFifo(int EPNum) {
    USBRXCSRL(EPNum) = USBRXCSRL(EPNum) & ~(1 << 4);
}

static void FlushTxFifo(int EPNum) {
    USBTXCSRL(EPNum) = USBTXCSRL(EPNum &  ~(1 << 3));
}

/*
* \brief Activate/Enable IN and OUT endpoint 0 by enable endpoint interrupt
* \param None
*/
static void ConfigureEndpoint0(void) {
    // Default value is 1
    USBTXIE = USBTXIE | 1;

}

/*
* \brief Deactivate/Disable IN and OUT endpoint by disable endpoint interrupt
* \param EPnum The number of the IN-OUT endpoint we want to deactivate/disable
*/
static void DeconfigureEndpoint(int EPNum) {
    USBTXIE = USBTXIE & ~(1 << EPNum);
    USBRXIE = USBRXIE & ~(1 << EPNum);

    //  Flush FIFOs
    FlushRxFifo(EPNum);
    FlushTxFifo(EPNum);
}

/*
* \brief Activate/Enable IN (Tx) endpoint by enable endpoint interrupt
* \param EPnum The number of the IN endpoint we want to activate/enable
*/
static void ConfigureINEndpoint(int EPNum, struct endpointInfo epInfo) {
    ASSERT(EPNum != 0);
    // TODO: missing config EP type
    ConfigureTxRxFIFOSize(EPNum, true, epInfo);
    USBTXIE = USBTXIE | 1 << EPNum;
}

/*
* \brief Activate/Enable OUT (Rx) endpoint by enable endpoint interrupt
* \param EPnum The number of the OUT endpoint we want to activate/enable
*/
static void ConfigureOUTEndpoint(int EPNum, struct endpointInfo epInfo) {
    ASSERT(EPNum != 0);
    // TODO: missing config EP type
    ConfigureTxRxFIFOSize(EPNum, false, epInfo);
    USBRXIE = USBRXIE | 1 << EPNum;
}

/*
* \brief Allocate FIFO Ram size of an endpoint 
* \param EPnum The number of the OUT endpoint we want to activate/enable
* \param isTxEndpoint A boolean check if configure TX endpoint or Rx endpoint
* \param epInfo Structure that contains reuqested size of FIFO RAM and the start address of FIFO RAM
*/
static void ConfigureTxRxFIFOSize (int EPNum, bool isTxEndpoint, struct endpointInfo epInfo) {
    struct endpointInfo EP = epInfo;
    unsigned int EPMaxTxRxFIFORamSize = 0;
    ASSERT((EP.FIFORamAddr % USB_FIFO_ADDR_MODULATE) == 0)
    ASSERT(EPNum != 0);

    USBEPIDX = EPNum;
    if (isTxEndpoint) {
        USBTXFIFOSZ = EP.FIFORamSize;
        USBTXFIFOADD = EP.FIFORamAddr / 8;
    }
    else {
        USBRXFIFOSZ = EP.FIFORamSize;
        USBRXFIFOADD = EP.FIFORamAddr / 8;
    }

    switch(EP.FIFORamSize) {
        case USB_FIFO_RAM_SIZE_8    :
            EPMaxTxRxFIFORamSize = 8;
            break;
        case USB_FIFO_RAM_SIZE_16   :
            EPMaxTxRxFIFORamSize = 16;
            break;
        case USB_FIFO_RAM_SIZE_32   :
            EPMaxTxRxFIFORamSize = 32;
            break;
        case USB_FIFO_RAM_SIZE_64   :
            EPMaxTxRxFIFORamSize = 64;
            break;
        case USB_FIFO_RAM_SIZE_128  :
            EPMaxTxRxFIFORamSize = 128;
            break;
        case USB_FIFO_RAM_SIZE_256  :
            EPMaxTxRxFIFORamSize = 256;
            break;
        case USB_FIFO_RAM_SIZE_512  :
            EPMaxTxRxFIFORamSize = 512;
            break;
        case USB_FIFO_RAM_SIZE_1024 :
            EPMaxTxRxFIFORamSize = 1024;
            break;
        case USB_FIFO_RAM_SIZE_2048 :
            EPMaxTxRxFIFORamSize = 2048;
            break;
        default:
            ASSERT(0);
    }

    if (isTxEndpoint) {
        USBTXMAXP(EPNum) = EPMaxTxRxFIFORamSize;
    } 
    else {
        USBRXMAXP(EPNum) = EPMaxTxRxFIFORamSize;
    }
}

static void writeTxFIFO(int EPNum, unsigned int *data, unsigned int accessSize) {
    ASSERT(data != 0);
    switch(accessSize) {
        case 8:
            USBFIFO_8(EPNum) = (unsigned char) *data;
            break;
        case 16:
            USBFIFO_16(EPNum) = (unsigned short) *data;
            break;
        case 32:
            USBFIFO_32(EPNum) = (unsigned int) *data;
            break;
    }
}

static void readRxFIFO(int EPNum, unsigned int *data, unsigned int accessSize) {
    switch(accessSize) {
        case 8:
            *data = USBFIFO_8(EPNum);
            break;
        case 16:
            *data = USBFIFO_16(EPNum);
            break;
        case 32:
            *data = USBFIFO_32(EPNum);
            break;
    }
}

static void readPacket(unsigned int EPNum, void *buffer, unsigned short size) {
    for(;size >= 4; size -=4, buffer +=4) {
        readRxFIFO(EPNum, buffer, 32);
    }
    if (size > 0) {
        for(; size > 0; size--, buffer++) {
            readRxFIFO(EPNum, buffer, 8);
        }
    }
}

static void writePacket(unsigned int EPNum, void const *buffer, unsigned short size) {
    for(;size >= 4; size -=4, buffer +=4) {
        writeTxFIFO(EPNum, (void*) buffer, 32);
    }
    if (size > 0) {
        for(; size > 0; size--, buffer++) {
            writeTxFIFO(EPNum, (void*) buffer, 8);
        }
    }

}


static void USBIntResetHandler(void) {
    unsigned int i;
    for (i = 0; i < 8; i++) {
        DeconfigureEndpoint(i);
    }
}

void USBDeviceGetIntStatus(unsigned int *ui32Status) {
    // General Interrupt
    *ui32Status = USBIS;
    // TX Endpoint Int 
    *ui32Status |= USBTXIS << 8;
    // RX Endpoint Int
    *ui32Status |= USBRXIS << 16;
}

void USBGetRcvBytes(unsigned int EPNum, unsigned short *bcnt) {
    if (EPNum == 0) {
        *bcnt = USBCOUNT0;
    }
    else {
        *bcnt = USBRXCOUNT(EPNum);
    }
}

static void USBGetEndpointStatus(int EPNum, unsigned int *ret) {
    if (EPNum == 0) {
        *ret = (USBCSRH0 << 8) | USBCSRL0;
    }
    else {
        *ret = (unsigned int) (USBTXCSRH(EPNum) << 24 | USBTXCSRL(EPNum) << 16 | USBRXCSRH(EPNum) << 8 | USBRXCSRL(EPNum));
    }
}

static void USBCoreInitialize(void) {
    USBDeviceInit();
    USBInterruptInitialize();
    ConfigureEndpoint0();
    // EndpointInitialize();
}

static void USBIntFlagClear(int EPNum, int intFlags) {
    
}

#define USBCSRL0_RXRDY_C    0x40
#define USBCSRL0_TXRDY      0x2
#define USBCSRL0_DATAEND    0x8

#define USBTXCSRL_TXRDY     0x1
#define USBTXCSRL_DATAEND   0x0

#define USBRXCSRL_RXRDY   0x1

static unsigned char USBEndpointSend(int EPNum, int isLastPacket) {
    unsigned short EPTXCSFlags = 0x0;
    if (0 == EPNum) {
        EPTXCSFlags =  (USBCSRH0 << 8) | USBCSRL0;

        // In transmit
        if (USBCSRL0_TXRDY & (EPTXCSFlags & 0xFF)) {
            return 1;
        }

        EPTXCSFlags |= USBCSRL0_TXRDY;
        if (isLastPacket) {
            EPTXCSFlags |= USBCSRL0_DATAEND;
        }

        USBCSRL0 = (EPTXCSFlags & 0xFF);
    }
    else {
        EPTXCSFlags = (USBTXCSRH(EPNum) << 8) | USBTXCSRL(EPNum);

        if ( USBTXCSRL_TXRDY & (EPTXCSFlags >> 8)) {
            return 1;
        }

        EPTXCSFlags |= USBTXCSRL_TXRDY;

        USBTXCSRL(EPNum) = (EPTXCSFlags & 0xFF);
    }

    return 0;
}

static void USBEndpointAck(unsigned int EPNum, int isLastPacket) {
    if (0 == EPNum) {
        USBCSRL0 |= USBCSRL0_RXRDY_C | (isLastPacket ? USBCSRL0_DATAEND : 0);
    }
    else {
        USBRXCSRL(EPNum) &= ~USBRXCSRL_RXRDY;   
    }
}

/*
    USB Module Interrupt handler
*/
#define USB_GENERAL_INT_DEVICE_SUSPEND  0x1
#define USB_GENERAL_INT_DEVICE_RESUME   0x2
#define USB_GENERAL_INT_DEVICE_RESET    0x4
#define USB_GENERAL_INT_DEVICE_SOF      0x8
#define USB_GENERAL_INT_DEVICE_DISCON   0x20


#define BIT0    0x1
#define BIT1    BIT0 << 1
#define BIT2    BIT0 << 2
#define BIT3    BIT0 << 3
#define BIT4    BIT0 << 4
#define BIT5    BIT0 << 5
#define BIT6    BIT0 << 6
#define BIT7    BIT0 << 7


static void USBDIntHandler(unsigned int ui32IntStatus) {
    // Suspend Handler
    if (ui32IntStatus & USB_GENERAL_INT_DEVICE_SUSPEND) {
    }
    // Resume handler
    if (ui32IntStatus & USB_GENERAL_INT_DEVICE_RESUME) {

    }
    // Reset handler
    if (ui32IntStatus & USB_GENERAL_INT_DEVICE_RESET) {
        usb_events.on_usb_reset_received();
    }
    // Start of frame handler
    if (ui32IntStatus & USB_GENERAL_INT_DEVICE_SOF) {

    }
    // Disconnect handler
    if (ui32IntStatus & USB_GENERAL_INT_DEVICE_DISCON) {

    }

    // Handle Endpoint Handler
    if (GET_TX_ENDPOINT_INT_MASK(ui32IntStatus) & BIT0) {
        usb_events.on_endpoint_handle(0, ui32IntStatus);
    }
    if ((GET_TX_ENDPOINT_INT_MASK(ui32IntStatus) | GET_RX_ENDPOINT_INT_MASK(ui32IntStatus)) & BIT1 ) {
        usb_events.on_endpoint_handle(1, ui32IntStatus);
    }
    if ((GET_TX_ENDPOINT_INT_MASK(ui32IntStatus) | GET_RX_ENDPOINT_INT_MASK(ui32IntStatus)) & BIT2 ) {
        usb_events.on_endpoint_handle(2, ui32IntStatus);
    }
    if ((GET_TX_ENDPOINT_INT_MASK(ui32IntStatus) | GET_RX_ENDPOINT_INT_MASK(ui32IntStatus)) & BIT3 ) {
        usb_events.on_endpoint_handle(3, ui32IntStatus);
    }
   
}

const USBDriver usb_driver = {
    .initialize_core = (void (*)(void)) &USBCoreInitialize,
    .initialize_gpio_pins = &USBPinInitialize,
    .connect = &USBBusConnect,
    .disconnect = &USBBusDisconnect,
    .flush_rxfifo = &FlushRxFifo,
    .flush_txfifo = &FlushTxFifo,
    .clear_intflag = &USBIntFlagClear,
    .configure_in_endpoint = &ConfigureINEndpoint,
    .configure_out_endpoint = &ConfigureOUTEndpoint,
    .write_packet = &writePacket,
    .read_packet = (void (*)(unsigned int, void const*, unsigned short)) &readPacket,
    .send_ep_data = &USBEndpointSend,
    .ack_ep_rcv = &USBEndpointAck,
    .endpoint_ctl_status = &USBGetEndpointStatus,
    .set_device_address = &USBSetDeviceAdress,
    .get_rcv_packet_size = &USBGetRcvBytes,
    .get_int_status = &USBDeviceGetIntStatus,
    .polling = &USBDIntHandler,
};
