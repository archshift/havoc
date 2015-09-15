// Copyright 2015 archshift
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <algorithm>
#include <cinttypes>
#include <stdexcept>
#include <string>
#include <vector>

template <typename E>
class EnumStringBimap {
    typedef std::pair<std::string, E> Pair;
    std::vector<Pair> data;
public:
    EnumStringBimap() {}
    EnumStringBimap(std::initializer_list<Pair> init_list): data(init_list) {}

    std::string GetString(E e) const
    {
        auto it = std::find_if(std::begin(data), std::end(data), [&e](const Pair& pair) {
            return (pair.second == e);
        });
        if (it != std::end(data))
            return it->first;
        throw std::invalid_argument(e);
    }

    E GetEnum(const std::string& str) const
    {
        auto it = std::find_if(std::begin(data), std::end(data), [&str](const Pair& pair) {
            std::string found_str = pair.first;
            std::string const_str = str;
            std::transform(found_str.begin(), found_str.end(), found_str.begin(), ::tolower);
            std::transform(const_str.begin(), const_str.end(), const_str.begin(), ::tolower);
            return (found_str == const_str);
        });
        if (it != std::end(data))
            return it->second;
        throw std::invalid_argument(str);
    }
};

static const EnumStringBimap<bool> bimap_bool {
    {"true", true},
    {"on", true},
    {"1", true},
    {"false", false},
    {"off", false},
    {"0", false},
};

/** A configuration profile on the mouse. There are four profiles to choose from.
 */
enum class Profile : uint8_t {
    PROFILE_0 = 0,
    PROFILE_1 = 1,
    PROFILE_2 = 2,
    PROFILE_3 = 3,
};

static const EnumStringBimap<Profile> bimap_profile {
    {"0", Profile::PROFILE_0},
    {"1", Profile::PROFILE_1},
    {"2", Profile::PROFILE_2},
    {"3", Profile::PROFILE_3},
};

/** A DPI configuration for a mouse profile. There are four configurations to choose from.
 */
enum class DpiSetting : uint8_t {
    DPI_1 = 0,
    DPI_2 = 1,
    DPI_3 = 2,
    DPI_4 = 3,
};

static const EnumStringBimap<DpiSetting> bimap_dpi_setting {
    {"1", DpiSetting::DPI_1},
    {"2", DpiSetting::DPI_2},
    {"3", DpiSetting::DPI_3},
    {"4", DpiSetting::DPI_4},
};

/** The mode the mouse LEDs are in. Does not include the color or brightness of the LEDs.
 */
enum class LedMode : uint8_t {
    OFF = 0,
    ON = 1,
    BREATHING = 2,
    // 3 - Always blue, breathing
    RAPID_FIRE = 4,
};

static const EnumStringBimap<LedMode> bimap_led_mode {
    {"off", LedMode::OFF},
    {"on", LedMode::ON},
    {"breathing", LedMode::BREATHING},
    {"rapidfire", LedMode::RAPID_FIRE},
};

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

static const EnumStringBimap<LedBrightness> bimap_led_brightness {
    {"0", LedBrightness::PERCENT_0},
    {"20", LedBrightness::PERCENT_20},
    {"40", LedBrightness::PERCENT_40},
    {"60", LedBrightness::PERCENT_60},
    {"80", LedBrightness::PERCENT_80},
    {"100", LedBrightness::PERCENT_100},
};

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

static const EnumStringBimap<Color> bimap_color {
    {"red", Color::RED},
    {"green", Color::GREEN},
    {"yellow", Color::YELLOW},
    {"blue", Color::BLUE},
    {"magenta", Color::MAGENTA},
    {"teal", Color::TEAL},
    {"white", Color::WHITE},
};

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
