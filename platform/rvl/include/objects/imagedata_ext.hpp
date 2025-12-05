#pragma once

#include <objects/imagedata/imagedata.tcc>

namespace love
{
    template<>
    class ImageData<Console::RVL> : public ImageData<Console::ALL>
    {
      public:
        using ImageData<Console::ALL>::ImageData;

        ImageData<Console::RVL>* Clone() const override
        {
            return new ImageData<Console::RVL>(*this);
        }
    };
} // namespace love
