#pragma once

#include <wiiuse/wpad.h>
#include <utilities/guid.hpp>

namespace love::wpad
{
    static constexpr size_t MAX_JOYSTICKS = 4;

    guid::GamepadType GetWPADType(WPADExtensionType extension);
}
