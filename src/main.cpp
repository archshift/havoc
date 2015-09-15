// Copyright 2015 archshift
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <cstdio>

#include <tclap/CmdLine.h>

#include "hardware-interface.h"
#include "profile-interface.h"
#include "havoc.h"

int main(int argc, const char* argv[]) {
    if (!HW::Initialize()) {
        printf("Failed to initialize hardware interface!\n");
        return -1;
    }

    Profile profile;
    ProfileSettings settings = {};

    TCLAP::CmdLine cmd("A configuration tool for the CMStorm Havoc mouse.", ' ', "0.1");

    TCLAP::ValueArg<std::string> arg_profile("p", "profile", "Profile to edit", true, "", "profile #", cmd);
    TCLAP::ValueArg<std::string> arg_dpi_setting("", "active-dpi", "DPI configuration to switch to", false, "", "dpi config #", cmd);
    TCLAP::ValueArg<std::string> arg_led_mode("", "led-mode", "LED mode", false, "", "mode", cmd);
    TCLAP::ValueArg<std::string> arg_led_brightness("", "led-brightness", "LED brightness", false, "", "percent", cmd);
    TCLAP::ValueArg<std::string> arg_color("", "color", "LED color", false, "", "color", cmd);
    TCLAP::ValueArg<std::string> arg_button_response("", "button-responsiveness", "Responsiveness of mouse buttons", false, "", "ratio", cmd);
    TCLAP::ValueArg<std::string> arg_angle_snapping("", "angle-snap", "Enable angle snapping", false, "", "bool", cmd);

    try {
        cmd.parse(argc, argv);

        try {
            profile = bimap_profile.GetEnum(arg_profile.getValue());
        } catch (std::invalid_argument e) {
            printf("Unknown profile specified!\n");
            return -1;
        }
    } catch (TCLAP::ArgException &e) {
        printf("Error: %s for arg %s\n", e.error().c_str(), e.argId().c_str());
        return -1;
    }

    if (!ProfileInterface::ReceiveSettings(profile, &settings)) {
        printf("Receiving profile settings from device failed!\n");
        return -1;
    }

    try {
        if (arg_dpi_setting.isSet()) {
            try {
                settings.active_dpi = bimap_dpi_setting.GetEnum(arg_dpi_setting.getValue());
            } catch (std::invalid_argument e) {
                printf("Unknown DPI configuration specified!\n");
                return -1;
            }
        }
        if (arg_led_mode.isSet()) {
            try {
                settings.led_mode = bimap_led_mode.GetEnum(arg_led_mode.getValue());
            } catch (std::invalid_argument e) {
                printf("Unknown LED mode specified!\n");
                return -1;
            }
        }
        if (arg_led_brightness.isSet()) {
            try {
                settings.led_brightness = bimap_led_brightness.GetEnum(arg_led_brightness.getValue());
            } catch (std::invalid_argument e) {
                printf("Unknown LED brightness specified!\n");
                return -1;
            }
        }
        if (arg_color.isSet()) {
            try {
                settings.color = bimap_color.GetEnum(arg_color.getValue());
            } catch (std::invalid_argument e) {
                printf("Unknown color specified!\n");
                return -1;
            }
        }
        if (arg_button_response.isSet()) {
            try {
                settings.button_response = DecodeButtonResponse(arg_button_response.getValue());
            } catch (std::invalid_argument e) {
                printf("Unknown button responsiveness setting specified!\n");
                return -1;
            }
        }
        if (arg_angle_snapping.isSet()) {
            try {
                settings.angle_snapping = bimap_bool.GetEnum(arg_angle_snapping.getValue());
            } catch (std::invalid_argument e) {
                printf("Unknown angle snapping boolean value specified!\n");
                return -1;
            }
        }
    } catch (TCLAP::ArgException &e) {
        printf("Error: %s for arg %s\n", e.error().c_str(), e.argId().c_str());
        return -1;
    }

    if (!ProfileInterface::SendSettings(profile, settings)) {
        printf("Sending profile settings to device failed!\n");
        return -1;
    }
    return 0;
}
