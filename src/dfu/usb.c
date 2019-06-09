#include "usb.h"
#include <stdint.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/crs.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/usb/dfu.h>
#include <stddef.h>
#include <string.h>
#include "main.h"

// https://git.drak.xyz/flabbergast/opencm3-ex/src/commit/6f1916fc2a5b3acd9b26a191eeb7ef4dc5697553/usb_dfu/usbdfu.c
// https://github.com/devanlai/dapboot

#define DFU_TRANSFER_SIZE PAGE_SIZE

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

static uint8_t usbd_control_buffer[DFU_TRANSFER_SIZE];
static bool handle_page_called;

static const char * const usb_strings[] = {
	"DotMatrix", // iManufacturer
	"Distributed DFU", // iProduct
	"NONE", // Serial number
	"flash"
};

static usbd_device *dfu_usbd_dev;
static enum dfu_state usbdfu_state = STATE_DFU_IDLE;

static const struct usb_device_descriptor desc_dev = {
	.bLength = USB_DT_DEVICE_SIZE,
	.bDescriptorType = USB_DT_DEVICE,
	.bcdUSB = 0x0200,
	.bDeviceClass = 0,
	.bDeviceSubClass = 0,
	.bDeviceProtocol = 0,
	.bMaxPacketSize0 = 64,
	.idVendor = 0xdead,
	.idProduct = 0xca5d,
	.bcdDevice = 0x0200,
	.iManufacturer = 1,
	.iProduct = 2,
	.iSerialNumber = 3,
	.bNumConfigurations = 1,
};

static const struct usb_dfu_descriptor dfu_function = {
	.bLength = sizeof(struct usb_dfu_descriptor),
	.bDescriptorType = DFU_FUNCTIONAL,
	.bmAttributes = USB_DFU_CAN_DOWNLOAD | USB_DFU_WILL_DETACH,
	.wDetachTimeout = 255,
	.wTransferSize = DFU_TRANSFER_SIZE,
	.bcdDFUVersion = 0x0110,
};

static const struct usb_interface_descriptor dfu_iface = {
	.bLength = USB_DT_INTERFACE_SIZE,
	.bDescriptorType = USB_DT_INTERFACE,
	.bInterfaceNumber = 0,
	.bAlternateSetting = 0,
	.bNumEndpoints = 0,
	.bInterfaceClass = 0xFE, /* Device Firmware Upgrade */
	.bInterfaceSubClass = 1,
	.bInterfaceProtocol = 2,
	.iInterface = 4,
	.extra = &dfu_function,
	.extralen = sizeof(dfu_function)
};

static const struct usb_interface iface_list[] = {{
	.num_altsetting = 1,
	.altsetting = &dfu_iface,
}};

static const struct usb_config_descriptor desc_config = {
	.bLength = USB_DT_CONFIGURATION_SIZE,
	.bDescriptorType = USB_DT_CONFIGURATION,
	.wTotalLength = 0,
	.bNumInterfaces = 1,
	.bConfigurationValue = 1,
	.iConfiguration = 0,
	.bmAttributes = 0xC0,
	.bMaxPower = 0x32,

	.interface = iface_list,
};

static uint8_t usbdfu_getstatus(uint32_t *bwPollTimeout)
{
	switch (usbdfu_state) {
	case STATE_DFU_DNBUSY:
		if (ready_to_flash && handle_page_called) {
			handle_page_called = false;
			usbdfu_state = STATE_DFU_DNLOAD_IDLE;
		} else {
			*bwPollTimeout = 10;
		}
		return DFU_STATUS_OK;
	case STATE_DFU_DNLOAD_SYNC:
		usbdfu_state = STATE_DFU_DNBUSY;
		*bwPollTimeout = 10;
		return DFU_STATUS_OK;
	case STATE_DFU_MANIFEST_SYNC:
		/* Device will reset when read is complete. */
		usbdfu_state = STATE_DFU_MANIFEST;
		return DFU_STATUS_OK;
	default:
		return DFU_STATUS_OK;
	}
}

static void usbdfu_getstatus_complete(usbd_device *usbd_dev, struct usb_setup_data *req)
{
	(void)req;
	(void)usbd_dev;

	switch (usbdfu_state) {
	case STATE_DFU_DNBUSY:
		if (ready_to_flash) {
			if (!handle_page_called) {
				handle_page_called = true;
				handle_page();
			}
		} else {
			/* remain in busy state */
		}
		return;
	case STATE_DFU_MANIFEST:
		if (ready_to_flash) {
			continue_boot();
		}
		return; /* Will never return. */
	default:
		return;
	}
}

static enum usbd_request_return_codes
dfu_ctrl(
	usbd_device *usbd_dev,
	struct usb_setup_data *req,
	uint8_t **buf,
	uint16_t *len,
	void (**complete)(usbd_device *usbd_dev, struct usb_setup_data *req))
{
	(void)complete;
	(void)buf;
	(void)usbd_dev;

	if ((req->bmRequestType & 0x7F) != 0x21)
			return 0; /* Only accept class request. */

	dfu_activated = true;
	switch (req->bRequest) {
	case DFU_DNLOAD:
		if (usbdfu_state != STATE_DFU_IDLE && usbdfu_state != STATE_DFU_DNLOAD_IDLE)
			return USBD_REQ_NOTSUPP;
		if ((len == NULL) || (*len == 0)) {
			usbdfu_state = STATE_DFU_MANIFEST_SYNC;
			return 1;
		} else {
			if (*len > PAGE_SIZE || req->wValue >= MAX_PAGE_COUNT) {
				usbdfu_state = STATE_DFU_ERROR;
			} else {
				/* Copy download data for use on GET_STATUS. */
				page_number = req->wValue;
				memcpy(page_data, *buf, PAGE_SIZE);
				ready_to_flash = false;
				usbdfu_state = STATE_DFU_DNLOAD_SYNC;
			}
			return 1;
		}
	case DFU_CLRSTATUS:
		/* Clear error and return to dfuIDLE. */
		if (usbdfu_state == STATE_DFU_ERROR)
			usbdfu_state = STATE_DFU_IDLE;
		return 1;
	case DFU_ABORT:
		/* Abort returns to dfuIDLE state. */
		usbdfu_state = STATE_DFU_IDLE;
		return 1;
	case DFU_GETSTATUS: {
			uint32_t bwPollTimeout = 0; /* 24-bit integer in DFU class spec */
			(*buf)[0] = usbdfu_getstatus(&bwPollTimeout);
			(*buf)[1] = bwPollTimeout & 0xFF;
			(*buf)[2] = (bwPollTimeout >> 8) & 0xFF;
			(*buf)[3] = (bwPollTimeout >> 16) & 0xFF;
			(*buf)[4] = usbdfu_state;
			(*buf)[5] = 0; /* iString not used here */
			*len = 6;
			*complete = usbdfu_getstatus_complete;
			return 1;
		}
	case DFU_GETSTATE:
		/* Return state with no state transision. */
		*buf[0] = usbdfu_state;
		*len = 1;
		return 1;
	}

	return USBD_REQ_NOTSUPP;
}

static void dfu_set_config(usbd_device *usbd_dev, uint16_t wValue)
{
	(void)wValue;
	main_mode = MODE_MASTER;
	usbd_register_control_callback(
		usbd_dev,
		USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,
		USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT,
		dfu_ctrl);
}

void usb_init(void) {
	crs_autotrim_usb_enable();
	rcc_set_usbclk_source(RCC_PLL);
	dfu_usbd_dev = usbd_init(&st_usbfs_v2_usb_driver, &desc_dev, &desc_config,
		usb_strings, 4,
		usbd_control_buffer, sizeof(usbd_control_buffer));
	usbd_register_set_config_callback(dfu_usbd_dev, dfu_set_config);
}

void usb_run(void) {
	usbd_poll(dfu_usbd_dev);
}
