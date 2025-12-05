#pragma once

#include <vector>
#include <utilities/sensor/sensorbase.hpp>
#include <wiiuse/wpad.h>

namespace love
{
    class Accelerometer : public SensorBase
    {
    public:
        Accelerometer()
        {
            data.assign(3, 0.f);
        }

        ~Accelerometer() {}

        void Update(const WPADData* input)
        {
            if (!input)
                return;

            data[0] = static_cast<float>(input->accel.x);
            data[1] = static_cast<float>(input->accel.y);
            data[2] = static_cast<float>(input->accel.z);
        }

        std::vector<float> GetData() override
        {
            return data;
        }

    private:
        std::vector<float> data;
    };
}
