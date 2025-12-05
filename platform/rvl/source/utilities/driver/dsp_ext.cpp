#include <common/exception.hpp>

#include <utilities/driver/dsp_ext.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <aesndlib.h>

using namespace love;

static std::array<int, 24> channelOffsets;

static void SDL_ChnEffectEvent(int channel, void* stream, int length, void* userdata)
{
    channelOffsets[channel] += length;
}

DSP<Console::RVL>::DSP()
{}

void DSP<Console::RVL>::Initialize()
{
    SDL_InitSubSystem(SDL_INIT_AUDIO);

    this->initialized = Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096) == 0;

    if (!this->initialized)
        throw love::Exception("Failed to initialize DSP driver: (%s)!", Mix_GetError());

    Mix_AllocateChannels(24);

    AESND_Init();
}

DSP<Console::RVL>::~DSP()
{
    for (size_t channel = 0; channel < 24; channel++)
        Mix_UnregisterAllEffects(channel);

    AESND_Reset();
    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void DSP<Console::RVL>::Update()
{}

void DSP<Console::RVL>::SetMasterVolume(float volume)
{
    Mix_Volume(-1, (volume * 128));
}

float DSP<Console::RVL>::GetMasterVolume() const
{
    return Mix_Volume(-1, -1) / 128.0f;
}

bool DSP<Console::RVL>::ChannelReset(size_t id)
{
    this->ChannelSetVolume(id, this->ChannelGetVolume(id));
    channelOffsets[id] = 0;

    return true;
}

void DSP<Console::RVL>::ChannelSetVolume(size_t id, float volume)
{
    Mix_Volume(id, (volume * 128.0f));
}

float DSP<Console::RVL>::ChannelGetVolume(size_t id)
{
    return Mix_Volume(id, -1) / 128.0f;
}

size_t DSP<Console::RVL>::ChannelGetSampleOffset(size_t id, int bitDepth)
{
    return channelOffsets[id] / bitDepth * 8;
}

bool DSP<Console::RVL>::ChannelAddBuffer(size_t id, Mix_Chunk* buffer, bool looping)
{
    int loops = (looping) ? -1 : 0;

    Mix_RegisterEffect(id, SDL_ChnEffectEvent, nullptr, nullptr);
    if (Mix_PlayChannel(id, buffer, loops) < 0)
        return false;

    return true;
}

void DSP<Console::RVL>::ChannelPause(size_t id, bool paused)
{
    if (paused)
        Mix_Pause(id);
    else
        Mix_Resume(id);
}

bool DSP<Console::RVL>::IsChannelPaused(size_t id)
{
    return Mix_Paused(id);
}

bool DSP<Console::RVL>::IsChannelPlaying(size_t id)
{
    return Mix_Playing(id);
}

void DSP<Console::RVL>::ChannelStop(size_t id)
{
    Mix_HaltChannel(id);
}
