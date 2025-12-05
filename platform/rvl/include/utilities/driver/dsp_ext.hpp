#pragma once

#include <utilities/bidirectionalmap/bidirectionalmap.hpp>

#include <utilities/driver/dsp.tcc>

#include <SDL2/SDL_mixer.h>

namespace love
{
    template<>
    class DSP<Console::RVL> : public DSP<Console::ALL>
    {
      public:
        static DSP& Instance()
        {
            static DSP instance;
            return instance;
        }

        DSP();

        ~DSP();

        void Initialize();

        void Update();

        void SetMasterVolume(float volume);

        float GetMasterVolume() const;

        bool ChannelReset(size_t id);

        void ChannelSetVolume(size_t id, float volume);

        float ChannelGetVolume(size_t id);

        size_t ChannelGetSampleOffset(size_t id, int bitDepth);

        bool ChannelAddBuffer(size_t id, Mix_Chunk* buffer, bool looping);

        void ChannelPause(size_t id, bool paused = true);

        bool IsChannelPaused(size_t id);

        bool IsChannelPlaying(size_t id);

        void ChannelStop(size_t id);

    };
} // namespace love
