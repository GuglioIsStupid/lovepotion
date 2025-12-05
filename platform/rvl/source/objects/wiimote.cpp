#include <objects/wiimote.hpp>
#include <utilities/bidirectionalmap/bidirectionalmap.hpp>
#include <modules/joystickmodule_ext.hpp>
#include <modules/timer_ext.hpp>
#include <utilities/wpad.hpp>

using namespace love;

#define Module() (Module::GetInstance<JoystickModule<Console::CAFE>>(Module::M_JOYSTICK))

// clang-format off
constexpr BidirectionalMap buttons = {
    Joystick<>::GAMEPAD_BUTTON_A,             WPAD_BUTTON_A,
    Joystick<>::GAMEPAD_BUTTON_B,             WPAD_BUTTON_B,
    Joystick<>::GAMEPAD_BUTTON_X,             WPAD_BUTTON_X,
    Joystick<>::GAMEPAD_BUTTON_Y,             WPAD_BUTTON_Y,
    Joystick<>::GAMEPAD_BUTTON_BACK,          WPAD_BUTTON_MINUS,
    Joystick<>::GAMEPAD_BUTTON_GUIDE,         WPAD_BUTTON_HOME,
    Joystick<>::GAMEPAD_BUTTON_START,         WPAD_BUTTON_PLUS,
    Joystick<>::GAMEPAD_BUTTON_LEFTSHOULDER,  WPAD_BUTTON_L,
    Joystick<>::GAMEPAD_BUTTON_RIGHTSHOULDER, WPAD_BUTTON_R,
    Joystick<>::GAMEPAD_BUTTON_DPAD_UP,       WPAD_BUTTON_UP,
    Joystick<>::GAMEPAD_BUTTON_DPAD_DOWN,     WPAD_BUTTON_DOWN,
    Joystick<>::GAMEPAD_BUTTON_DPAD_LEFT,     WPAD_BUTTON_LEFT,
    Joystick<>::GAMEPAD_BUTTON_DPAD_RIGHT,    WPAD_BUTTON_RIGHT
};

constexpr BidirectionalMap axes = {
    Joystick<>::GAMEPAD_AXIS_LEFTX,        WPAD_STICK_L_EMULATION_LEFT | WPAD_STICK_L_EMULATION_RIGHT,
    Joystick<>::GAMEPAD_AXIS_LEFTY,        WPAD_STICK_L_EMULATION_UP   | WPAD_STICK_L_EMULATION_DOWN,
    Joystick<>::GAMEPAD_AXIS_RIGHTX,       WPAD_STICK_R_EMULATION_LEFT | WPAD_STICK_R_EMULATION_RIGHT,
    Joystick<>::GAMEPAD_AXIS_RIGHTY,       WPAD_STICK_R_EMULATION_UP   | WPAD_STICK_R_EMULATION_DOWN,
    Joystick<>::GAMEPAD_AXIS_TRIGGERLEFT,  WPAD_BUTTON_ZL,
    Joystick<>::GAMEPAD_AXIS_TRIGGERRIGHT, WPAD_BUTTON_ZR
};
// clang-format on

Wiimote::Wiimote(int id) : state {}, buttonStates {}
{
    this->id = id;
    this->instanceId = -1;
}

Wiimote::Wiimote(int id, int index) : Wiimote(id)
{
    this->Open(index);
}

Wiimote::~Wiimote()
{
    this->Close();
}

bool Wiimote::Open(int index)
{
    this->Close();

    this->instanceId = index;

    this->name = guid::GetGamepadName(this->GetGamepadType());
    this->guid = guid::GetGamepadGUID(this->GetGamepadType());

    return this->IsConnected();
}

void Wiimote::Close()
{
    this->instanceId = -1;
    this->vibration.SendValues(0, 0);
}

bool Wiimote::IsConnected() const
{
    return WPAD_Probe(this->instanceId, nullptr) == WPAD_READ_OK;
}

void Wiimote::Update()
{
    WPADReadError error {};
    WPADRead(this->instanceId, &this->state, 1, &error);

    if (error == WPAD_READ_NO_SAMPLES)
        return;

    this->buttonStates.pressed  = this->state.trigger;
    this->buttonStates.released = this->state.release;
    this->buttonStates.held     = this->state.hold;

    this->leftStick  = { this->state.leftStick.x, this->state.leftStick.y };
    this->rightStick = { this->state.rightStick.x, this->state.rightStick.y };
}

bool Wiimote::IsDown(JoystickInput& result)
{
    if (!this->IsConnected() || !this->buttonStates.pressed)
        return false;

    for (auto& entry : buttons.GetEntries())
    {
        if ((int)entry.second == -1) continue;

        if (entry.second & this->buttonStates.pressed)
        {
            this->buttonStates.pressed ^= entry.second;
            result = { .type = InputType::INPUT_TYPE_BUTTON,
                       .button = entry.first,
                       .buttonNumber = 0 };
            return true;
        }
    }

    return false;
}

float Wiimote::GetAxis(int index)
{
    if (!this->IsConnected())
        return 0.0f;

    if (index == 0) return this->leftStick.dx;
    if (index == 1) return this->leftStick.dy;
    if (index == 2) return this->rightStick.dx;
    if (index == 3) return this->rightStick.dy;
    if (index == 4) return (this->buttonStates.held & WPAD_BUTTON_ZL) ? 1.0f : 0.0f;
    if (index == 5) return (this->buttonStates.held & WPAD_BUTTON_ZR) ? 1.0f : 0.0f;

    return 0.0f;
}

std::vector<float> Wiimote::GetAxes()
{
    return { this->leftStick.dx, this->leftStick.dy, this->rightStick.dx, this->rightStick.dy,
             (this->buttonStates.held & WPAD_BUTTON_ZL) ? 1.0f : 0.0f,
             (this->buttonStates.held & WPAD_BUTTON_ZR) ? 1.0f : 0.0f };
}

guid::GamepadType Wiimote::GetGamepadType() const
{
    return guid::GAMEPAD_TYPE_WIIMOTE;
}

bool Wiimote::IsVibrationSupported()
{
    return true;
}

bool Wiimote::SetVibration(float left, float right, float duration)
{
    left  = std::clamp(left, 0.0f, 1.0f);
    right = std::clamp(right, 0.0f, 1.0f);

    if (!this->IsConnected())
        return false;

    bool success = this->vibration.SendValues(left, left);

    if (success)
        Module()->AddVibration(&this->vibration);

    return success;
}

bool Wiimote::SetVibration()
{
    return this->vibration.SendValues(0, 0);
}

void Wiimote::GetVibration(float& left, float& right)
{
    this->vibration.GetValues(left, right);
}

bool Wiimote::HasSensor(Sensor::SensorType type) const
{
    return type == Sensor::SENSOR_ACCELEROMETER;
}

bool Wiimote::IsSensorEnabled(Sensor::SensorType type)
{
    return this->sensors[type] != nullptr;
}

void Wiimote::SetSensorEnabled(Sensor::SensorType type, bool enabled)
{
    if (enabled && type == Sensor::SENSOR_ACCELEROMETER && !this->sensors[type])
    {
        this->sensors[type] = new Accelerometer();
        this->sensors[type]->SetEnabled(true);
    }
    else if (!enabled && this->sensors[type])
    {
        delete this->sensors[type];
        this->sensors[type] = nullptr;
    }
}

std::vector<float> Wiimote::GetSensorData(Sensor::SensorType type)
{
    if (!this->IsSensorEnabled(type))
        return {};

    return this->sensors[type]->GetData();
}

WPADStatus Wiimote::GetWPADStatus() const
{
    return this->state;
}
