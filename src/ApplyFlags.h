#pragma once

#include "System/EnumFlags.h"

enum class ApplyFlags : int {
    NONE           = 0,
    DONT_CALL_CTOR = 1,
};

ENUM_FLAGS( ApplyFlags )
