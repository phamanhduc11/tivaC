#ifndef _USBD_FRAMEOWRK_H_
#define _USBD_FRAMEWORK_H_
#include "usbd_driver.h"
#include "usb_device.h"

#ifdef __cplusplus
extern "C"
{
#endif

void USBDInit(USBDevice *usb_device); 
void USBDisable(void);
void USBEnable(void);
void USBInterruptStatus(void);

#ifdef __cplusplus
}
#endif
#endif
