#ifndef BASIC_LIB
#define BASIC_LIB
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#endif // BASIC_LIB
#include "Candy.h"
#include "Headers.h"

int Candy::CandyWidth = 69;
int Candy::CandyHeight = 69;


Candy::Candy()
{
    type = EMPTY;
    // just to get the default width and height, doesn't matter if it's blue candy or not
    mTexture = &mCandyTexture[type];
    mWidth = mTexture->getWidth();
    mHeight = mTexture->getWidth();
    mX = 0;
    mY = 0;
    mIndex = 0;
    is2ndWrapped = false;
}
Candy::~Candy()
{
    empty();
}

void Candy::empty()
{
    setBreed(EMPTY);
}

bool Candy::equal(Candy* other)
{
    // 4 is the number of type of candy of 1 color
    // -1 because the first one is EMPTY
    if(this->getBreed() == COLOUR_BOMB || this->getBreed() == EMPTY || other->getBreed() == COLOUR_BOMB || other->getBreed() == EMPTY)
        return false;
    int cur_type = this->getBreed() / 4;
    int other_type = other->getBreed() / 4;
    //printf("%d %d- %d %d cur_type : %d other_type : %d\n", mX, mY, other->getX(), other->getY(), cur_type, other_type);
    return (other_type == cur_type);
}

void Candy::render(SDL_Renderer* lRenderer)
{
    mTexture->render(lRenderer, mX, mY);
}

void Candy::render(SDL_Renderer* lRenderer, int lX, int lY)
{
    mTexture->render(lRenderer, lX, lY);
}


Texture* Candy::getTexture()
{
    return mTexture;
}

CandyBreed Candy::getBreed()
{
    return type;
}

CandyType Candy::getType()
{
    if(type == EMPTY || type == COLOUR_BOMB)
        return NORMAL;
    int ret = (int) type;
    ret = type - (type / 4) * 4;
    return (CandyType) ret;
}

void Candy::swap(Candy* other)
{
    CandyBreed tmp_breed = this->getBreed();
    this->setBreed(other->getBreed());
    other->setBreed(tmp_breed);

    bool tmp_is2ndWrapped = this->isUnwrapped();
    this->is2ndWrapped = other->isUnwrapped();
    if(tmp_is2ndWrapped == true)
        other->makeUnwrapped();
    else
        other->makeNotUnwrapped();
}

void Candy::turnIntoNormal()
{
    if(type == EMPTY || type == COLOUR_BOMB)
        return;
    // turn into normal
    CandyBreed tmp_breed = (CandyBreed) ((int) type - (int) getType());
    setBreed(tmp_breed);
}

void Candy::setInfo(CandyBreed lType, int lX, int lY)
{
    setBreed(lType);
    mX = lX;
    mY = lY;
}

void Candy::setBreed(CandyBreed lCandyBreed)
{
    type = lCandyBreed;
    mTexture = &mCandyTexture[type];
    return;
}

int Candy::getWidth()
{
    return mWidth;
}

int Candy::getHeight()
{
    return mHeight;
}

int Candy::getX()
{
    return mX;
}

int Candy::getY()
{
    return mY;
}

void Candy::setX(int x)
{
    mX = x;
}

void Candy::setY(int y)
{
    mY = y;
}


int Candy::getIndex()
{
    return mIndex;
}
bool Candy::isUnwrapped()
{
    return is2ndWrapped;
}

void Candy::makeUnwrapped()
{
    is2ndWrapped = true;
}

void Candy::makeNotUnwrapped()
{
    is2ndWrapped = false;
}

