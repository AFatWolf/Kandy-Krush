#ifndef SDL_TTF_H
#define SDL_TTF_H 1
#include <SDL_ttf.h>
#endif // SDL_TTF_H

class Texture
{
public:
    // add to private
    std::string pathToFile;
    SDL_Texture* mTexture;

    Texture();
    ~Texture();
    bool loadFromFile(SDL_Renderer* lRenderer, std::string path);
    bool loadFromRenderedText(SDL_Renderer* lRenderer, std::string textureText, SDL_Color textColor, TTF_Font* textureFont);
    void free();
    // render the texture at (x,y) coordinate
    void render(SDL_Renderer* lRenderer, int x, int y);
    void setAlpha(Uint8 alpha);
    Uint8 getAlpha();
    int getWidth();
    int getHeight();
    void setWidth(int );
    void setHeight(int );
    int getX();
    int getY();
    void setX(int );
    void setY(int );
private:
    // mTexture == instance texture
    // i stand for instance
    int mWidth;
    int mHeight;
    // coordinate
    int mX, mY;
    int mAlpha;

};
