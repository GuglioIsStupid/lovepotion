#include <modules/graphics_ext.hpp>

#include <utilities/driver/renderer_ext.hpp>

#include <common/matrix.hpp>

#include <modules/fontmodule_ext.hpp>
#include <modules/window_ext.hpp>

#include <objects/texture_ext.hpp>
#include <objects/shader_ext.hpp>

using Renderer = love::Renderer<love::Console::RVL>;
using namespace love;

Graphics<Console::RVL>::Graphics()
{
    auto* window = Module::GetInstance<Window<Console::RVL>>(Module::M_WINDOW);

    if (window != nullptr)
    {
        window->SetGraphics(this);

        if (window->IsOpen())
            window->SetWindow();
    }
}

bool Graphics<Console::RVL>::SetMode(int x, int y, int width, int height)
{
    try
    {
        ::Renderer::Instance();
    }
    catch (love::Exception&)
    {
        throw;
    }

    this->RestoreState(this->states.back());

    this->SetViewportSize(width, height);

    for (int index = 0; index < Shader<>::STANDARD_MAX_ENUM; index++)
    {
        const auto type = (Shader<>::StandardShader)index;

        try
        {
            if (!Shader<Console::RVL>::defaults[index])
            {
                auto* shader = new Shader<Console::RVL>();
                shader->LoadDefaults(type);

                Shader<Console::RVL>::defaults[index] = shader;
            }
        }
        catch (love::Exception&)
        {
            throw;
        }
    }

    if (!Shader<Console::RVL>::current)
        Shader<Console::RVL>::defaults[Shader<>::STANDARD_DEFAULT]->Attach();

    this->created = true;
    return true;
}

Texture<Console::RVL>* Graphics<Console::RVL>::NewTexture(const Texture<>::Settings& settings,
                                                          const Texture<>::Slices* slices) const
{
    return new Texture<Console::RVL>(this, settings, slices);
}

void Graphics<Console::RVL>::Draw(Drawable* drawable, const Matrix4& matrix)
{
    drawable->Draw(*this, matrix);
}

void Graphics<Console::RVL>::Draw(Texture<Console::RVL>* texture, Quad* quad,
                                  const Matrix4& matrix)
{
    texture->Draw(*this, quad, matrix);
}

void Graphics<Console::RVL>::SetShader()
{
    Shader<Console::RVL>::AttachDefault(Shader<>::STANDARD_DEFAULT);
    this->states.back().shader.Set(nullptr);
}

void Graphics<Console::RVL>::SetShader(Shader<Console::RVL>* shader)
{
    if (shader == nullptr)
        return this->SetShader();

    shader->Attach();
    this->states.back().shader.Set(shader);
}

void Graphics<Console::RVL>::SetViewportSize(int width, int height)
{
    ::Renderer::Instance().SetViewport(Rect::EMPTY);
    this->SetScissor(Rect::EMPTY);
}
