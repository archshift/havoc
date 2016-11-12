// Copyright 2015 archshift
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "raii-libusb.h"

#include <cassert>

#include <libusb-1.0/libusb.h>

LibusbContext::Pointer LibusbContext::Initialize() {
    libusb_context* context = nullptr;
    int status = libusb_init(&context);
    if (status < 0) {
        printf("`libusb_init` failed with err %i\n", status);
        return nullptr;
    }
    return Claim(context);
}

LibusbContext::Pointer LibusbContext::Claim(libusb_context* context) {
    auto new_context = LibusbContext::Pointer(new LibusbContext());
    new_context->context = context;
    return new_context;
}

LibusbContext::~LibusbContext() {
    libusb_exit(context);
    context = nullptr;
}

LibusbDeviceHandle::Pointer LibusbDeviceHandle::Claim(libusb_device_handle* handle) {
    auto new_handle = LibusbDeviceHandle::Pointer(new LibusbDeviceHandle());
    new_handle->handle = handle;
    return new_handle;
}

LibusbDeviceHandle::~LibusbDeviceHandle() {
    for (int i = 0; i < 0xFF; i++) {
        if (interface_claims[i]) {
            InterfaceRelease(i);
        }
        if (interface_drivers_unloaded[i]) {
            DriverReload(i);
        }
    }
    libusb_close(handle);
    handle = nullptr;
}

bool LibusbDeviceHandle::DriverUnload(int interface) {
    assert(interface >= 0 && interface <= 0xFF);

    int status = libusb_kernel_driver_active(handle, interface);
    if (status == 1) {
        status = libusb_detach_kernel_driver(handle, interface);
        if (status != 0 && status != LIBUSB_ERROR_NOT_SUPPORTED) {
            printf("`libusb_detach_kernel_driver` failed with err: %i\n", status);
            return false;
        }
    } else if (status != 0 && status != LIBUSB_ERROR_NOT_SUPPORTED) {
        printf("`libusb_kernel_driver_active` failed with err = %i\n", status);
        return false;
    }

    interface_drivers_unloaded[interface] = true;
    return true;
}

bool LibusbDeviceHandle::DriverReload(int interface) {
    assert(interface >= 0 && interface <= 0xFF);

    int status = libusb_kernel_driver_active(handle, interface);
    if (status == 0) {
        status = libusb_attach_kernel_driver(handle, interface);
        if (status != 0 && status != LIBUSB_ERROR_NOT_SUPPORTED) {
            printf("`libusb_attach_kernel_driver` failed with err: %i\n", status);
            return false;
        }
    } else if (status != 1 && status != LIBUSB_ERROR_NOT_SUPPORTED) {
        printf("`libusb_kernel_driver_active` failed with err = %i\n", status);
        return false;
    }

    interface_drivers_unloaded[interface] = false;
    return true;
}

bool LibusbDeviceHandle::InterfaceClaim(int interface) {
    assert(interface >= 0 && interface <= 0xFF);

    int status = libusb_claim_interface(handle, interface);
    if (status != 0) {
        printf("`libusb_claim_interface` failed with err: %i\n", status);
        return false;
    }

    interface_claims[interface] = true;
    return true;
}

bool LibusbDeviceHandle::InterfaceRelease(int interface) {
    assert(interface >= 0 && interface <= 0xFF);

    int status = libusb_release_interface(handle, interface);
    if (status != 0) {
        printf("`libusb_release_interface` failed with err: %i\n", status);
        return false;
    }

    interface_claims[interface] = false;
    return true;
}
