#ifndef SDL_MIXER_H
#define SDL_MIXER_H
#include <SDL_mixer.h>
#endif // SDL_MIXER_H

#include <string>

class Audio
{
public:
    Audio();
    ~Audio();
    void free();
    bool loadFromFile(std::string path);
    void play();
    void playOnce();
private:
    Mix_Chunk* mSound;
    std::string path;
};
