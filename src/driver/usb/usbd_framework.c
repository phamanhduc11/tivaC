#include <stdint.h>
#include <stddef.h>
#include "utils/uartstdio.h"

#include "INC/usb/usbd_framework.h"
#include "INC/usb/usbd_driver.h"
#include "INC/usb/usb_standards.h"
#include "INC/usb/usb_device.h"
#include "INC/gtimer_driver.h"
#include "INC/usb/log_debug.h"
#include "INC/usb/usb_descriptors.h"
#include "INC/usb/math.h"

static USBDevice *usbd_handle = 0;
const USBEvents usb_events;

void USBDInit(USBDevice *usb_device) {

    usb_driver.initialize_gpio_pins();
    usb_driver.initialize_core();
    usbd_handle = usb_device;
    usb_driver.connect();
}

void usbd_configure() {
    struct endpointInfo ep3Info = {
        .epType = USB_ENDPOINT_TYPE_INTERRUPT,
        .isDoubleBuffering = 0,
        .maxPacketSize = usb_config_set.mouse_endpoint_desc.wMaxPacketSize,
        .FIFORamAddr = 64,
        .FIFORamSize = USB_FIFO_RAM_SIZE_64,
    };

    usb_driver.configure_in_endpoint(0x3, ep3Info);

    usb_driver.send_ep_data(0x3, true);
}
void USBDisable(void) {
    usb_driver.disconnect();
}

void USBDEnable(void) {
    usb_driver.connect();
}

static void logging_request_info(void)
{
    USBRequest const *request = usbd_handle->ptr_out_buffer;
    uint8_t bmRequestTypeDir = request->bmRequestType & USB_BM_REQUEST_TYPE_DIRECTION_MASK;
    uint8_t bmRequestTypeType = request->bmRequestType & USB_BM_REQUEST_TYPE_TYPE_MASK;
    uint8_t bmRequestTypeRecv = request->bmRequestType & USB_BM_REQUEST_TYPE_RECIPIENT_MASK;
    log_info("bmRequestType info:");
    log_info("    Direction: %s", (bmRequestTypeDir ? "Device to Host" : "Host to Device"));
    log_info("    Type: %s", (bmRequestTypeType == USB_BM_REQUEST_TYPE_TYPE_STANDARD ? 
                                    "Standard" : bmRequestTypeType == USB_BM_REQUEST_TYPE_TYPE_CLASS ?
                                    "Class" : bmRequestTypeType == USB_BM_REQUEST_TYPE_TYPE_VENDOR ?
                                    "Vendor" : "Reserved!!!"));
    log_info("    Recipient: %s", (bmRequestTypeRecv == USB_BM_REQUEST_TYPE_RECIPIENT_DEVICE ?
                                    "Device" : bmRequestTypeRecv == USB_BM_REQUEST_TYPE_RECIPIENT_INTERFACE ?
                                    "Interface" : bmRequestTypeRecv == USB_BM_REQUEST_TYPE_RECIPIENT_ENDPOINT ?
                                    "Endpoint" : bmRequestTypeRecv == USB_BM_REQUEST_TYPE_RECIPIENT_OTHER ?
                                    "Other" : "Reserved!!!"));

    log_info("bRequest info: ");
    switch (request->bRequest)
    {
    case USB_STANDARD_GET_STATUS:
        log_info("    GET_STATUS");
        break;
    case USB_STANDARD_CLEAR_FEATURE:
        log_info("    CLEAR_FEATURE");
        break;
    case USB_STANDARD_SET_FEATURE:
        log_info("    SET_FEATURE");
        break;
    case USB_STANDARD_SET_ADDRESS:
        log_info("    SET_ADDRESS");
        break;
    case USB_STANDARD_GET_DESCRIPTOR:
        log_info("    GET_DESCRIPTOR");
        break;
    case USB_STANDARD_SET_DESCRIPTOR:
        log_info("    SET_DESCRIPTOR");
        break;
    case USB_STANDARD_GET_CONFIG:
        log_info("    GET_CONFIG");
        break;
    case USB_STANDARD_SET_CONFIG:
        log_info("    SET_CONFIG");
        break;
    case USB_STANDARD_GET_INTERFACE:
        log_info("    GET_INTERFACE");
        break;
    case USB_STANDARD_SET_INTERFACE:
        log_info("    SET_INTERFACE");
        break;
    case USB_STANDARD_SYNCH_FRAME:
        log_info("    SYNCH_FRAME");
        break;
    default:
        log_error("    Error bRequest value %02x!!!", request->bRequest);
        break;
    }

    log_info("wValue  : %02X", request->wValue);
    log_info("wIndex  : %02X", request->wIndex);
    log_info("wwLength: %02X", request->wLength);
}

static bool reset_flag = false;
static volatile unsigned short dev_addr = 0;

static void usb_reset_received_handler()
{
    log_info("USB Reset set handling!");
	usbd_handle->in_data_size = 0;
	usbd_handle->out_data_size = 0;
	usbd_handle->configuration_value = 0;
	usbd_handle->device_state = USB_DEVICE_STATE_DEFAULT;
	usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_SETUP;
	usb_driver.set_device_address(0);
    reset_flag = true;
}

static void process_standard_device_request(void) {
    USBRequest const *request = usbd_handle->ptr_out_buffer;
    switch (request->bRequest) {
    case USB_STANDARD_GET_DESCRIPTOR:
        usb_driver.ack_ep_rcv(0, false);

        log_info("Standard Get Descriptor request received.");
        const unsigned char descriptor_type = request->wValue >> 8;
        const unsigned short descriptor_length = request->wLength;
        const unsigned char descriptor_index = request->wValue & 0xFF;

        switch (descriptor_type) {
        case USB_DESCRIPTOR_TYPE_DEVICE:
            log_info("- Get Device Descriptor.");
            usbd_handle->ptr_in_buffer = &device_descriptor;
            // todo: descriptor length > real device_descriptor's size
            usbd_handle->in_data_size = descriptor_length;
            log_info("Switch control stage to  IN-DATA.");
            usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_DATA_IN;
            break;

        case USB_DESCRIPTOR_TYPE_CONFIGURATION:
            log_info("- Get Config Descriptor.");
            usbd_handle->ptr_in_buffer = &usb_config_set;
            usbd_handle->in_data_size = MIN(descriptor_length, sizeof(usb_config_set));
            log_info("Switch control stage to  IN-DATA.");
            usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_DATA_IN;
            break;

        default:
            log_error("[%s] TODO descriptorType=%d", __FUNCTION__, descriptor_type);
        }
        break;
    case USB_STANDARD_SET_ADDRESS:
        {
            if (true == reset_flag) {
                reset_flag = false;
                dev_addr = 0;
            };

            dev_addr = request->wValue | 0x8000;
            log_info("Standard Set Address %X request received.", dev_addr);

            usbd_handle->device_state = USB_DEVICE_STATE_ADDRESSED;
            log_info("Switching control transfer stage to IN-STATUS.");
            usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_STATUS_IN;
            break;
        }

    case USB_STANDARD_SET_CONFIG:
        log_info("Standard Set Configuration request received.");
        usbd_handle->configuration_value = request->wValue;

        usbd_configure();

        usbd_handle->device_state = USB_DEVICE_STATE_CONFIGURED;
        log_info("Switching control transfer to IN-STATUS.");
        usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_STATUS_IN;
        break;
    default:

        log_info("Other request: TODO bRequest=%d", request->bRequest);
        break;
    }
}

static void process_class_interface_request() {
    USBRequest const *request = usbd_handle->ptr_out_buffer;
    switch (request->bRequest) {
    case USB_HID_SETIDLE:
        log_info("Switching control transfer to IN-STATUS.");
        usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_STATUS_IN;
        break;
    }

}

static process_standard_interface_request() {
    USBRequest const *request = usbd_handle->ptr_out_buffer;
    switch (request->wValue >> 8) {
    case USB_DESCRIPTOR_TYPE_HID_REPORT:
        usbd_handle->ptr_in_buffer = &hid_report_descriptor;
        usbd_handle->in_data_size = sizeof(hid_report_descriptor);

        log_info("Switching control transfer to IN-STATUS from %s.", __FUNCTION__);
        usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_DATA_IN;
        break;
    default:
        break;
    }
}

static void process_request(void) {
    USBRequest const *request = usbd_handle->ptr_out_buffer;
    // UARTprintf("---Dummy log---\r\n");
    logging_request_info();
    switch(request->bmRequestType & (USB_BM_REQUEST_TYPE_TYPE_MASK | USB_BM_REQUEST_TYPE_RECIPIENT_MASK)) {
        case USB_BM_REQUEST_TYPE_TYPE_STANDARD | USB_BM_REQUEST_TYPE_RECIPIENT_DEVICE:
            process_standard_device_request();
            break;

        case USB_BM_REQUEST_TYPE_TYPE_CLASS | USB_BM_REQUEST_TYPE_RECIPIENT_INTERFACE:
            process_class_interface_request();
            break;

        case USB_BM_REQUEST_TYPE_TYPE_STANDARD | USB_BM_REQUEST_TYPE_RECIPIENT_INTERFACE:
            process_standard_interface_request();
            break;

        default:
            log_error("[%s]Other request: TODO", __FUNCTION__);
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

            log_info("Switching control stage to IN-DATA IDLE after sending %d bytes.", data_size);
            usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_DATA_IN_IDLE;

            if (usbd_handle->in_data_size == 0) {
                if (data_size == device_descriptor.bMaxPacketSize0) {
                    if(usb_driver.send_ep_data(0, false)) {
                        log_error("[%d]Endpoint is already in transmitting!!!",__LINE__);
                    }

                    log_info("Switching control stage to IN-DATA ZERO.");
                    usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_DATA_IN_ZERO;
                }
                else {
                    if(1 == usb_driver.send_ep_data(0, true)) {
                        log_error("[%d]Endpoint is already in transmitting!!!",__LINE__);
                    }

                    log_info("Switching control stage to OUT-STATUS.");
                    usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_STATUS_OUT;
                }
            }

            break;
        case USB_CONTROL_STAGE_DATA_IN_IDLE:
            break;
        case USB_CONTROL_STAGE_DATA_IN_ZERO:
		    usb_driver.write_packet(0, NULL, 0);

            if(1 == usb_driver.send_ep_data(0, true)) {
                log_error("[%d]Endpoint is already in transmitting!!!",__LINE__);
            }

            log_info("Switching control stage to OUT-STATUS.");
            usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_STATUS_OUT;
            break;

        case USB_CONTROL_STAGE_STATUS_IN:
            usb_driver.ack_ep_rcv(0, true);
            // move code to in_transfer_completed_handler dev_addr
            break;

        case USB_CONTROL_STAGE_STATUS_OUT:
            usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_SETUP;
        default:
            break;
    }
}

static void write_mouse_report() {
    log_debug("Sending USB HID mouse report.");

    HIDReport hid_report = {
        .x = 5,
    };

    usb_driver.write_packet((usb_config_set.mouse_endpoint_desc.bEndpointAddress & 0x0F),
                            &hid_report,
                            sizeof(hid_report));
    
    usb_driver.send_ep_data(3, true);
}

static void in_transfer_completed_handler(unsigned int endpoint_number)
{
	if (usbd_handle->in_data_size)
	{
		log_info("Switching control stage to IN-DATA.");
		usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_DATA_IN;
	}

	if (endpoint_number == (usb_config_set.mouse_endpoint_desc.bEndpointAddress & 0x0F))
	{
		write_mouse_report();
	}
}

// Handle Endpoint 0 process
static void setup_data_received_handler(unsigned int EPNum, unsigned short byte_count) {
    if(usbd_handle != 0) {
        extern unsigned char debugBuffer[1024];
        extern unsigned char trigger;
        
        usb_driver.read_packet(EPNum, usbd_handle->ptr_out_buffer, byte_count);
        log_debug_array("SETUP data: ", (unsigned char*)usbd_handle->ptr_out_buffer, byte_count);
        // update current transfer stage
        in_transfer_completed_handler(0);
        // software setup
        process_request();

    } 
    else {
        // ERROR
        while(1);
    }
}


// Todo: check if this process should be done in framework or driver
static void USBDEndpointHandler(unsigned int EPNum, unsigned int ui32IntStatus) {
    unsigned int endpointStatus = 0;
    unsigned char endPointType = 0;
    unsigned short bcnt = 0;
    // Get Endpoint control status
    usb_driver.endpoint_ctl_status(EPNum, &endpointStatus);
    // Get Endpoint received packet size;
    usb_driver.get_rcv_packet_size(EPNum, &bcnt);
    log_info("NewStart - Endpoint control status of EP%d is 0x%x and packet size=%d.", EPNum, endpointStatus, bcnt);

    switch (EPNum) {
        case 0:
            // Set address later => temp workaround for set address after finish SET_ADDRESS transaction
            if (0 == endpointStatus) {
                log_info("Empty response from Host");
            }
            if ((USB_DEVICE_STATE_ADDRESSED == usbd_handle->device_state) && (dev_addr & 0x8000) && (USB_CONTROL_STAGE_STATUS_IN == usbd_handle->control_transfer_stage)) {
                dev_addr &= 0xFF;
                usb_driver.set_device_address(dev_addr);
                log_info("Switching control transfer stage to SETUP after SET_ADDRESS.");
                usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_SETUP;
            }
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
        case 3:
            in_transfer_completed_handler(3);
            break;
        default:
            if (GET_TX_ENDPOINT_INT_MASK(ui32IntStatus)) {
                log_error("[%s][%d] TODO", __FUNCTION__, __LINE__);
            }
            else if (GET_RX_ENDPOINT_INT_MASK(ui32IntStatus)) {
                log_error("[%s][%d] TODO", __FUNCTION__, __LINE__);
            }
            else {
                log_error("False case!!!!!");
            }
            break;
    }
        // software request
    process_control_transfer_stage();
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
