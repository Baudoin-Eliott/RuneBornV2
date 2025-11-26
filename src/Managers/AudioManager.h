#pragma once
#include <SDL2/SDL_mixer.h>
#include <string>
#include <unordered_map>
#include <iostream>
#include <random>

class AudioManager
{
private:
    AudioManager() = default;
    AudioManager(const AudioManager &) = delete;
    AudioManager &operator=(const AudioManager &) = delete;

    std::unordered_map<std::string, Mix_Music *> musics;
    std::unordered_map<std::string, std::vector<Mix_Chunk *>> sounds;

    int musicVolume = 5;
    int soundVolume = 10;

    bool initialized = false;

public:
    static AudioManager &getInstance();

    bool init();

    void clean();

    bool loadMusic(const std::string &id, const std::string &filePath);

    bool loadSound(const std::string &id, const std::vector<std::string> &filesPaths);

    bool hasLoadMusic(const std::string &id) const { return musics.find(id) != musics.end(); }

    bool hasLoadSound(const std::string &id) const { return sounds.find(id) != sounds.end(); }

    void playMusic(const std::string &id, int fadeMs = 0);

    void stopMusic(int fadeMs = 0)
    {
        if (fadeMs > 0)
            Mix_FadeOutMusic(fadeMs);
        else
            Mix_HaltMusic();
    }

    void pauseMusic() { Mix_PauseMusic(); }

    void resumerMusic() { Mix_ResumeMusic(); }

    int playSound(const std::string &id, int loops = 0, int volume = -1);

    void setMusicVolume(int volume) { musicVolume = SDL_clamp(volume, 0, 128); }

    void setSoundVolume(int volume) { soundVolume = SDL_clamp(volume, 0, 128); }

    int getMusicVolume() const { return musicVolume; }

    int getSoundVolume() const { return soundVolume; }
};