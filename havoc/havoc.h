#pragma once

enum class LedMode : uint8_t {
    OFF = 0,
    ON = 1,
    BREATHING = 2,
    // 3 - Always blue, breathing
    RAPID_FIRE = 4,
};

enum class Color : uint8_t {
    RED = 1,
    GREEN = 2,
    YELLOW = 3,
    BLUE = 4,
    MAGENTA = 5,
    TEAL = 6,
    WHITE = 7,
};

struct ProfileSettings {
    LedMode led_mode;
    Color color;
};