// Copyright 2015 archshift
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <algorithm>
#include <cinttypes>
#include <string>
#include <map>

static auto case_independent_sorter = [](const std::string &lhs, const std::string &rhs) {
    std::string lhs_ = lhs;
    std::string rhs_ = rhs;
    std::transform(lhs_.begin(), lhs_.end(), lhs_.begin(), ::tolower);
    std::transform(rhs_.begin(), rhs_.end(), rhs_.begin(), ::tolower);
    return std::less<std::string>()(lhs_, rhs_);
};
typedef decltype(case_independent_sorter) CaseIndependentSorter;

std::map<std::string, bool, CaseIndependentSorter> decode_bool({
    {"true", true},
    {"on", true},
    {"1", true},
    {"false", false},
    {"off", false},
    {"0", false},
}, case_independent_sorter);

/** A configuration profile on the mouse. There are four profiles to choose from.
 */
enum class Profile : uint8_t {
    PROFILE_0 = 0,
    PROFILE_1 = 1,
    PROFILE_2 = 2,
    PROFILE_3 = 3,
};

std::map<std::string, Profile, CaseIndependentSorter> decode_profile({
    {"0", Profile::PROFILE_0},
    {"1", Profile::PROFILE_1},
    {"2", Profile::PROFILE_2},
    {"3", Profile::PROFILE_3},
}, case_independent_sorter);

/** A DPI configuration for a mouse profile. There are four configurations to choose from.
 */
enum class DpiSetting : uint8_t {
    DPI_1 = 0,
    DPI_2 = 1,
    DPI_3 = 2,
    DPI_4 = 3,
};

std::map<std::string, DpiSetting, CaseIndependentSorter> decode_dpi_setting({
    {"1", DpiSetting::DPI_1},
    {"2", DpiSetting::DPI_2},
    {"3", DpiSetting::DPI_3},
    {"4", DpiSetting::DPI_4},
}, case_independent_sorter);

/** The mode the mouse LEDs are in. Does not include the color or brightness of the LEDs.
 */
enum class LedMode : uint8_t {
    OFF = 0,
    ON = 1,
    BREATHING = 2,
    // 3 - Always blue, breathing
    RAPID_FIRE = 4,
};

std::map<std::string, LedMode, CaseIndependentSorter> decode_led_mode({
    {"off", LedMode::OFF},
    {"on", LedMode::ON},
    {"breathing", LedMode::BREATHING},
    {"rapidfire", LedMode::RAPID_FIRE},
}, case_independent_sorter);

/** The brightness of the mouse LEDs. Providing values between the enumerated percentages is unreliable.
 */
enum class LedBrightness : uint8_t {
    PERCENT_0 = 0x0,
    PERCENT_20 = 0x1,
    PERCENT_40 = 0x11,
    PERCENT_60 = 0x25,
    PERCENT_80 = 0x6C,
    PERCENT_100 = 0xFF,
};

std::map<std::string, LedBrightness, CaseIndependentSorter> decode_led_brightness({
    {"0", LedBrightness::PERCENT_0},
    {"20", LedBrightness::PERCENT_20},
    {"40", LedBrightness::PERCENT_40},
    {"60", LedBrightness::PERCENT_60},
    {"80", LedBrightness::PERCENT_80},
    {"100", LedBrightness::PERCENT_100},
}, case_independent_sorter);

/** The color of the mouse LEDs.
 */
enum class Color : uint8_t {
    RED = 1,
    GREEN = 2,
    YELLOW = 3,
    BLUE = 4,
    MAGENTA = 5,
    TEAL = 6,
    WHITE = 7,
};

std::map<std::string, Color, CaseIndependentSorter> decode_color({
    {"red", Color::RED},
    {"green", Color::GREEN},
    {"yellow", Color::YELLOW},
    {"blue", Color::BLUE},
    {"magenta", Color::MAGENTA},
    {"teal", Color::TEAL},
    {"white", Color::WHITE},
}, case_independent_sorter);

/**
 * @throw std::invalid_argument
 */
inline float DecodeButtonResponse(const std::string& str) {
    float f  = std::stof(str);
    if (f >= 0 && f <= 1) {
        return f;
    }
    throw std::invalid_argument(str);
}

struct ProfileSettings {
    DpiSetting active_dpi;

    LedMode led_mode;
    LedBrightness led_brightness;
    Color color;

    /**
     * This variable should be set between 0 and 1. This corresponds to between 250us and 32ms.
     * This is converted to between 0x0 and 0x80 to be sent to the hardware.
     */
    float button_response;
    bool angle_snapping;
};
