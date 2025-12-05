#pragma once

#include <objects/joystick/joystick.tcc>
#include <wiiuse/wpad.h>

namespace love
{
    template<>
    class Joystick<Console::RVL> : public Joystick<Console::ALL>
    {
      public:
        Joystick()
        {}

        Joystick(int id) : buttonStates{}, connected(false)
        {
            this->instanceId = -1;
            this->id         = id;
        }

        virtual ~Joystick()
        {
            this->Close();
        }

        virtual bool Open(int index) = 0;

        void Close()
        {
            this->instanceId = -1;
            this->connected  = false;
        }

        bool IsConnected() const
        {
            return this->connected;
        }

        bool IsGamepad() const
        {
            return true;
        }

        virtual bool IsDown(JoystickInput& result) = 0;

        virtual bool IsDown(const std::vector<int>& buttons) const = 0;

        virtual bool IsGamepadDown(const std::vector<GamepadButton>& buttons) const = 0;

        virtual bool IsUp(JoystickInput& result) = 0;

        virtual bool IsAxisChanged(GamepadAxis axis) = 0;

        virtual float GetAxis(int index) = 0;

        virtual float GetGamepadAxis(GamepadAxis axis) = 0;

        virtual std::vector<float> GetAxes() = 0;

        virtual void Update() = 0;

        virtual guid::GamepadType GetGamepadType() const = 0;

        int GetAxisCount() const
        {
            if (!this->connected)
                return 0;

            return guid::GetGamepadAxisCount(this->GetGamepadType());
        }

        int GetButtonCount() const
        {
            if (!this->connected)
                return 0;

            return guid::GetGamepadButtonCount(this->GetGamepadType());
        }

        void SetPlayerIndex(int index)
        {}

        int GetPlayerIndex() const
        {
            return this->id;
        }

        virtual bool IsVibrationSupported() = 0;

        virtual bool SetVibration(float left, float right, float duration = -1.0f) = 0;

        virtual bool SetVibration() = 0;

        virtual void GetVibration(float& left, float& right) = 0;

        virtual bool HasSensor(Sensor::SensorType type) const = 0;

        virtual bool IsSensorEnabled(Sensor::SensorType type) = 0;

        virtual void SetSensorEnabled(Sensor::SensorType type, bool enabled) = 0;

        virtual std::vector<float> GetSensorData(Sensor::SensorType type) = 0;

    protected:
        struct Stick
        {
            float dx;
            float dy;
        };

        struct Trigger
        {
            bool down;
        };

        struct ButtonStates
        {
            int32_t pressed;
            int32_t released;
            int32_t held;
        } buttonStates;

        Stick leftStick;
        Stick rightStick;

        Trigger triggers[2];

        bool connected;

        WPADData wpadState;
    };
} // namespace love
