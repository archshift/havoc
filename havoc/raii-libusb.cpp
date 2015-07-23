#include "raii-libusb.h"

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
    libusb_close(handle);
    handle = nullptr;
}