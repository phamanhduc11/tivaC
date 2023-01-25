#ifndef _USB_DEVICE_
#define _USB_DEVICE_
#include "usb_standards.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    USBDeviceState device_state;
    USBControlTransferStage control_transfer_stage;
    unsigned char configuration_value;
    void const *ptr_out_buffer;
    unsigned int out_data_size;
    void const *ptr_in_buffer;
    unsigned int in_data_size;
} USBDevice;

#ifdef __cplusplus
}
#endif
#endif
