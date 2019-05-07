#ifndef TEXTURE_H
#define TEXTURE_H
#include "Texture.h"
#endif // TEXTURE_H

#ifndef ANIMATION_H
#define ANIMATION_H
#include "Animation.h"
#endif // ANIMATION_H

enum CandyBreed
{
    BLUE_NORMAL, // 1
    BLUE_STRIPED_V, // 2
    BLUE_STRIPED_H, // 3
    BLUE_WRAPPED, // 4
    GREEN_NORMAL,
    GREEN_STRIPED_V,
    GREEN_STRIPED_H,
    GREEN_WRAPPED,
    ORANGE_NORMAL,
    ORANGE_STRIPED_V,
    ORANGE_STRIPED_H,
    ORANGE_WRAPPED,
    PURPLE_NORMAL,
    PURPLE_STRIPED_V,
    PURPLE_STRIPED_H,
    PURPLE_WRAPPED,
    RED_NORMAL,
    RED_STRIPED_V,
    RED_STRIPED_H,
    RED_WRAPPED,
    YELLOW_NORMAL,
    YELLOW_STRIPED_V,
    YELLOW_STRIPED_H,
    YELLOW_WRAPPED,
    COLOUR_BOMB,
    EMPTY,
    NUMBER_OF_CANDY_TYPES
};

enum CandyType
{
    NORMAL,
    STRIPED_V,
    STRIPED_H,
    WRAPPED
};

enum CandyColour
{
    BLUE,
    GREEN,
    ORANGE,
    PURPLE,
    RED,
    YELLOW
};

class Candy
{
public:
    static int CandyWidth;
    static int CandyHeight;
    Candy();
    ~Candy();
    // empty the cell after being matched
    void empty();
    // if two candy are of the same type
    bool equal(Candy* other);
    void render(SDL_Renderer* lRenderer);
    void render(SDL_Renderer* lRenderer, int, int);
    CandyBreed getBreed();
    void setInfo(CandyBreed lCandy = EMPTY, int lX = 0, int lY = 0);
    void setBreed(CandyBreed );
    void turnIntoNormal();
    CandyType getType();
    Texture* getTexture();
    int getWidth();
    int getHeight();
    int getX();
    int getY();
    void setX(int x);
    void setY(int y);
    int getVelX();
    int getVelY();
    int getIndex();
    bool isUnwrapped();
    void makeUnwrapped();
    void makeNotUnwrapped();
private:
    Texture* mTexture;
    CandyBreed type;
    int mWidth, mHeight;
    int mX, mY;
    int mRenderX, mRenderY;
    // a luxurious name for which rows the candy is in right now
    // the higher the index, the lower it is in the candy board
    int mIndex;
    // true if it has just been unwrapped
    // false if not wrapped or have not been unwrapped
    bool is2ndWrapped;
};
