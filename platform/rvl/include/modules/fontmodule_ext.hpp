#pragma once

#include <modules/font/fontmodule.tcc>
#include <objects/data/filedata/filedata.hpp>
#include <utilities/bidirectionalmap/bidirectionalmap.hpp>

namespace love
{
    using SystemFontType = std::string;

    class SystemFont : public Data
    {
      public:
        SystemFont(const std::string& path) : size(0), data(nullptr), path(path)
        {
            loadFromFile(path.c_str());
        }

        SystemFont(const char* path) : size(0), data(nullptr), path(path)
        {
            loadFromFile(path);
        }

        SystemFont(int type) : size(0), data(nullptr)
        {
            std::string path;
            switch(type)
            {
                case 0: path = "sd:/fonts/standard.ttf"; break;
                case 1: path = "sd:/fonts/chinese.ttf"; break;
                case 2: path = "sd:/fonts/korean.ttf"; break;
                case 3: path = "sd:/fonts/taiwanese.ttf"; break;
                default: path = "sd:/fonts/standard.ttf"; break;
            }
            this->path = path;
            loadFromFile(path.c_str());
        }

        ~SystemFont()
        {
            if (data)
                free(data);
        }

        SystemFont* Clone() const override
        {
            return new SystemFont(this->path);
        }

        void* GetData() const override
        {
            return data;
        }

        size_t GetSize() const override
        {
            return size;
        }

      private:
        void* data;
        size_t size;
        std::string path;

        void loadFromFile(const char* path)
        {
            FILE* file = fopen(path, "rb");
            if (!file) return;

            fseek(file, 0, SEEK_END);
            size = ftell(file);
            fseek(file, 0, SEEK_SET);

            data = malloc(size);
            if (data)
                fread(data, 1, size, file);

            fclose(file);
        }
    };

    template<>
    class FontModule<Console::RVL> : public FontModule<Console::ALL>
    {
      public:
        FontModule() {}

        virtual ~FontModule() {}

        using FontModule<Console::ALL>::NewTrueTypeRasterizer;

        Rasterizer* NewImageRasterizer(ImageData<Console::Which>* data, const std::string& text,
                                       int extraSpacing, float dpiScale) const override;

        Rasterizer* NewImageRasterizer(ImageData<Console::Which>* data, uint32_t* glyphs,
                                       int glyphCount, int extraSpacing,
                                       float dpiScale) const override;

        Rasterizer* NewTrueTypeRasterizer(Data* data, int size,
                                          TrueTypeRasterizer<>::Hinting hinting) const override;

        Rasterizer* NewTrueTypeRasterizer(Data* data, int size, float dpiScale,
                                          TrueTypeRasterizer<>::Hinting hinting) const override;

        Rasterizer* NewRasterizer(FileData* data) const;

        // clang-format off
        static constexpr BidirectionalMap systemFonts = {
            "standard",  "sd:/fonts/standard.ttf",
            "chinese",   "sd:/fonts/chinese.ttf",
            "korean",    "sd:/fonts/korean.ttf",
            "taiwanese", "sd:/fonts/taiwanese.ttf"
        };
        // clang-format on

      private:
        FT_Library library;
    };
} // namespace love
