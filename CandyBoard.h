#include <vector>

#ifndef CANDY_H
#define CANDY_H
#include "Candy.h"
#endif // CANDY_H

enum DIRECTION
{
    LEFT,
    UP,
    RIGHT,
    DOWN,
    VERTICAL,
    HORIZONTAL
};

class CandyBoard
{
public:
    CandyBoard(int = 9, int = 9);
    //CandyBoard(int rows, int columns);
    ~CandyBoard();
    void handleEvent(SDL_Event* e);

    void fallingBoard(SDL_Renderer* lRenderer);
    int reIndex();
    void sugarCrush();
    void generateRandomBoard();
    int countPossibleMove();
    // count match in particular direction
    // direction can be one of the 4 DIRECTION
    int countMatchInDirection(int x, int y, DIRECTION direction);
    // for coordinate instead of board position
    int countMatchInDirectionCoord(int cx, int cy, DIRECTION direction);
    // return the number of candies deleted
    int deleteMatchInDirectionCoord(int cx, int cy, DIRECTION direction);
    int deleteMatchInDirection(int lX, int lY, DIRECTION direction);
    int deleteBySpecialCandy(Candy* );
    int deleteByColourBomb(Candy* );

    void displayCandyBoard(SDL_Renderer* lRenderer);
    // -1: if there is already a match
    // n: number of potential matches
    int getBoardWidth();
    int getBoardHeight();
    int getX();
    int getY();
    // to avoid logic error
    Candy* getCandy(int x, int y);
    Candy* getCandyCoord(int x,int y);
    int getPositionX(int );
    int getPositionY(int );
private:
    int multiplier;
    int mCascadeCounter;
    const int LEAST_POSSIBLE_MOVES;
    // for board texture - the grid containing the candies
    Texture boardTexture;
    // need to be more dynamic
    std::vector< std::vector<Candy> >candies;
    SDL_Point last_candy_choosen;
    int mRows, mColumns;
    int mWidth, mHeight;
    // coordinate of up left corner
    int mX, mY;
};
