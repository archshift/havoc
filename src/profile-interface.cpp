// Copyright 2015 archshift
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "profile-interface.h"

#include <functional>

#include "hardware-interface.h"

uint16_t ProfileInterface::GetDataAddr(const Profile profile, uint16_t profile0_offset) {
    uint16_t setting_addr = profile0_offset + 0x54 * (uint8_t)profile;
    if (profile == Profile::PROFILE_3) {
        setting_addr += 4;
    }
    return setting_addr;
}

bool ProfileInterface::WriteData(const Profile profile, uint16_t profile0_offset, uint8_t data) {
    return HW::WriteEepromByte(GetDataAddr(profile, profile0_offset), data);
}

bool ProfileInterface::ReadData(const Profile profile, uint16_t profile0_offset, uint8_t* data_out) {
    return HW::ReadEepromByte(GetDataAddr(profile, profile0_offset), data_out);
}

bool ProfileInterface::ReceiveSettings(const Profile profile, ProfileSettings* settings) {
    struct { uint16_t offs; std::function<void(uint8_t)> cb; } mappings[] = {
        // High nibble: 0x8. Low nibble: active DPI configuration.
        { 0x100, [&](uint8_t byte){ settings->active_dpi = (DpiSetting)(byte & 0xF); } },

        // High nibble: LED mode. Low nibble: LED color.
        { 0x10B, [&](uint8_t byte){ settings->led_mode = (LedMode)(byte >> 4);
                                    settings->color = (Color)(byte & 0xF); } },

        // High nibble: ?. Low nibble: Angle snapping bool.
        { 0x146, [&](uint8_t byte){ settings->angle_snapping = (bool)byte; } },

        // Byte: LED brightness.
        { 0x14C, [&](uint8_t byte){ settings->led_brightness = (LedBrightness)byte; } },

        // Byte: Button responsiveness ratio
        { 0x150, [&](uint8_t byte){ settings->button_response = (float)byte / 0x80; } },
    };

    for (auto mapping : mappings) {
        uint8_t byte = 0;
        if (!ReadData(profile, mapping.offs, &byte)) {
            return false;
        }
        mapping.cb(byte);
    }

    return true;
}

bool ProfileInterface::SendSettings(const Profile profile, const ProfileSettings& settings) {
    struct { uint16_t offs; std::function<uint8_t()> cb; } mappings[] = {
        { 0x100, [&]{ return 0x80 | ((uint8_t)settings.active_dpi & 0xF); } },
        { 0x10B, [&]{ return ((uint8_t)settings.led_mode << 4) | ((uint8_t)settings.color & 0xF); } },
        { 0x10C, [&]{ return ((uint8_t)settings.led_mode << 4) | ((uint8_t)settings.color & 0xF); } },
        { 0x10D, [&]{ return ((uint8_t)settings.led_mode << 4) | ((uint8_t)settings.color & 0xF); } },
        { 0x146, [&]{ return 0xC0 | (settings.angle_snapping & 0xF); } },
        { 0x14C, [&]{ return (uint8_t)settings.led_brightness; } },
        { 0x14D, [&]{ return (uint8_t)settings.led_brightness; } },
        { 0x14E, [&]{ return (uint8_t)settings.led_brightness; } },
        { 0x150, [&]{ return settings.button_response * 0x80; } },
    };

    for (auto mapping : mappings) {
        if (!WriteData(profile, mapping.offs, mapping.cb())) {
            return false;
        }
    }
    return true;
}
