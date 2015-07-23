#include <cstdio>


#include "hardware-interface.h"
#include "profile-interface.h"
#include "havoc.h"

int main(int argc, const char* argv[]) {
    if (!HW::Initialize()) {
        printf("Failed to initialize hardware interface!\n");
        return -1;
    }

    ProfileSettings settings {
        DpiSetting::DPI_1,
        LedMode::ON,
        LedBrightness::PERCENT_100,
        Color::BLUE,
        0.5f,
        false,
    };

    if (!ProfileInterface::SendSettings(profile, settings)) {
        printf("Sending profile settings to device failed!\n");
    }

    HW::Deinitialize();
    return 0;
}
