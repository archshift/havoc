#pragma once

struct ProfileSettings;

namespace HW {

bool Initialize();
void Deinitialize();

bool SendProfileSettings(const ProfileSettings& settings);

}