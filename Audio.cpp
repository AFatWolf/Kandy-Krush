#include <stdio.h>

#ifndef AUDIO_H
#define AUDIO_H 1
#include "Audio.h"
#endif // AUDIO_H

Audio::Audio()
{
    mSound = NULL;
}

Audio::~Audio()
{
    free();
}

void Audio::free()
{
    if(mSound != NULL)
        Mix_FreeChunk(mSound);
}

bool Audio::loadFromFile(std::string path)
{
    bool success = true;
    mSound = Mix_LoadWAV(path.c_str());
    this->path = path;
    if(mSound == NULL)
    {
        printf("Unable to load file %s! Mix Error: %s\n", path.c_str(), Mix_GetError());
        success = false;
    }
    return success;
}

void Audio::play()
{
    int cnt = 0;
    if(mSound != NULL)
        // loop until there is available channels
        while(Mix_PlayChannel(-1, mSound, 0) == -1 && cnt < 500)
        {
            printf("Warning file: %s - Mix error: %s\n",path.c_str(), Mix_GetError());
            cnt ++;
        };
}

void Audio::playOnce()
{
    if(mSound != NULL)
    {
        Mix_PlayChannel(-1, mSound, 0);
    }
}
