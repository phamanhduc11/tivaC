#include <stdint.h>
#include <stddef.h>
#include "utils/uartstdio.h"
#include "usbd_framework.h"
#include "usbd_driver.h"
#include "usb_standards.h"
#include "usb_device.h"
#include "gtimer_driver.h"
#include "log_debug.h"
#include "usb_descriptors.h"
#include "math.h"

static USBDevice *usbd_handle = 0;
const USBEvents usb_events;

void USBDInit(USBDevice *usb_device) {

    usb_driver.initialize_gpio_pins();
    usb_driver.initialize_core();
    usbd_handle = usb_device;
    usb_driver.connect();
}

void USBDisable(void) {
    usb_driver.disconnect();
}

void USBDEnable(void) {
    usb_driver.connect();
}

static void usb_reset_received_handler()
{
	usbd_handle->in_data_size = 0;
	usbd_handle->out_data_size = 0;
	usbd_handle->configuration_value = 0;
	usbd_handle->device_state = USB_DEVICE_STATE_DEFAULT;
	usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_SETUP;
	usb_driver.set_device_address(0);
}

static void process_standard_device_request(void) {
    USBRequest const *request = usbd_handle->ptr_out_buffer;
    switch (request->bRequest) {
    case USB_STANDARD_GET_DESCRIPTOR:
        log_info("Standard Get Descriptor request received.");
        const unsigned char descriptor_type = request->wValue >> 8;
        const unsigned short descriptor_length = request->wLength;
        switch (descriptor_type) {
        case USB_DESCRIPTOR_TYPE_DEVICE:
            log_info("- Get Device Descriptor.");
            usbd_handle->ptr_in_buffer = &device_descriptor;
            usbd_handle->in_data_size = descriptor_length;
            log_info("Switch control stage to  IN-DATA.");
            usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_DATA_IN;
            break;
        }
        break;
    default:
        log_info("Other request: TODO");
        break;
    }
}

static void process_request(void) {
    USBRequest const *request = usbd_handle->ptr_out_buffer;
    switch(request->bmRequestType & (USB_BM_REQUEST_TYPE_TYPE_MASK | USB_BM_REQUEST_TYPE_RECIPIENT_MASK)) {
        case USB_BM_REQUEST_TYPE_TYPE_STANDARD | USB_BM_REQUEST_TYPE_RECIPIENT_DEVICE:
            process_standard_device_request();
            break;
        default:
            break;
    }
}

static void process_control_transfer_stage() {
    switch(usbd_handle->control_transfer_stage) {
        case USB_CONTROL_STAGE_SETUP:
            break;
        case USB_CONTROL_STAGE_DATA_IN:
            log_info("Processing IN-DATA stage.");
            unsigned char data_size = MIN(usbd_handle->in_data_size, device_descriptor.bMaxPacketSize0);
            usb_driver.write_packet(0, usbd_handle->ptr_in_buffer, data_size);
            usbd_handle->in_data_size -= data_size;
            usbd_handle->ptr_in_buffer += data_size;

            log_info("Switching control stage to IN-DATA IDLE.");
            usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_DATA_IN_IDLE;

            if (usbd_handle->in_data_size == 0) {
                if (data_size == device_descriptor.bMaxPacketSize0) {
                    log_info("Switching control stage to IN-DATA ZERO.");
                    usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_DATA_IN_ZERO;
                }
                else {
                    log_info("Switching control stage to OUT-STATUS.");
                    usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_STATUS_OUT;
                }
            }

            break;
        case USB_CONTROL_STAGE_DATA_IN_IDLE:
            break;
        case USB_CONTROL_STAGE_STATUS_OUT:
            usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_SETUP;
        default:
            break;
    }
}

static void in_transfer_completed_handler(unsigned int endpoint_number)
{
	if (usbd_handle->in_data_size)
	{
		log_info("Switching control stage to IN-DATA.");
		usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_DATA_IN;
	}
	else if (usbd_handle->control_transfer_stage == USB_CONTROL_STAGE_DATA_IN_ZERO)
	{
		usb_driver.write_packet(0, NULL, 0);
		log_info("Switching control stage to OUT-STATUS.");
		usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_STATUS_OUT;
	}

	// if (endpoint_number == (configuration_descriptor_combination.usb_mouse_endpoint_descriptor.bEndpointAddress & 0x0F))
	// {
	// 	write_mouse_report();
	// }
}

static void setup_data_received_handler(unsigned int EPNum, unsigned short byte_count) {
    if(usbd_handle != 0) {
        extern unsigned char debugBuffer[1024];
        extern unsigned char trigger;
        unsigned int clockTick = SysTickCurrentGet();
        usb_driver.read_packet(EPNum, usbd_handle->ptr_out_buffer, byte_count);
        log_debug_array("SETUP data: ", (unsigned char*)usbd_handle->ptr_out_buffer, byte_count);
        // software setup
        process_request();
        // software request
        process_control_transfer_stage();
        // update current transfer stage
        in_transfer_completed_handler(0);

    } 
    else {
        // ERROR
        while(1);
    }
}


static void USBDEndpointHandler(unsigned int EPNum, unsigned int u32IntStatus) {
    unsigned int endpointStatus = 0;
    unsigned char endPointType = 0;
    unsigned short bcnt = 0;
    // Get Endpoint control status
    usb_driver.endpoint_ctl_status(EPNum, &endpointStatus);
    // Get Endpoint received packet size;
    usb_driver.get_rcv_packet_size(EPNum, &bcnt);
    log_info("Endpoint control status of EP%d is 0x%x and packet size=%d.", EPNum, endpointStatus, bcnt);
    // ToDo: depends on status
    switch (EPNum) {
        case 0:
            // Received handle
            if (endpointStatus & 0x1) {
                usb_events.control_transfer_handle(EPNum, bcnt);
            }
            // Transmit handle
            if (endpointStatus & 0x2) {

            }
            // Stalled handle
            if (endpointStatus & 0x4) {

            }
            // DataEnd handle
            if (endpointStatus & 0x8) {

            }
            // SetEnd handle
            if (endpointStatus & 0x10) {

            }

            // 0x100 Flush request
            break;
        default:
            if (GET_TX_ENDPOINT_INT_MASK(u32IntStatus)) {
                log_error("[%s][%d] TODO", __FUNCTION__, __LINE__);
            }
            else if (GET_RX_ENDPOINT_INT_MASK(u32IntStatus)) {
                log_error("[%s][%d] TODO", __FUNCTION__, __LINE__);
            }
            else {
                log_error("False case!!!!!");
            }
            break;
    }
    

}

#define USB_GENERAL_INT_DEVICE_SUSPEND  0x1
#define USB_GENERAL_INT_DEVICE_RESUME   0x2
#define USB_GENERAL_INT_DEVICE_RESET    0x4
#define USB_GENERAL_INT_DEVICE_SOF      0x8
#define USB_GENERAL_INT_DEVICE_DISCON   0x20

void USBInterruptStatus(void) {
    unsigned int ui32IntStatus = 0;
    // TODO: Check other interrupt registers
    usb_driver.get_int_status(&ui32IntStatus);
    usb_driver.polling(ui32IntStatus);
    // USBDeviceGetIntStatus(&ui32IntStatus);
    // // Endpoint Handler
    // if(ui32IntStatus & 0x100){
    //     unsigned int eSize = 0;
    //     unsigned int i = 0;
    //     unsigned int clockTick = SysTickCurrentGet();
    //     unsigned int EPStatus = 0;
    //     unsigned char array[16] = {0};
    //     eSize = USBGetRcvBytes(0);
    //     usb_driver.read_packet(0, array, eSize);
    //     usb_driver.endpoint_ctl_status(0, 0, &EPStatus);
    //     UARTprintf("%x %d %x %x:",ui32IntStatus, eSize, EPStatus,clockTick);
    //     for (i=0; i < eSize; i++) {
    //         UARTprintf("%02x ", array[i]);
    //     }
    //     UARTprintf("\n");
    // }
}


const USBEvents usb_events = {
	.on_usb_reset_received = &usb_reset_received_handler,
	.control_transfer_handle = &setup_data_received_handler,
    .on_endpoint_handle = &USBDEndpointHandler,
	.on_in_transfer_completed = &in_transfer_completed_handler,
	// .on_out_transfer_completed = &out_transfer_completed_handler
};
