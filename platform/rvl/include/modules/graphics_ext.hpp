#pragma once

#include <modules/graphics/graphics.tcc>

namespace love
{
    template<>
    class Graphics<Console::RVL> : public Graphics<Console::ALL>
    {
      public:
        Graphics();

        bool SetMode(int x, int y, int width, int height);

        Texture<Console::RVL>* NewTexture(const Texture<>::Settings& settings,
                                          const Texture<>::Slices* slices = nullptr) const;

        void Draw(Texture<Console::RVL>* texture, Quad* quad, const Matrix4& matrix);

        void Draw(Drawable* drawable, const Matrix4& matrix);

        void SetViewportSize(int width, int height);

        void SetShader();

        void SetShader(Shader<Console::RVL>* shader);
    };
} // namespace love
