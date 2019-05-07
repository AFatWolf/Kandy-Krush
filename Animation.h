#ifndef TEXTURE_H
#define TEXTURE_H 1
#include "Texture.h"
#endif // TEXTURE_H

enum STATUS
{
    FALLING,
    WALKING,
    STANDING_STILL
};

class Animation
{
public:
    Animation();
    bool setTexture(Texture*);
    void render(SDL_Renderer*);
    void setDest(int,int);
    void setPosition(int, int);
    void fall(SDL_Renderer*);
    void walk(SDL_Renderer*);
    void stop();
    STATUS getStatus();
    int getX();
    int getY();
    int getVelX();
    int getVelY();
    ~Animation();

private:

    Texture* mTexture;
    STATUS mStatus;
    // where the texture is heading to
    int mDestX, mDestY;
    // current x and y of the texture
    int mX, mY;
    // velocity
    int mVelX, mVelY;
    //accelerator
    int mAccX, mAccY;
};
