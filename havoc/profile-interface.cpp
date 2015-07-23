#include "profile-interface.h"

#include "hardware-interface.h"

uint64_t ProfileInterface::GetDataAddr(const Profile profile, uint16_t profile0_offset) {
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
    uint8_t active_dpi_byte = 0;  // High nibble: 0x8. Low nibble: active DPI configuration.
    if (!ReadData(profile, 0x100, &active_dpi_byte)) {
        return false;
    }
    settings->active_dpi = (DpiSetting)(active_dpi_byte & 0xF);

    uint8_t led_mode_color = 0;  // High nibble: LED mode. Low nibble: LED color.
    if (!ReadData(profile, 0x10B, &led_mode_color)) {
        return false;
    }
    settings->led_mode = (LedMode)(led_mode_color >> 4);
    settings->color = (Color)(led_mode_color & 0xF);

    uint8_t angle_snapping = 0;  // High nibble: ?. Low nibble: Angle snapping bool.
    if (!ReadData(profile, 0x146, &angle_snapping)) {
        return false;
    }
    settings->angle_snapping = (bool)angle_snapping;

    uint8_t led_brightness = 0;  // Byte: LED brightness.
    if (!ReadData(profile, 0x14C, &led_brightness)) {
        return false;
    }
    settings->led_brightness = (LedBrightness)led_brightness;

    uint8_t button_response = 0;  // Byte: Button responsiveness ratio
    if (!ReadData(profile, 0x150, &button_response)) {
        return false;
    }
    settings->button_response = (float)button_response / 0x80;

    return true;
}

bool ProfileInterface::SendSettings(const Profile profile, const ProfileSettings& settings) {
    if (!WriteData(profile, 0x100, 0x80 | ((uint8_t)settings.active_dpi & 0xF))) {
        return false;
    }
    for (int i = 0; i < 3; i++) {
        if (!WriteData(profile, 0x10B + i, ((uint8_t)settings.led_mode << 4) | ((uint8_t)settings.color & 0xF))) {
            return false;
        }
    }
    if (!WriteData(profile, 0x146, 0xC0 | (settings.angle_snapping & 0xF))) {
        return false;
    }
    for (int i = 0; i < 3; i++) {
        if (!WriteData(profile, 0x14C + i, (uint8_t)settings.led_brightness)) {
            return false;
        }
    }
    if (!WriteData(profile, 0x150, settings.button_response * 0x80)) {
        return false;
    }
    return true;
}