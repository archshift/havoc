#pragma once

#include <memory>

struct libusb_context;
struct libusb_device_handle;

class LibusbContext {
    LibusbContext() {};
public:
    typedef std::unique_ptr<LibusbContext> Pointer;

    libusb_context* context;

    static Pointer Initialize();
    static Pointer Claim(libusb_context* context);
    ~LibusbContext();
};

class LibusbDeviceHandle {
    LibusbDeviceHandle() {};
public:
    typedef std::unique_ptr<LibusbDeviceHandle> Pointer;

    libusb_device_handle* handle;

    static Pointer Claim(libusb_device_handle* context);
    ~LibusbDeviceHandle();
};
