#ifndef BASIC_LIB
#define BASIC_LIB
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#endif // BASIC_LIB

#include "Animation.h"
#ifndef HEADER_H
#define HEADER_H 1
#include "Headers.h"
#endif // HEADER_H


Animation::Animation()
{
    mTexture = NULL;
    mDestX = 0;
    mDestY = 0;
    mX = 0;
    mY = 0;
    mVelX = 0;
    mVelY = 0;
    mAccX = 0;
    mAccY = 0;
    mStatus = STANDING_STILL;
}

Animation::~Animation()
{
    mTexture = NULL;
}

bool Animation::setTexture(Texture* lTexture)
{
    bool success = false;
    if(lTexture != NULL)
    {
        mTexture = lTexture;
        success = true;
    }
    else
    {
        printf("Warning: pass a NULL pointer to Animation::setTexture!\n");
        success = false;
    }
    return success;
}

void Animation::render(SDL_Renderer* lRenderer)
{
    mTexture->render(lRenderer, mX, mY);
}

void Animation::setDest(int destX, int destY)
{
    mDestX = destX;
    mDestY = destY;
}

void Animation::setPosition(int lX, int lY)
{
    mX = lX;
    mY = lY;
}

void Animation::fall(SDL_Renderer* lRenderer)
{
    // not going to fall to anywhere
    if(mDestX < mX || mDestY < mY)
        return;
    // set accelerator to gravitial acc
    if(mAccY != FALLING_ACCELERATOR)
    {
        mAccY = FALLING_ACCELERATOR;
    }
    // not moving vertically
    if(mAccX != 0)
    {
        mAccX = 0;
    }
    mStatus = FALLING;
    mVelY += mAccY;
    mVelX += mAccX;
    mX = std::min(mDestX, mX + mVelX);
    mY = std::min(mDestY, mY + mVelY);
    if(mY >= mDestY)
    {
        stop();
    }
    // why need this? because some candy might be falling from outside of the board
    if(mX >= 0 && mY >= 0)
    {
        render(lRenderer);
    }
}

void Animation::walk(SDL_Renderer* lRenderer)
{
    // not going to fall to anywhere
    if(mDestX < mX || mDestY < mY)
        return;
    // set accelerator to gravitial acc although its horizontal
    if(mAccX != FALLING_ACCELERATOR)
    {
        mAccX = FALLING_ACCELERATOR;
    }
    // not moving vertically
    if(mAccY != 0)
    {
        mAccY = 0;
    }
    mStatus = WALKING;
    mVelY += mAccY;
    mVelX += mAccX;
    mX = std::min(mDestX, mX + mVelX);
    mY = std::min(mDestY, mY + mVelY);
    if(mY >= mDestY)
    {
        stop();
    }
    // why need this? because some candy might be falling from outside of the board
    if(mX >= 0 && mY >= 0)
    {
        render(lRenderer);
    }
}

void Animation::stop()
{
    mStatus = STANDING_STILL;
    mAccX = 0;
    mAccY = 0;
    mVelX = 0;
    mVelY = 0;
}

STATUS Animation::getStatus()
{
    return mStatus;
}

int Animation::getX()
{
    return mX;
}

int Animation::getY()
{
    return mY;
}

int Animation::getVelX()
{
    return mVelX;
}

int Animation::getVelY()
{
    return mVelY;
}
