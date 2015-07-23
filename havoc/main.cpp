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

    if (!ProfileInterface::ReceiveSettings(Profile::PROFILE_0, &settings)) {
        printf("Receiving profile settings from device failed!\n");
    }

    TCLAP::CmdLine cmd("A configuration tool for the CMStorm Havoc mouse.", ' ', "0.1");

    TCLAP::ValueArg<std::string> arg_profile("p", "profile", "Profile to edit", true, "", "string", cmd);
    TCLAP::ValueArg<std::string> arg_color("c", "color", "LED color", false, "", "color", cmd);

    try {
        cmd.parse(argc, argv);

        try {
            profile = decode_profile.at(arg_profile.getValue());
        } catch (std::out_of_range e) {
            printf("Unknown profile specified!\n");
            goto err;
        }

        if (arg_color.isSet()) {
            try {
                settings.color = decode_color.at(arg_color.getValue());
            } catch (std::out_of_range e) {
                printf("Unknown color specified!\n");
                goto err;
            }
        }
    } catch (TCLAP::ArgException &e) {
        printf("Error: %s for arg %s\n", e.error().c_str(), e.argId().c_str());
        goto err;
    }

    if (!ProfileInterface::SendSettings(profile, settings)) {
        printf("Sending profile settings to device failed!\n");
    }

err:
    HW::Deinitialize();
    return 0;
}
