#ifndef __USB_HID__
#define __USB_HID__
#include "usb_hid.h"
#include "usb_hid_usage_button.h"
#include "usb_hid_usage_desktop.h"

#define USB_DESCRIPTOR_TYPE_HID 0x21
#define USB_DESCRIPTOR_TYPE_HID_REPORT 0x22

#define USB_HID_COUNTRY_NONE 0

typedef struct {
    unsigned char bLength;
    unsigned char bDescriptorType;
    unsigned short bcdHID;
    unsigned char bCountryCode;
    unsigned char bNumDescriptors;
    unsigned char bDescriptorType0;
    unsigned short wDescriptorLength0;
} __attribute__((__packed__)) USBHIDDescriptor;

#endif