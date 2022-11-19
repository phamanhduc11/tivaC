#ifndef _USBD_FRAMEOWRK_H_
#define _USBD_FRAMEWORK_H_
#include "usbd_driver.h"
#include "usb_device.h"

void USBDInit(USBDevice *usb_device); 
void USBDisable(void);
void USBEnable(void);
void USBInterruptStatus(void);
#endif
