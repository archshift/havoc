#pragma once

#include "havoc.h"

namespace HW {

bool Initialize();
void Deinitialize();

bool SendProfileSettings(const Profile profile, const ProfileSettings& settings);

}