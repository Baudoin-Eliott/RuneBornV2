#include "AudioManager.h"

AudioManager &AudioManager::getInstance()
{
    static AudioManager instance;
    return instance;
}

bool AudioManager::init()
{

    int flags = MIX_INIT_OGG;
    int initted = Mix_Init(flags);

    if ((initted & flags) != flags)
    {
        std::cerr << "[AudioManager] Mix_Init failed: " << Mix_GetError() << "\n";
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cerr << "[AudioManager] Mix_OpenAudio failed: " << Mix_GetError() << "\n";
        return false;
    }

    Mix_AllocateChannels(16);

    initialized = true;
    return true;
}

void AudioManager::clean()
{
    for (auto &pair : musics)
    {
        if (pair.second)
        {
            Mix_FreeMusic(pair.second);
        }
    }
    musics.clear();

    for (auto &pair : sounds)
    {

        for (auto &sound : pair.second)
        {
            if (sound)
                Mix_FreeChunk(sound);
        }
    }

    sounds.clear();

    if (initialized)
    {
        Mix_CloseAudio();
        Mix_Quit();
        initialized = false;
        std::cout << "[AudioManager] clean up\n";
    }
}

bool AudioManager::loadMusic(const std::string &id, const std::string &filePath)
{
    if (musics.find(id) != musics.end())
    {
        std::cerr << "[AudioManager] trying to load a music already load !\n";
        return false;
    }

    std::string fullPath = "assets/Audio/" + filePath;
    Mix_Music *music = Mix_LoadMUS(fullPath.c_str());
    if (!music)
    {
        std::cerr << "[AudioManager] failed to load music at: " << fullPath << "\n";
        return false;
    }
    musics[id] = music;
    std::cout << "[AudioManager] successfully loaded: " << fullPath << " now referended at id: " << id << "\n";
    return true;
}

bool AudioManager::loadSound(const std::string &id, const std::vector<std::string> &filesPaths)
{

    if (sounds.find(id) != sounds.end())
    {
        std::cerr << "[AudioManager] trying to load a sounds already load !\n";
        return false;
    }
    for (auto &filePath : filesPaths)
    {
        std::string fullPath = "assets/Audio/" + filePath;
        Mix_Chunk *sound = Mix_LoadWAV(fullPath.c_str());

        if (!sound)
        {

            std::cerr << "[AudioManager] failed to load sound at: " << fullPath << "\n";
            return false;
        }

        sounds[id].push_back(sound);
    }

    return true;
}

void AudioManager::playMusic(const std::string &id, int fadeMs)
{
    if (musics.find(id) == musics.end())
    {
        std::cerr << "[AudioManager] Warning ! music: " << id << " not loaded ! try loadMusic befors !\n";
        return;
    }

    Mix_Music *music = musics[id];

    if (fadeMs > 0)
        Mix_FadeInMusic(music, -1, fadeMs);
    else
        Mix_PlayMusic(music, -1);

    Mix_VolumeMusic(musicVolume);
}

int AudioManager::playSound(const std::string &id, int loops, int volume)
{
    if (sounds.find(id) == sounds.end())
    {
        std::cerr << "[AudioManager] Warning ! sound not loaded ! try loadSound first !\n";
        return -1;
    }

    const std::vector<Mix_Chunk *> &thisSounds = sounds[id];

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, thisSounds.size() - 1);

    Mix_Chunk *sound = thisSounds[dis(gen)];
    Mix_VolumeChunk(sound, volume != -1 ? volume : soundVolume);

    int channel = Mix_PlayChannel(-1, sound, loops);

    if (channel == -1)
    {
        std::cerr << "[AudioManager] no channel available, try later or increase channel numbers\n";
        return -1;
    }

    return channel;
}
