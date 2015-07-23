#pragma once

#include "havoc.h"

namespace HW {

bool Initialize();
void Deinitialize();

bool SendProfileSettings(const ProfileSettings& settings);

}