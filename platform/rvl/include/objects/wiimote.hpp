#pragma once

#include <objects/joystick_ext.hpp>
#include <utilities/wpad.hpp>

namespace love
{
    class Wiimote : public Joystick<Console::RVL>
    {
      public:
        Wiimote() = default;

        Wiimote(int id);

        Wiimote(int id, int index);

        virtual ~Wiimote() = default;

        bool Open(int index) override;

        void Close() override;

        bool IsConnected() const override;

        bool IsDown(JoystickInput& result) override;

        bool IsDown(const std::vector<int>& buttons) const override;

        bool IsGamepadDown(const std::vector<GamepadButton>& buttons) const override;

        bool IsAxisChanged(GamepadAxis axis) override;

        bool IsUp(JoystickInput& result) override;

        float GetAxis(int index) override;

        float GetGamepadAxis(GamepadAxis axis) override;

        std::vector<float> GetAxes() override;

        void Update() override;

        bool IsGamepad() const
        {
            return true;
        }

        guid::GamepadType GetGamepadType() const override
        {
            return guid::GAMEPAD_TYPE_WIIMOTE;
        }

        bool IsVibrationSupported() override
        {
            return true;
        }

        bool SetVibration(float left, float right, float duration = -1.0f) override;

        bool SetVibration() override;

        void GetVibration(float& left, float& right) override;

        bool HasSensor(Sensor::SensorType type) const override;

        bool IsSensorEnabled(Sensor::SensorType type) override;

        void SetSensorEnabled(Sensor::SensorType type, bool enabled) override;

        std::vector<float> GetSensorData(Sensor::SensorType type) override;

      private:
        WPADData state;
        Vibration<Console::RVL> vibration;
    };
} // namespace love
