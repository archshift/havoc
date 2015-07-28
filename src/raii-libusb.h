// Copyright 2015 archshift
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

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
    bool interface_drivers_unloaded[0xFF] = {false};
    bool interface_claims[0xFF] = {false};

    LibusbDeviceHandle() {};
public:
    typedef std::unique_ptr<LibusbDeviceHandle> Pointer;

    libusb_device_handle* handle;

    static Pointer Claim(libusb_device_handle* context);
    ~LibusbDeviceHandle();

    bool DriverUnload(int interface);
    bool DriverReload(int interface);
    bool InterfaceClaim(int interface);
    bool InterfaceRelease(int interface);
};