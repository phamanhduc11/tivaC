#ifndef _USBD_DRIVER_H_
#define _USBD_DRIVER_H_
#include <stdbool.h>
#include "usb_standards.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define USB_FIFO_RAM_SIZE_8     0x0
#define USB_FIFO_RAM_SIZE_16    0x1
#define USB_FIFO_RAM_SIZE_32    0x2
#define USB_FIFO_RAM_SIZE_64    0x3
#define USB_FIFO_RAM_SIZE_128   0x4
#define USB_FIFO_RAM_SIZE_256   0x5
#define USB_FIFO_RAM_SIZE_512   0x6
#define USB_FIFO_RAM_SIZE_1024  0x7
#define USB_FIFO_RAM_SIZE_2048  0x8

#define USB_FIFO_ADDR_MODULATE  0x8

#define GET_GENERAL_INT_MASK(x) (x & 0xFF)
#define GET_TX_ENDPOINT_INT_MASK(x) ((x >> 8) & 0xFF)
#define GET_RX_ENDPOINT_INT_MASK(x) ((x >> 16) & 0xFF)

struct endpointInfo {
    USBEndpointType epType;
    bool    isDoubleBuffering;
    int     maxPacketSize;
    int     FIFORamAddr;
    int     FIFORamSize;
};
typedef struct {
    unsigned char   endpointIndex;
    unsigned char   *Buffer;
    bool            isTransmitEP;
    unsigned int    bufferSize;
    struct endpointInfo epInfo;
}   USBEndpointClass;

typedef struct {
    void (*initialize_core)(void);
    void (*initialize_gpio_pins)(void);
    void (*connect)(void);
    void (*disconnect)(void);
    void (*flush_rxfifo)(int EPNum);
    void (*flush_txfifo)(int EPNum);
    void (*clear_intflag)(int EPNum, int intFlags);
    void (*configure_in_endpoint)(int EPNum, struct endpointInfo epInfo);
    void (*configure_out_endpoint)(int EPNum, struct endpointInfo EpInfo);
    void (*write_packet)(unsigned int EPNum, void const *buffer, unsigned short size);
    void (*endpoint_ctl_status)(int EPNum, unsigned int *ret);
    void (*read_packet)(unsigned int EPNum, void const *buffer, unsigned short size);
    void (*set_device_address)(unsigned char address);
    void (*get_rcv_packet_size)(unsigned int EPNum, unsigned short *bcnt);
    void (*polling)(unsigned int ui32Status);
    void (*get_int_status)(unsigned int *ui32IntStatus);
} USBDriver;

extern const USBDriver usb_driver;
extern const USBEvents usb_events;


/// Function: USBDeviceInit
/// Parameters: None
/// Description:
/// This function will init USB module to Device Mode

#ifdef __cplusplus
}
#endif
#endif
