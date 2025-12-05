#pragma once

#include <vector>
#include <utilities/sensor/sensorbase.hpp>
#include <wiiuse/wpad.h>

namespace love
{
    class Gyroscope : public SensorBase
    {
    public:
        Gyroscope()
        {
            data.assign(3, 0.f);
        }

        ~Gyroscope() {}

        void Update(const WPADData* input)
        {
            if (!input)
                return;

            if (input->exp.type == EXP_MOTION_PLUS)
            {
                data[0] = static_cast<float>(input->exp.mp.rx);
                data[1] = static_cast<float>(input->exp.mp.ry);
                data[2] = static_cast<float>(input->exp.mp.rz);
            }
        }

        std::vector<float> GetData() override
        {
            return data;
        }

    private:
        std::vector<float> data;
    };
}
