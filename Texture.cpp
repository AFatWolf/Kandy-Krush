#ifndef BASIC_LIB
#define BASIC_LIB
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#endif // BASIC_LIB

#include "Texture.h"

Texture::Texture()
{
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
    mX = 0;
    mY = 0;
}

Texture::~Texture()
{
    free();
}

bool Texture::loadFromFile(SDL_Renderer* lRenderer, std::string path)
{
    // free current texture
    free();

    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str() );
    if(!loadedSurface)
    {
        printf("Unable to load image %s! Image error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        pathToFile = path;
        // set (0xFF,0xFF,0xFF) / white as transparent pixel
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface-> format, 255, 255, 254));

        newTexture = SDL_CreateTextureFromSurface(lRenderer, loadedSurface);
        if(!newTexture)
        {
            printf("Unable to create texture from image %s! SDL error: %s\n", path.c_str(), SDL_GetError());
        }
        else
        {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }

    mTexture = newTexture;
    //printf("Path: %s Address: %p\n", path.c_str(), mTexture);
    newTexture = NULL;
    // success or not
    return (mTexture != NULL);
}

bool Texture::loadFromRenderedText(SDL_Renderer* lRenderer, std::string textureText, SDL_Color textColor, TTF_Font* textureFont)
{
    free();

    SDL_Surface* textSurface = TTF_RenderText_Solid(textureFont, textureText.c_str(), textColor);
    if(textSurface == NULL)
    {
        printf("Unable to render text surface of text\"%s\"! SDL_ttf Error: %s\n", textureText.c_str(), TTF_GetError());
    }
    else
    {
        mTexture = SDL_CreateTextureFromSurface(lRenderer, textSurface);
        if( mTexture == NULL)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        SDL_FreeSurface(textSurface);
    }
    return mTexture != NULL;
}

void Texture::free()
{
    if(mTexture != NULL){
        SDL_DestroyTexture(mTexture);
        mWidth = 0;
        mHeight = 0;
        mX = 0;
        mY = 0;
    }
}

void Texture::render(SDL_Renderer* lRenderer, int x, int y)
{
    if(x < 0 || y < 0)
    {
        printf("Invalid coordinate passed in when trying to render image %s.\n", pathToFile.c_str());
        return;
    }
    mX = x;
    mY = y;
    // destiny Rect
    SDL_Rect dstRenderRect = { mX, mY, mWidth, mHeight};
    if(SDL_RenderCopy(lRenderer, mTexture, NULL, &dstRenderRect) < 0)
    {
        printf("Unable to render image %s! SDL Error: %s\n", pathToFile.c_str(), SDL_GetError());
    }
}

void Texture::setAlpha(Uint8 alpha)
{
    mAlpha = alpha;
    SDL_SetTextureAlphaMod( mTexture, alpha);
}

Uint8 Texture::getAlpha()
{
    return mAlpha;
}

int Texture::getWidth()
{
    return mWidth;
}

int Texture::getHeight()
{
    return mHeight;
}

void Texture::setWidth(int w)
{
    mWidth = w;
}

void Texture::setHeight(int h)
{
    mHeight = h;
}

int Texture::getX()
{
    return mX;
}

int Texture::getY()
{
    return mY;
}

void Texture::setX(int lX)
{
    mX = lX;
}

void Texture::setY(int lY)
{
    mY = lY;
}
