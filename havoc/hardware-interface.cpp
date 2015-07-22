#include "hardware-interface.h"

#include <cstdio>
#include <libusb-1.0/libusb.h>

#include "havoc.h"

bool SendProfileSettings(libusb_device_handle* device_handle, const ProfileSettings& settings) {
    int status = 0;

    uint8_t data[6][8] = {
        {0xC4, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0xDE, 0x0B, (uint8_t)(((uint8_t)settings.led_mode << 4) | ((uint8_t)settings.color & 0xF)), 0x01, 0x00, 0x00, 0x00, 0x00},
        {0xDE, 0x0C, (uint8_t)(((uint8_t)settings.led_mode << 4) | ((uint8_t)settings.color & 0xF)), 0x01, 0x00, 0x00, 0x00, 0x00},
        {0xDE, 0x0D, (uint8_t)(((uint8_t)settings.led_mode << 4) | ((uint8_t)settings.color & 0xF)), 0x01, 0x00, 0x00, 0x00, 0x00},
        {0xC4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0xC4, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    };

    for (int i = 0; i < 6; i++) {
        status = libusb_control_transfer(device_handle, LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE, 9, 0x300, 0, data[i], 8, 1000);
        if (status != 8 || status < 0) {
            printf("`libusb_control_transfer` failed with err %i\n", status);
            return false;
        }
    }
    return true;
}