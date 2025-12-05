// HID_wii.hpp
#pragma once

#include <array>
#include <vector>
#include <cstring>

#include <ogc/pad.h>
#include <wiiuse/wpad.h>
#include <gccore.h>

#include <utilities/driver/hid.tcc>
#include <utilities/guid.hpp>

// include your Event declaration so std::array<Event,...> is valid
#include <modules/event/event.hpp>

namespace love
{
    template<>
    class HID<Console::RVL> : public HID<Console::ALL>
    {
      public:
        static HID& Instance()
        {
            static HID instance;
            return instance;
        }

        ~HID();

      private:
        HID();

        static constexpr int MAX_CONTROLLERS = 4;

        virtual void _Poll() override;

        void CheckFocus();

        std::array<PADStatus, MAX_CONTROLLERS> gcStatus;
        std::array<u16, MAX_CONTROLLERS> previousGcButtons;

        std::array<WPADData, MAX_CONTROLLERS> wpadData;
        std::array<u32, MAX_CONTROLLERS> previousWpadButtons;
        std::array<int, MAX_CONTROLLERS> wpadTypes;

        std::vector<guid::GamepadType> previousGamepadTypes;

        std::array<love::Event, MAX_CONTROLLERS> statusEvents;
    };
} // namespace love
