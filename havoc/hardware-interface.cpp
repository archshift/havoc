#include "hardware-interface.h"

#include <cstdio>
#include <libusb-1.0/libusb.h>

#include "havoc.h"
#include "raii-libusb.h"

static LibusbContext::Pointer usb_context;
static LibusbDeviceHandle::Pointer havoc_device_handle;

bool HW::Initialize() {
    usb_context = LibusbContext::Initialize();
    if (usb_context == nullptr) {
        return false;
    }
    libusb_set_debug(usb_context->context, 3); // Very verbose!

    // TODO: Support multiple mice
    libusb_device_handle* handle = libusb_open_device_with_vid_pid(usb_context->context, 0x2516, 0x001d);
    if (handle == nullptr) {
        printf("`libusb_open_device_with_vid_pid` failed to open device for vid=0x2516, pid=0x001d\n");
        return false;
    }
    havoc_device_handle = LibusbDeviceHandle::Claim(handle);

    return true;
}

uint64_t HW::MakeWriteEepromCommand(uint16_t eeprom_addr, uint8_t data) {
    return ((uint64_t)(eeprom_addr >> 8) << 24) | ((uint64_t)data << 16) | ((uint64_t)(eeprom_addr & 0xFF) << 8) | 0xDE;
}

uint64_t HW::MakeReadEepromCommand(uint16_t eeprom_addr) {
    return ((uint64_t)(eeprom_addr >> 8) << 24) | ((uint64_t)(eeprom_addr & 0xFF) << 8) | 0xDF;
}

bool HW::CmdSend(uint64_t command) {
    int status = libusb_control_transfer(havoc_device_handle->handle,
                                     LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
                                     9, 0x300, 0, (uint8_t*)(&command), 8, 1000);
    if (status != 8 || status < 0) {
        printf("`libusb_control_transfer` out failed with err %i\n", status);
        return false;
    }
    return true;
}

bool HW::CmdReceive(uint64_t* data) {
    int status = libusb_control_transfer(havoc_device_handle->handle,
                                         LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
                                         1, 0x300, 0, (uint8_t*)data, 8, 1000);
    if (status != 8 || status < 0) {
        printf("`libusb_control_transfer` in failed with err %i\n", status);
        return false;
    }
    return true;
}

bool HW::ReadEepromByte(uint16_t eeprom_addr, uint8_t* byte) {
    return ReadEepromBytes(eeprom_addr, byte, 1);
}

bool HW::ReadEepromBytes(uint16_t eeprom_addr, uint8_t* out_buf, uint16_t num_bytes) {
    for (uint16_t i = 0; i < num_bytes; i++) {
        if (!CmdSend(MakeReadEepromCommand(eeprom_addr + i))) {
            break;
        }

        uint64_t received_data = 0;
        if (!CmdReceive(&received_data)) {
            break;
        }

        uint16_t received_addr = (received_data & 0xFF) | (uint16_t)((received_data & 0xFF0000) >> 8);
        if (received_addr != eeprom_addr + i) {
            printf("Received data from wrong address! Requested=0x%04X, received=0x%04X", eeprom_addr + i, received_addr);
            break;
        }

        out_buf[i] = (received_data & 0xFF00) >> 8;
    }

    if (!CmdSend(0x00000000000006C4)) {
        return false;
    }
    return true;
}

bool HW::WriteEepromByte(uint16_t eeprom_addr, uint8_t byte) {
    return WriteEepromBytes(eeprom_addr, &byte, 1);
}

bool HW::WriteEepromBytes(uint16_t eeprom_addr, const uint8_t* byte_buf, uint16_t num_bytes) {
    if (!CmdSend(0x00000000000106C4)) {
        return false;
    }

    for (uint16_t i = 0; i < num_bytes; i++) {
        if (!CmdSend(MakeWriteEepromCommand(eeprom_addr + i, byte_buf[i]))) {
            break;
        }
    }

    if (!CmdSend(0x00000000000000C4)) {
        return false;
    }
    if (!CmdSend(0x00000000000006C4)) {
        return false;
    }
    return true;
}
