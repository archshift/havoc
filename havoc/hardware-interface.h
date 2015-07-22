#pragma once

struct libusb_device_handle;

struct ProfileSettings;

bool SendProfileSettings(libusb_device_handle* device_handle, const ProfileSettings& settings);