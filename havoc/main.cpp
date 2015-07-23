#include <cstdio>

#include <libusb-1.0/libusb.h>

#include "hardware-interface.h"
#include "havoc.h"

int main(int argc, const char * argv[]) {
    if (!HW::Initialize()) {
        printf("Failed to initialize hardware interface!\n");
        return -1;
    }

    ProfileSettings settings {
        LedMode::ON,
        Color::TEAL,
    };

    if (!HW::SendProfileSettings(settings)) {
        printf("Sending profile settings to device failed!\n");
    }

    HW::Deinitialize();
    return 0;
}
