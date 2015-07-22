#include <cstdio>

#include <libusb-1.0/libusb.h>

#include "hardware-interface.h"
#include "havoc.h"

int main(int argc, const char * argv[]) {
    int status = 0;
    libusb_context* usb_context = nullptr;

    status = libusb_init(&usb_context);
    if (status < 0) {
        printf("`libusb_init` failed with err %i\n", status);
        return -1;
    }
    libusb_set_debug(usb_context, 3); // Very verbose!

    {
        // TODO: Support multiple mice
        libusb_device_handle* havoc_device_handle = libusb_open_device_with_vid_pid(usb_context, 0x2516, 0x001d);
        if (havoc_device_handle == nullptr) {
            printf("`libusb_open_device_with_vid_pid` failed to open device for vid=0x2516, pid=0x001d\n");
            return -1;
        }

        ProfileSettings settings {
            LedMode::ON,
            Color::TEAL,
        };

        if (!SendProfileSettings(havoc_device_handle, settings)) {
            printf("Sending profile settings to device failed!\n");
            return -1;
        }

        libusb_close(havoc_device_handle);
    }

    libusb_exit(usb_context);
    return 0;
}
