#include <vector>
#include <time.h>

#ifndef TEXTURE_H
#define TEXTURE_H 1
#include "Texture.h"
#endif // TEXTURE_H

class Background
{
public:
    Background();
    ~Background();
    void addBackgroundFromFile(std::string );
    void display();

private:
    std::vector<Texture> background;
    int numberOfBackground;
    // to change background
    time_t mStartTime;
};
