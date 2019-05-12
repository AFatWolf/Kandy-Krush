#ifndef BASIC_LIB
#define BASIC_LIB
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <vector>
#include <queue>
#endif // BASIC_LIB

#ifndef CANDY_BOARD_H
#define CANDY_BOARD_H
#include "CandyBoard.h"
#endif // CANDY_BOARD_H

#ifndef HEADER_H
#define HEADER_H 1
#include "Headers.h"
#endif // HEADER_H

#ifndef ANIMATION_H
#define ANIMATION_H 1
#include "Animation.h"
#endif // ANIMATION_H

CandyBoard::CandyBoard(int rows, int columns):
LEAST_POSSIBLE_MOVES(5)
{
    srand(time(0));
    candies.assign(rows, std::vector<Candy>());
    for(int i = 0; i < rows; i++)
        candies[i].assign(columns, Candy());

    mRows = rows;
    mColumns = columns;

    mWidth = mRows * Candy::CandyWidth;
    mHeight = mColumns * Candy::CandyHeight;

    // 20 is for that it does not touch the bottom
    mX = std::min(SCREEN_WIDTH / 3, SCREEN_WIDTH - mWidth - 20);
    mY = SCREEN_HEIGHT - mHeight - 20;

    last_candy_choosen = {-1, -1};

    multiplier = 1;
    mCascadeCounter = 0;

    generateRandomBoard();
    printf("Candy board constructed.\n");
}

CandyBoard::~CandyBoard()
{
    boardTexture.free();
}

void CandyBoard::handleEvent( SDL_Event* e )
{
    if(e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP || e->type == SDL_MOUSEMOTION)
	{
		//Get mouse position
		int x, y;
		int mouse_state = SDL_GetMouseState( &x, &y );
        // printf("Mouse in X:%d Y:%d and mX %d mY %d\n", x, y, mX, mY);
        // determine which candy it is
        if(x >= mX && x <= mX + mWidth && y >= mY && y <= mY + mHeight)
        {
            switch( e->type )
            {

            case SDL_MOUSEBUTTONDOWN:
                {
                    // first candy to touch
                    if(last_candy_choosen.x == -1 && last_candy_choosen.y == -1)
                    {
                        last_candy_choosen = {x, y};
                        printf("Last choose: X %d Y %d\n", x, y);
                    }
                    break;
                }
            /*
            case SDL_MOUSEMOTION:
                if(mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT) && last_candy_choosen.x >= 0 && last_candy_choosen.y >= 0)
                {
                    if(x != last_candy_choosen.x && y != last_candy_choosen.y)
                    {
                        // change the last candy coord

                        Candy* cur_candy = getCandyCoord(last_candy_choosen.x, last_candy_choosen.y);
                        int candy_X = cur_candy->getX();
                        int candy_Y = cur_candy->getY();
                        int center_X = candy_X + Candy::CandyWidth / 2;
                        int center_Y = candy_Y + Candy::CandyHeight / 2;
                        int dx = x - last_candy_choosen.x;
                        int dy = y - last_candy_choosen.y;
                        int new_x = candy_X;
                        int new_y = candy_Y;
                        if(abs(dx) > abs(dy))
                            new_x += dx;
                        else
                            new_y += dy;
                        if(x >= center_X && x <= center_X + Candy::CandyWidth && x >= center_Y && y <= center_Y + Candy::CandyHeight)
                        {
                            if(abs(new_x - center_X) < Candy::CandyWidth * 2 && abs(new_y - center_Y) < Candy::CandyHeight * 2)
                            {

                                printf("X %d Y %d NX %d NY %d dx %d dy %d\n", x, y, candy_X, candy_Y, dx, dy);
                                SDL_RenderClear(gRenderer);
                                renderFrame();
                                for(int i = 0; i < mRows; i++)
                                    for(int j = 0; j < mColumns; j++)
                                    {
                                        Candy* tmp = getCandy(i , j);
                                        if(tmp != cur_candy)
                                        {
                                            tmp->render(gRenderer);
                                        }
                                    }
                                cur_candy->render(gRenderer, new_x, new_y);
                                ANIMATION_MODE = 1;
                                SDL_RenderPresent(gRenderer);
                                // if the cursor still in that candy
                            }
                            else
                            {
                                last_candy_choosen = {-2, -2};
                                ANIMATION_MODE = false;
                            }
                        }
                        else
                        {
                            ANIMATION_MODE = false;
                        }
                    }
                    break;
                }
            */
            case SDL_MOUSEBUTTONUP:
                printf("Choose X %d y %d\n", x, y);
                if(last_candy_choosen.x != -1 && last_candy_choosen.y != -1 && x != last_candy_choosen.x && y != last_candy_choosen.y)
                {
                    // swap
                    // meaning that it is adjacent cell and not itself
                    int ddx = x - last_candy_choosen.x;
                    int ddy = y - last_candy_choosen.y;
                    // if its horizontal (ddx > ddy)
                    x = last_candy_choosen.x + (abs(ddx) / ddx) * (abs(ddx) > abs(ddy)) * Candy::CandyWidth;
                    // else if its vertical
                    y = last_candy_choosen.y + (abs(ddy) / ddy) * (abs(ddy) >= abs(ddx)) * Candy::CandyHeight;
                    printf("Next choose: X %d Y %d\n", x, y);
                    if(abs(getPositionX(last_candy_choosen.x) - getPositionX(x)) + abs(getPositionY(last_candy_choosen.y) - getPositionY(y)) <= 1
                        && abs(getPositionX(last_candy_choosen.x) - getPositionX(x)) + abs(getPositionY(last_candy_choosen.y) - getPositionY(y)) != 0)
                    {
                        Candy* last_candy = getCandyCoord(last_candy_choosen.x, last_candy_choosen.y);
                        Candy* current_candy = getCandyCoord(x, y);
                        CandyBreed tmp = last_candy->getBreed();

                        // swapping effect
                        // render to create  effect
                        Candy tmp_last_candy = *(last_candy);
                        Candy tmp_current_candy = *(current_candy);
                        // caculate the difference in coordinate between two candy
                        int dx = tmp_current_candy.getX() - tmp_last_candy.getX();
                        int dy = tmp_current_candy.getY() - tmp_last_candy.getY();
                        // if horizontally
                        if(dx)
                        {
                            for(int i = 0; i < abs(dx); i+=10)
                            {
                                // i * (abs(dx) / dx) == might be the RIGHT side
                                SDL_RenderClear(gRenderer);
                                renderFrame();
                                for(int i = 0; i < mRows; i ++)
                                    for(int j = 0; j < mColumns; j++)
                                    {
                                        if(getCandy(i,j) != last_candy && getCandy(i, j) != current_candy)
                                        getCandy(i ,j)->render(gRenderer);
                                    }
                                tmp_last_candy.render(gRenderer, tmp_last_candy.getX() + i * (abs(dx) / dx), tmp_last_candy.getY());
                                tmp_current_candy.render(gRenderer, tmp_current_candy.getX() - i * (abs(dx) / dx), tmp_last_candy.getY());
                                SDL_RenderPresent(gRenderer);
                                //delay(100);
                            }
                        }
                        // else if vertically
                        else if(dy)
                        {
                            for(int i = 0;i < abs(dy); i+= 10)
                            {
                                SDL_RenderClear(gRenderer);
                                renderFrame();
                                for(int i = 0; i < mRows; i++)
                                    for(int j = 0; j < mColumns; j++)
                                    {
                                        if(getCandy(i,j) != last_candy && getCandy(i, j) != current_candy)
                                            getCandy(i ,j)->render(gRenderer);
                                    }
                                tmp_last_candy.render(gRenderer, tmp_last_candy.getX(), tmp_last_candy.getY() + i * (abs(dy) / dy));
                                tmp_current_candy.render(gRenderer, tmp_current_candy.getX(), tmp_current_candy.getY() - i * (abs(dy) / dy));
                                SDL_RenderPresent(gRenderer);
                                //delay(100);
                            }

                        }
                        // swap after effect
                        last_candy->setBreed(current_candy->getBreed());
                        current_candy->setBreed(tmp);
                        // play swap sound
                        soundEffect[NORMAL_SWITCH].play();

                        // reset multiplier
                        multiplier = 1;
                        // reset cascade counter
                        mCascadeCounter = 0;

                        // if one of them is color bomb

                        if(last_candy->getBreed() == COLOUR_BOMB)
                        {
                            gMoves -= 1;
                            last_candy->setBreed(EMPTY);
                            deleteByColourBomb(current_candy);
                        }
                        else if(current_candy->getBreed() == COLOUR_BOMB)
                        {
                            gMoves -= 1;
                            current_candy->setBreed(EMPTY);
                            deleteByColourBomb(last_candy);
                        }
                        else
                        {

                            // check if valid move
                            bool last_candy_match = false;
                            // -1 to avoid count this twice
                            // calculate the number of match after swapping
                            int last_candy_horizontal_match = countMatchInDirectionCoord(last_candy_choosen.x, last_candy_choosen.y, LEFT)
                                                    + countMatchInDirectionCoord(last_candy_choosen.x, last_candy_choosen.y, RIGHT) - 1;
                            int last_candy_vertical_match = countMatchInDirectionCoord(last_candy_choosen.x, last_candy_choosen.y, UP)
                                                    + countMatchInDirectionCoord(last_candy_choosen.x, last_candy_choosen.y, DOWN) - 1;
                            last_candy_match |= (last_candy_vertical_match >= 3);
                            last_candy_match |= (last_candy_horizontal_match >= 3);
                            printf("Last: ver %d hor %d\n",last_candy_vertical_match, last_candy_horizontal_match);

                            bool current_candy_match = false;
                            int current_candy_horizontal_match = countMatchInDirectionCoord(x, y, LEFT)
                                                + countMatchInDirectionCoord(x, y, RIGHT) - 1;
                            int current_candy_vertical_match = countMatchInDirectionCoord(x, y, UP)
                                                + countMatchInDirectionCoord(x, y, DOWN) - 1;
                            current_candy_match |= (current_candy_vertical_match >= 3);
                            current_candy_match |= (current_candy_horizontal_match >= 3);
                            printf("Current: ver %d hor %d\n",current_candy_vertical_match, current_candy_horizontal_match);

                            // if there is no match after swapping
                            if( !last_candy_match  && !current_candy_match)
                            {
                                current_candy->setBreed(last_candy->getBreed());
                                last_candy->setBreed(tmp);
                                // play swap back sound
                                soundEffect[NEGATIVE_SWITCH].play();
                            }
                            //
                            else
                            {
                                // decrease the moves number
                                gMoves -= 1;

                                // delete the match
                                // there is a match vertically -> delete
                                CandyBreed last_type = getCandyCoord(last_candy_choosen.x, last_candy_choosen.y)->getBreed() - getCandyCoord(last_candy_choosen.x, last_candy_choosen.y)->getType();
                                CandyBreed cur_type = getCandyCoord(x, y)->getBreed() - getCandyCoord(x, y)->getType();
                                SDL_RenderClear(gRenderer);
                                renderFrame();
                                displayCandyBoard(gRenderer);
                                SDL_RenderPresent(gRenderer);
                                delay(100);
                                printf("Last type: %d Cur Type: %d\n",last_type, cur_type);

                                if(last_candy_vertical_match >= 3)
                                {
                                    // set to current state of the candy because the delete() base on the condition that the current candy is not EMPTY
                                    getCandyCoord(last_candy_choosen.x, last_candy_choosen.y)->setBreed(last_type);
                                    deleteMatchInDirectionCoord(last_candy_choosen.x, last_candy_choosen.y, VERTICAL);
                                }
                                if(last_candy_horizontal_match >= 3)
                                {
                                    getCandyCoord(last_candy_choosen.x, last_candy_choosen.y)->setBreed(last_type);
                                    deleteMatchInDirectionCoord(last_candy_choosen.x, last_candy_choosen.y, HORIZONTAL);
                                }
                                if(current_candy_vertical_match >= 3)
                                {
                                    getCandyCoord(x, y)->setBreed(cur_type);
                                    deleteMatchInDirectionCoord(x, y, VERTICAL);
                                }
                                if(current_candy_horizontal_match >= 3)
                                {
                                    getCandyCoord(x, y)->setBreed(cur_type);
                                    deleteMatchInDirectionCoord(x, y, HORIZONTAL);
                                }

                                // now case it creates special candy
                                // the L-candy case
                                if(last_candy_horizontal_match >= 3 && last_candy_vertical_match >= 3)
                                {
                                    // becoming wrapped candy
                                    last_candy->setBreed( (CandyBreed) ((int) last_type + (int )WRAPPED));
                                    gScoreNumber += 200;
                                    // play wrapped sound
                                    soundEffect[WRAPPED_CANDY_CREATED].play();
                                }
                                // COLOUR BOMB case
                                if(last_candy_vertical_match >= 5 || last_candy_horizontal_match >= 5)
                                {
                                    last_candy->setBreed(COLOUR_BOMB);
                                    gScoreNumber += 200;
                                    // play create sound
                                    soundEffect[COLOUR_BOMB_CREATE].play();
                                }
                                // the Striped case
                                else
                                {
                                    if(last_candy_horizontal_match >= 4)
                                    {
                                        last_candy->setBreed((CandyBreed) ((int) last_type + (int) STRIPED_H));
                                        gScoreNumber += 120;
                                        // play create sound
                                        soundEffect[STRIPED_CANDY_CREATE].play();
                                    }
                                    if(last_candy_vertical_match >= 4)
                                    {
                                        last_candy->setBreed((CandyBreed) ((int) last_type + (int) STRIPED_V));
                                        gScoreNumber += 120;
                                        // play create sound
                                        soundEffect[STRIPED_CANDY_CREATE].play();
                                    }
                                }
                                // the same goes for the current candy
                                if(current_candy_horizontal_match >= 3 && current_candy_vertical_match >= 3)
                                {
                                    // becoming wrapped candy
                                    current_candy->setBreed((CandyBreed) ((int) cur_type + (int) WRAPPED));
                                    gScoreNumber += 200;
                                    soundEffect[WRAPPED_CANDY_CREATED].play();
                                }

                                // COLOUR BOMB case
                                if(current_candy_horizontal_match >= 5 || current_candy_vertical_match >= 5)
                                {
                                    current_candy->setBreed(COLOUR_BOMB);
                                    gScoreNumber += 200;
                                    soundEffect[COLOUR_BOMB_CREATE].play();
                                }
                                // the Striped case
                                else
                                {
                                    if(current_candy_horizontal_match >= 4)
                                    {
                                        current_candy->setBreed((CandyBreed) ((int) cur_type + (int) STRIPED_H));
                                        gScoreNumber += 120;
                                        soundEffect[STRIPED_H].play();
                                    }

                                    if(current_candy_vertical_match >= 4)
                                    {
                                        current_candy->setBreed((CandyBreed) ((int) cur_type + (int) STRIPED_V));
                                        gScoreNumber += 120;
                                        soundEffect[STRIPED_V].play();
                                    }
                                }

                                // display the board being EMPTY some spot
                                SDL_RenderClear(gRenderer);
                                renderFrame();
                                displayCandyBoard(gRenderer);
                                SDL_RenderPresent(gRenderer);
                                delay(100);
                            }
                        }
                        // reIndex() : to fill the EMPTY spot
                        // return: nunber of candies being deleted
                        int sweetOrNot = reIndex();
                        int old_man_start_x = SCREEN_WIDTH / 3;
                        int old_man_start_y = SCREEN_HEIGHT / 3;
                        int display = -1;
                        printf("Sweet or not: %d\n", sweetOrNot);
                        // DIVINE
                        if(sweetOrNot >= 30)
                        {
                            display = DIVINE;
                        }
                        // DELICIOUS
                        else if(sweetOrNot >= 24)
                        {
                            display = DELICIOUS;
                        }
                        else if(sweetOrNot >= 18)
                        {
                            display = TASTY;
                        }
                        else if(sweetOrNot >= 12)
                        {
                            display = SWEET;
                        }
                        if(display != -1)
                        {

                            oldManVoice[display].play();
                            Animation drop;
                            drop.setTexture(&oldManTexture[display]);
                            drop.setPosition(old_man_start_x, 0);
                            drop.setDest(old_man_start_x, old_man_start_y);
                            do
                            {
                                SDL_RenderClear(gRenderer);
                                renderFrame();
                                displayCandyBoard(gRenderer);
                                drop.fall(gRenderer);
                                SDL_RenderPresent(gRenderer);
                                delay(10);
                            } while(drop.getStatus() != STANDING_STILL);
                            delay(500);

                        }

                    }
                    //reset
                    last_candy_choosen = {-1, -1};
                    ANIMATION_MODE = false;
                    break;
                }
            }
        }
    }
}

// can be fixed to run faster by generating without caring about the possible moves
int CandyBoard::reIndex()
{

    bool have_empty = false;
    // number of falls down a.k.a. number of cascades
    int times = 0;
    // number of candies deleted
    int number_candies_deleted = 0;
    // candy fall down
    do
    {
        // reset
        have_empty = false;
        // cascade -> multiplier += 1
        multiplier += 1;
        std::vector< std::vector<Candy*> > emptyNeedFilling;
        emptyNeedFilling.assign(mColumns, std::vector<Candy*>());
        // using for falling animation
        std::vector<Animation> fallCandy;
        // to render when others candies trying to fall
        std::vector<Candy*> notFallingCandies;

        // count number of EMPTY cell -> number of candies deleted previouslt
        for(int i = 0; i < mRows; i++)
            for(int j = 0; j < mColumns; j++)
            {
                number_candies_deleted += (getCandy(j, i)->getBreed() == EMPTY);
            }

        // prepare which one is empty
        for(int x = 0; x < mColumns; x++)
        {
            // the algorithm is: swap the EMPTY with the nearest not EMPTY above in the same column
            std::queue<Candy*> needSwap;
            for(int y = mRows - 1; y >= 0; y--)
            {
                // if empty candy
                if(getCandy(x, y)->getBreed() == EMPTY)
                {
                    needSwap.push(getCandy(x, y));
                }
                else
                {
                        // have sth need swapped
                    if(!needSwap.empty())
                    {
                        // take the farthest EMPTY
                        Candy* down_empty_candy = needSwap.front();
                        needSwap.pop();
                        // preparing for animation

                        // nearest not EMPTY
                        Candy* up_not_empty_candy = getCandy(x, y);
                        // swapping
                        // the down
                        //down_empty_candy->setBreed(up_not_empty_candy->getBreed());
                        down_empty_candy->swap(up_not_empty_candy);
                        // the up
                        up_not_empty_candy->setBreed(EMPTY);
                        // now is the new EMPTY
                        needSwap.push(up_not_empty_candy);

                        // for animation
                        Animation down_candy_anime;
                        // take the falling candy image
                        down_candy_anime.setTexture(down_empty_candy->getTexture());
                        // set the start position to the falling
                        down_candy_anime.setPosition(up_not_empty_candy->getX(), up_not_empty_candy->getY());
                        // set the destination to the EMPTY has just been filled
                        down_candy_anime.setDest(down_empty_candy->getX(), down_empty_candy->getY());
                        fallCandy.push_back(down_candy_anime);
                    }
                    else
                    {
                        notFallingCandies.push_back(getCandy(x, y));
                    }
                }
            }
            while(!needSwap.empty())
            {
                // put the EMPTY cell of column x in this
                emptyNeedFilling[x].push_back(needSwap.front());
                needSwap.pop();
            }
        }

         // fill EMPTY
        for(int x = 0; x < mColumns; x++)
        {
            for(int i = 0; i < emptyNeedFilling[x].size(); i++)
            {
                Candy* cur_candy = emptyNeedFilling[x][i];
                CandyBreed generate_type;
                do
                {
                    generate_type = (CandyBreed) (CandyBreed)( ( (rand() % (int) NUMBER_OF_CANDY_TYPES) / 4)  * 4);
                } while(generate_type == EMPTY || generate_type == COLOUR_BOMB);
                cur_candy->setBreed(generate_type);

                Animation empty_down_candy_anime;
                // falling from outside of the board
                empty_down_candy_anime.setTexture(cur_candy->getTexture());
                // (i+1) so that it has falling order
                empty_down_candy_anime.setPosition(cur_candy->getX(),(i + 1) * (- Candy::CandyHeight) );
                empty_down_candy_anime.setDest(cur_candy->getX(), cur_candy->getY());
                fallCandy.push_back(empty_down_candy_anime);
            }
        }
        // falling animation
        bool falling = true;
        do
        {
            SDL_RenderClear(gRenderer);
            renderFrame();

            for(int i = 0; i < notFallingCandies.size(); i++)
            {
                notFallingCandies[i]->render(gRenderer);
            }
            for(int i = 0; i < fallCandy.size(); i++)
            {
                // now fall
                fallCandy[i].fall(gRenderer);
                if(fallCandy[i].getStatus() != FALLING)
                {
                    falling = false;
                    //break;
                }
            }
            // no candy to fall
            if(fallCandy.empty())
            {
                falling = false;
                Sound land = (Sound) std::min((int) CANDY_LAND1 + times, (int) CANDY_LAND4);
                // play landing sound
                soundEffect[land].play();
            }


            SDL_RenderPresent(gRenderer);
            delay(10);
            //delay(2000);
        } while(falling == true);

        // show after falling down
        SDL_RenderClear(gRenderer);
        renderFrame();
        displayCandyBoard(gRenderer);
        SDL_RenderPresent(gRenderer);
        // delay so that player can see it
        delay(200);
        // play combo sound
        // only have 12 landing sound so ...
        Sound combo = (Sound) std::min((int) COMBO_SOUND1 + times, (int) COMBO_SOUND12);
        soundEffect[combo].play();
        // increase time
        times ++;

        // delete unwrapped candy from wrapped candy first before matching
        for(int y = 0; y < mRows; y++)
            for(int x = 0; x < mColumns; x++)
            {
            if(getCandy(x, y) -> isUnwrapped() == true && getCandy(x, y) -> getType() == WRAPPED)
                {
                    deleteBySpecialCandy(getCandy(x, y));
                    have_empty = true;
                }
            }

        for(int y = 0; y < mRows; y++)
            for(int x = 0; x < mColumns; x++)
            {
                CandyBreed cur_type = getCandy(x, y) -> getBreed() - getCandy(x, y) -> getType();
                bool vertical_match = false;
                int numb_v_match = countMatchInDirection(x, y, VERTICAL);
                int numb_h_match = countMatchInDirection(x, y, HORIZONTAL);
                // create special candy
                if(numb_v_match >= 3 && numb_h_match >= 3)
                {
                    // delete match
                    deleteMatchInDirection(x, y, VERTICAL);
                    getCandy(x, y)->setBreed(cur_type);
                    deleteMatchInDirection(x, y, HORIZONTAL);
                    have_empty = true;

                    gScoreNumber += 200;
                    // become wrapped
                    getCandy(x, y)->setBreed((CandyBreed) ((int) cur_type + (int) WRAPPED));
                    soundEffect[WRAPPED_CANDY_CREATED].play();
                }
            }
        // count and match the match after falling and filling
        // create special candy before matching normally
        for(int y = 0; y < mRows; y++)
            for(int x = 0; x < mColumns; x++)
            {
                CandyBreed cur_type = getCandy(x, y) -> getBreed();
                bool vertical_match = false;
                int numb_v_match = countMatchInDirection(x, y, VERTICAL);
                int numb_h_match = countMatchInDirection(x, y, HORIZONTAL);
                // create special candy
                /*
                if(numb_v_match >= 3 && numb_h_match >= 3)
                {
                    // delete match
                    deleteMatchInDirection(x, y, VERTICAL);
                    getCandy(x, y)->setBreed(cur_type);
                    deleteMatchInDirection(x, y, HORIZONTAL);
                    have_empty = true;

                    gScoreNumber += 200;
                    // become wrapped
                    getCandy(x, y)->setBreed((CandyBreed) ((int) cur_type + (int) WRAPPED));
                    soundEffect[WRAPPED_CANDY_CREATED].play();
                }
                */
                // the bomb
                if(numb_v_match >= 5 || numb_h_match >= 5)
                {
                    if(numb_v_match >= 5)
                    {
                        deleteMatchInDirection(x, y, VERTICAL);
                        have_empty = true;
                    }
                    if(numb_h_match >= 5)
                    {
                        deleteMatchInDirection(x, y, HORIZONTAL);
                        have_empty = true;
                    }
                    getCandy(x ,y)->setBreed(COLOUR_BOMB);
                    gScoreNumber += 200;
                    // play creation sound
                    soundEffect[COLOUR_BOMB_CREATE].play();
                }
                // the striped case
                else
                {
                    if(numb_v_match >= 4)
                    {
                        deleteMatchInDirection(x, y, VERTICAL);
                        have_empty = true;
                        getCandy(x,y)->setBreed((CandyBreed) ((int) cur_type + (int) STRIPED_V));
                        gScoreNumber += 120;
                        soundEffect[STRIPED_V].play();
                    }
                    if(numb_h_match >= 4)
                    {
                        deleteMatchInDirection(x, y, HORIZONTAL);
                        have_empty = true;
                        getCandy(x, y)->setBreed((CandyBreed) ((int) cur_type + (int) STRIPED_H));
                        gScoreNumber += 120;
                        soundEffect[STRIPED_H].play();
                    }
                }
            }
        // deal with match that not create special candies
        for(int y = 0; y < mRows; y++)
            for(int x = 0; x < mColumns;x ++)
            {
                CandyBreed cur_type = getCandy(x, y) -> getBreed();
                bool vertical_match = false;
                int numb_v_match = 0;
                int numb_h_match = 0;
                if(countMatchInDirection(x, y, VERTICAL) >= 3)
                {
                    numb_v_match = deleteMatchInDirection(x, y, VERTICAL);
                    have_empty = true;
                    vertical_match = true;
                }
                // only if have vertical match
                // reset so that the count horizontally is correct in the L shape
                if(vertical_match)
                    getCandy(x, y)->setBreed(cur_type);
                if(countMatchInDirection(x, y, HORIZONTAL) >= 3)
                {
                    numb_h_match = deleteMatchInDirection(x, y, HORIZONTAL);
                    have_empty = true;
                }
                if(vertical_match)
                    getCandy(x,y) -> setBreed(EMPTY);
            }
        // show board after clearing matches
        SDL_RenderClear(gRenderer);
        renderFrame();
        displayCandyBoard(gRenderer);
        SDL_RenderPresent(gRenderer);

    } while(have_empty);

    return number_candies_deleted;
}

void CandyBoard::sugarCrush()
{
    // for when there is still special candy left
    bool have_special = false;
    int sweetOrNot = 0;
    int old_man_start_x;
    int old_man_start_y;
    int display = -1;
    do
    {
        // reset
        have_special = false;
        for(int i = 0; i < mRows;i ++)
            for(int j = 0; j < mColumns; j++)
            {
                Candy* tmp_candy = getCandy(j ,i);
                if(tmp_candy == NULL)
                    continue;
                if(tmp_candy->getType() != NORMAL)
                {
                    have_special = true;
                    deleteBySpecialCandy(tmp_candy);
                }
                if(tmp_candy->getBreed() == COLOUR_BOMB)
                {
                    have_special = true;
                    printf("COLOUR BOMB!\n");
                    // choose randomly an adjancent cell that is a NORMAL candy
                    int dx[3] = {-1, 0, 1};
                    int dy[3] = {-1, 0, 1};
                    int ix = 0, iy = 0;
                    // in case there is no normal candy
                    bool have_normal = false;
                    for(int ii = 0; ii < 3; ii++)
                        for(int jj = 0; jj < 3; jj++)
                        {
                            if(j + ix >= 0 && j + ix < mColumns && i + iy >= 0 && i + iy < mRows && getCandy(j + ix, i + iy)->getType() == NORMAL && getCandy(j + ix, i + iy)->getBreed() != EMPTY)
                                have_normal = true;
                        }
                    // if have a normal cell
                    if(have_normal)
                        do
                        {
                            ix = dx[rand() % 3];
                            iy = dy[rand() % 3];
                        } while(!(j + ix >= 0 && j + ix < mColumns && i + iy >= 0 && i + iy < mRows && getCandy(j + ix, i + iy)->getType() == NORMAL && getCandy(j + ix, i + iy)->getBreed() != EMPTY));
                    // empty the bomb beforehand
                    getCandy(j, i)->setBreed(EMPTY);
                    // treat the candy as if trade with colour bomb
                    deleteByColourBomb(getCandy(j + ix, i + iy));

                }
            }
        // call reindex after sugar crush
       // reIndex() : to fill the EMPTY spot
        // return: nunber of candies being deleted
        sweetOrNot += reIndex();
        old_man_start_x = SCREEN_WIDTH / 3;
        old_man_start_y = SCREEN_HEIGHT / 3;
        printf("Done\n");
    } while(have_special == true);

    printf("Sweet or not: %d\n", sweetOrNot);
    // DIVINE
    if(sweetOrNot >= 30)
    {
        display = DIVINE;
    }
    // DELICIOUS
    else if(sweetOrNot >= 24)
    {
        display = DELICIOUS;
    }
    else if(sweetOrNot >= 18)
    {
        display = TASTY;
    }
    else if(sweetOrNot >= 12)
    {
        display = SWEET;
    }
    if(display != -1)
    {
        oldManVoice[display].play();
        Animation drop;
        drop.setTexture(&oldManTexture[display]);
        drop.setPosition(old_man_start_x, 0);
        drop.setDest(old_man_start_x, old_man_start_y);
        do
        {
            SDL_RenderClear(gRenderer);
            renderFrame();
            displayCandyBoard(gRenderer);
            drop.fall(gRenderer);
            SDL_RenderPresent(gRenderer);
            _sleep(10);
        } while(drop.getStatus() != STANDING_STILL);
        //_sleep(200);
    }
}

void CandyBoard::generateRandomBoard()
{
    int least_moves = 5;
    // see if there is already match exists
    int already_match = 0;
    do
    {
        // reset each loop
        already_match = 0;
        for(int i = 0; i < mRows; i++)
            for(int j = 0; j < mColumns; j++)
            {
                int candyY = mY + i * Candy::CandyHeight;
                int candyX = mX + j * Candy::CandyWidth;
                CandyBreed candyType;
                do
                {
                    // "/4" to get the color
                    // "*4" to get the actual candy
                    candyType = (CandyBreed)( ( (rand() % (int) NUMBER_OF_CANDY_TYPES) / 4)  * 4  );
                    //if(rand() % 10 == 1)
                //    candyType = (CandyBreed) ((int) candyType + (int) WRAPPED);
                } while(candyType == EMPTY || candyType == COLOUR_BOMB);
                candies[i][j].setInfo(candyType, candyX, candyY);
            }
        for(int i = 0; i < mRows;i ++)
            for(int j = 0; j < mColumns; j++)
            {
                if(countMatchInDirection(i, j, RIGHT) >= 3)
                    already_match = 1;
                if(countMatchInDirection(i, j, DOWN) >= 3)
                    already_match = 1;
            }
    } while(countPossibleMove() < LEAST_POSSIBLE_MOVES || already_match == 1);
}

int CandyBoard::countPossibleMove()
{
    int moves = 0;
    // count already match
    for(int i = 0; i < mRows;i ++)
            for(int j = 0; j < mColumns; j++)
            {
                if(countMatchInDirection(i, j, RIGHT) >= 3)
                    moves += 1;
                if(countMatchInDirection(i, j, DOWN) >= 3)
                    moves += 1;
            }
    // count potential match
    // algorithm: swap the (i,j) with the right cell (i,j+1) or down cell (i+1,j) then check possible move
    for(int i = 0; i < mRows; i++)
        for(int j = 0; j < mColumns; j++)
        {
            // horizontal swap
            // (i,j) with (i,j+1)
            // for recover after swap
            CandyBreed oldCur = candies[i][j].getBreed();
            int right_swap_match = 0;
            int down_swap_match = 0;

            // if a valid X
            if(j+1 < mColumns)
            {
                CandyBreed rightType = candies[i][j+1].getBreed();
                // swap 2 types of candies
                candies[i][j].setBreed(rightType);
                candies[i][j+1].setBreed(oldCur);
                // - 1 because the current candy is counted twice
                right_swap_match = countMatchInDirection(i,j,RIGHT) + countMatchInDirection(i,j,LEFT) - 1;
                right_swap_match += countMatchInDirection(i,j,UP) + countMatchInDirection(i,j,DOWN) - 1;
                if(oldCur != rightType)
                {
                    right_swap_match += countMatchInDirection(i,j+1,RIGHT) + countMatchInDirection(i,j+1,LEFT) - 1;
                    right_swap_match += countMatchInDirection(i,j+1,UP) + countMatchInDirection(i,j+1,DOWN) - 1;
                }
                //swap back
                candies[i][j].setBreed(oldCur);
                candies[i][j+1].setBreed(rightType);
            }

            // vertical swap
            // (i,j) with (i+1,j)
            // if a valid Y
            if(i+1 < mRows)
            {
                CandyBreed downType = candies[i+1][j].getBreed();
                // swap
                candies[i][j].setBreed(downType);
                candies[i+1][j].setBreed(oldCur);
                down_swap_match = countMatchInDirection(i,j,UP) + countMatchInDirection(i,j,DOWN) - 1;
                down_swap_match += countMatchInDirection(i,j,RIGHT) + countMatchInDirection(i,j,LEFT) - 1;
                if(oldCur != downType)
                {
                    down_swap_match += countMatchInDirection(i+1,j,UP) + countMatchInDirection(i+1,j,DOWN) - 1;
                    down_swap_match += countMatchInDirection(i+1,j,RIGHT) + countMatchInDirection(i+1,j,LEFT) - 1;
                }
                // swap back
                candies[i][j].setBreed(oldCur);
                candies[i+1][j].setBreed(downType);
            }

            // need fixed bc might be more than 1 match
            if(down_swap_match >= 3)
                moves += 1;
            if(right_swap_match >= 3)
                moves += 1;
            // run faster
            if(moves >= LEAST_POSSIBLE_MOVES)
                return moves;
        }
    return moves;
}

int CandyBoard::countMatchInDirection(int lX, int lY, DIRECTION direction)
{
    if(lX < 0 || lX >= mColumns || lY < 0 || lY >= mRows)
    {
        printf("CandyBoard::countMatchInDirection Invalid position passed! %d %d\n", lX, lY);
        return -1;
    }
    // not for empty cell
    if(getCandy(lX, lY) -> getBreed() == EMPTY)
        return -1;

    int match = 0;
    int x = lX, y = lY;
    CandyBreed curType = getCandy(lX, lY)->getBreed();
    Candy* cur_candy = getCandy(lX, lY);
    switch(direction)
    {
    case LEFT:
        while(x >= 0 && getCandy(x, lY)->equal(cur_candy))
        {
            match ++;
            x --;
        }
        break;
    case RIGHT:
        while(x < mColumns && getCandy(x, lY)->equal(cur_candy))
        {
            match ++;
            x ++;
        }
        break;
    case UP:
        while(y  >= 0 && getCandy(lX, y)->equal(cur_candy))
        {
            match ++;
            y --;
        }
        break;
    case DOWN:
        while(y < mRows && getCandy(lX, y)->equal(cur_candy))
        {
            match ++;
            y ++;
        }
        break;
    case VERTICAL:
        match = countMatchInDirection(lX, lY, UP)
                                        + countMatchInDirection(lX, lY, DOWN) - 1;
        break;
    case HORIZONTAL:
        match = countMatchInDirection(lX, lY, LEFT)
                                        + countMatchInDirection(lX, lY, RIGHT) - 1;
        break;
    default:
        printf("CandyBoard::countMatchInDirection : Invalid direction passed\n");
        break;
    }
    return match;
}

int CandyBoard::countMatchInDirectionCoord(int lcX, int lcY, DIRECTION direction)
{
    int posX = (lcX - mX) / Candy::CandyWidth;
    int posY = (lcY - mY) / Candy::CandyHeight;
    return countMatchInDirection(posX, posY, direction);
}

int CandyBoard::deleteMatchInDirection(int lX, int lY, DIRECTION direction)
{
    if(lX < 0 || lX >= mColumns || lY < 0 || lY >= mRows)
        return -1;
    // the current candy will be deleted later
    int x = lX - 1, y = lY - 1;
    CandyBreed cur_type = getCandy(lX, lY)->getBreed();
    Candy* cur_candy = getCandy(lX, lY);
    // to store the special candy being matched
    std::vector<Candy*> specialCandy;
    // candies directly affected by this function
    int number_of_candy_deleted = 0;
    switch(direction)
    {
    case HORIZONTAL:
        // LEFT
        while(x >= 0 && getCandy(x, lY)->equal(cur_candy))
        {

            number_of_candy_deleted ++;
            // if special empty after using its power!
            if(getCandy(x, lY) -> getType() != NORMAL)
                specialCandy.push_back(getCandy(x, lY));
            // empty the match
            else
                getCandy(x, lY)->setBreed(EMPTY);
            x--;
        }
        // reset to previous position + 1 to replace the RIGHT
        x = lX + 1;
        while(x < mColumns && getCandy(x, lY)->equal(cur_candy))
        {
            number_of_candy_deleted ++;
            if(getCandy(x, lY) -> getType() != NORMAL)
                specialCandy.push_back(getCandy(x, lY));
            else
                getCandy(x, lY)->setBreed(EMPTY);
            x ++;
        }
        break;
    case VERTICAL:
        // UP
        while(y >= 0 && getCandy(lX, y)->equal(cur_candy))
        {
            number_of_candy_deleted ++;
            if(getCandy(lX, y) -> getType() != NORMAL)
                specialCandy.push_back(getCandy(lX, y));
            else
                getCandy(lX, y)->setBreed(EMPTY);
            y --;
        }
        // reset for DOWN
        y = lY + 1;
        while(y < mRows && getCandy(lX, y)->equal(cur_candy))
        {
            number_of_candy_deleted ++;
            if(getCandy(lX, y) -> getType() != NORMAL)
                specialCandy.push_back(getCandy(lX, y));
            else
                getCandy(lX, y)->setBreed(EMPTY);
            y ++;
        }
        break;
    default:
        printf("CandyBoard::deleteMatchInDirectionCoord Invalid direction passed!\n");
        break;
    }
    // if the cur_candy is a special candy?
    if(cur_candy->getType() != NORMAL)
        specialCandy.push_back(cur_candy);
    else
        cur_candy->setBreed(EMPTY);

    printf("Candy Size: %d\n", specialCandy.size());

    for(int i = 0; i < specialCandy.size(); i++)
    {
        deleteBySpecialCandy(specialCandy[i]);
    }
    printf("Number of candy deleted %d\n", number_of_candy_deleted);
    // update score
    // can be changed
    gScoreNumber += (number_of_candy_deleted + 1) * 40 * multiplier;
    // every match upgrade multiplier by 1
    multiplier += 1;
    return (number_of_candy_deleted + 1);
}

// delete the candy affected by special candy
int CandyBoard::deleteBySpecialCandy(Candy* lCandy)
{
    // number of candies deleted by t his candy
    int number_candy_deleted = 0;
    CandyBreed breed = lCandy -> getBreed();
    CandyType type = lCandy -> getType();
    int posX = getPositionX(lCandy -> getX());
    int posY = getPositionY(lCandy -> getY());
    // special candy in the way!
    std::vector<Candy*> special_candies;
    printf("X: %d Y: %d\n", posX, posY);
    // to avoid the dead end loop of recursion
    lCandy->setBreed(EMPTY);
    switch(type)
    {
    // delete vertically
    case STRIPED_V:
        {
            // loop the rows, keep the column
            // in other word, keep the x, loop the y
            // loop both the left and right at the same time to create vertical effect
            // everytime striped is matched -> immediately grant score
            int y_up, y_down;
            y_up = getPositionY(lCandy->getY()) - 1;
            y_down = getPositionY(lCandy->getY()) + 1;
            while(y_up >= 0 || y_down < mRows)
            {
                if(y_up >= 0)
                {
                    Candy* cur_candy = getCandy(posX, y_up);
                    // if its another special candy, call this function on it
                    // in case its already been deleted
                    if(cur_candy != lCandy)
                    {
                        if(cur_candy->getBreed() != EMPTY)
                            number_candy_deleted ++;
                        if(cur_candy->getType() != NORMAL)
                        {
                            special_candies.push_back(cur_candy);
                        }
                        // its color bomb
                        else if(cur_candy -> getBreed() == COLOUR_BOMB)
                        {
                            // turn into color than turn into normal first because deleteColor bomb requires the color
                            special_candies.push_back(cur_candy);
                        }
                        else
                        // else, just empty it
                            cur_candy->setBreed(EMPTY);
                    }
                    y_up --;
                }
                if(y_down < mRows)
                {
                    Candy* cur_candy = getCandy(posX, y_down);
                    // if its another special candy, call this function on it
                    // in case its already been deleted
                    if(cur_candy != lCandy)
                    {
                        if(cur_candy->getBreed() != EMPTY)
                            number_candy_deleted ++;
                        if(cur_candy->getType() != NORMAL)
                        {
                            special_candies.push_back(cur_candy);
                            //deleteBySpecialCandy(cur_candy);
                        }
                        // its color bomb
                        else if(cur_candy -> getBreed() == COLOUR_BOMB)
                        {
                            // turn into color than turn into normal first because deleteColor bomb requires the color
                            special_candies.push_back(cur_candy);
                        }
                        else
                        // else, just empty it
                            cur_candy->setBreed(EMPTY);
                    }
                    y_down ++;
                }
            }
            // play wrapped sound
            soundEffect[STRIPED_CANDY_BLAST].play();
            break;
        }

    case STRIPED_H:
        {
            // loop the x, keep the y
            int x_left, x_right;
            x_left = getPositionX(lCandy->getX()) - 1;
            x_right = getPositionX(lCandy->getX()) + 1;
            while(x_left >= 0 || x_right < mColumns)
            {
                if(x_left >= 0)
                {
                    Candy* cur_candy = getCandy(x_left, posY);
                    if(cur_candy != lCandy)
                    {
                        if(cur_candy->getBreed() != EMPTY)
                            number_candy_deleted ++;
                        if(cur_candy->getType() != NORMAL)
                        {
                            special_candies.push_back(cur_candy);
                            //deleteBySpecialCandy(cur_candy);
                        }
                        else if(cur_candy -> getBreed() == COLOUR_BOMB)
                        {
                            /*
                            lCandy->setBreed(breed);
                            lCandy->turnIntoNormal();
                            deleteByColourBomb(lCandy);
                            */
                            special_candies.push_back(cur_candy);
                        }
                        else
                            cur_candy->setBreed(EMPTY);
                    }
                    x_left --;
                }
                 if(x_right < mColumns)
                {
                    Candy* cur_candy = getCandy(x_right, posY);
                    if(cur_candy != lCandy)
                    {
                        if(cur_candy->getBreed() != EMPTY)
                            number_candy_deleted ++;
                        if(cur_candy->getType() != NORMAL)
                        {
                            special_candies.push_back(cur_candy);
                            //deleteBySpecialCandy(cur_candy);
                        }
                        else if(cur_candy -> getBreed() == COLOUR_BOMB)
                        {
                            /*
                            lCandy->setBreed(breed);
                            lCandy->turnIntoNormal();
                            deleteByColourBomb(lCandy);
                            */
                            special_candies.push_back(cur_candy);
                        }
                        else
                            cur_candy->setBreed(EMPTY);
                    }
                    x_right ++;
                }
            }
        soundEffect[STRIPED_CANDY_BLAST].play();
        break;
        }
    case WRAPPED:
        {
            gScoreNumber += 540;
            // used for 8 directions around
            int dx[3] = {-1, 0, 1};
            int dy[3] = {-1, 0, 1};
            for(int i = 0; i < 3; i++)
                for(int j = 0; j < 3; j++)
                {
                    int sx = dx[i];
                    int sy = dy[j];
                    // is it valid position
                    if(posX + sx >= 0 && posX + sx < mColumns && posY + sy >= 0 && posY + sy < mRows)
                    {
                        Candy* cur_candy = getCandy(posX + sx, posY + sy);
                        if(cur_candy != lCandy)
                        {
                            if(cur_candy->getBreed() != EMPTY)
                                number_candy_deleted ++;
                            if(cur_candy->getType() != NORMAL)
                            {
                                special_candies.push_back(cur_candy);
                                //deleteBySpecialCandy(cur_candy);
                            }
                            else if(cur_candy -> getBreed() == COLOUR_BOMB)
                            {
                                special_candies.push_back(cur_candy);
                            }
                            else
                                cur_candy->setBreed(EMPTY);
                        }
                    }
                }
            soundEffect[WRAPPED_CANDY_DENOTATION].play();
            break;
        }
    default:
        break;
    }
    SDL_RenderClear(gRenderer);
    renderFrame();
    displayCandyBoard(gRenderer);
    // display striped effect
    Texture effect;
    switch(type)
    {
    case STRIPED_H:
        effect.loadFromFile(gRenderer, std::string("img/cross_h.png"));
        effect.setWidth(mWidth);
        effect.setHeight(Candy::CandyHeight);
        // start at the start of the row
        effect.setX(mX);
        // the candy row
        effect.setY(posY);
        effect.render(gRenderer, mX, lCandy->getY());
        break;
    case STRIPED_V:
        effect.loadFromFile(gRenderer, std::string("img/cross_v.png"));
        effect.setWidth(Candy::CandyWidth);
        effect.setHeight(mHeight);
        // start at the start of the column
        effect.setX(posX);
        // the candy column
        effect.setY(mY);
        effect.render(gRenderer, lCandy->getX(), mY);
        break;
    }

    SDL_RenderPresent(gRenderer);
    delay(200);

    // deal with special candies separately
    for(int i = 0; i < special_candies.size(); i++)
    {
            Candy* tmp_candy = special_candies[i];
            if(tmp_candy -> getType() != NORMAL)
            {
                deleteBySpecialCandy(tmp_candy);
            }
            else if(tmp_candy -> getBreed() == COLOUR_BOMB)
            {
                lCandy->setBreed(breed);
                lCandy->turnIntoNormal();
                deleteByColourBomb(lCandy);
                tmp_candy->setBreed(EMPTY);
            }
    }
    // if it is a wrapped candy --> unwrapped it
    if(type == WRAPPED)
    {
        // if it is not an unwrapped candy -> which means only explodes for once
        // is2nd = false --> isUnwrapped = false
        if( lCandy -> isUnwrapped() == false)
        {
            lCandy -> setBreed(breed);
            // make is2nd = true
            lCandy -> makeUnwrapped();
        }
        else
        {
            // return to not unwrapped
            lCandy -> setBreed(EMPTY);
            lCandy -> makeNotUnwrapped();
        }
    }
    // update score
    gScoreNumber += number_candy_deleted * 60 * multiplier;
    // update multiplier
    multiplier += 1;
    return (number_candy_deleted);
}

int CandyBoard::deleteByColourBomb(Candy* lCandy)
{
    // candies directly affected
    int number_of_candy_deleted = 0;
    unsigned long long sum_candy_values = 0;
    // if swap with another colour bomb -> delete all
    if(lCandy -> getBreed() == COLOUR_BOMB)
    {
        printf("Colour bomb: X: %d Y: %d and multiplier: %d\n", getPositionX(lCandy->getX()), getPositionY(lCandy->getY()), multiplier);
        for(int i = 0; i < mRows; i++)
            for(int j = 0; j < mColumns; j++)
            {
                printf("Sum candy values: %d\n", sum_candy_values);
                number_of_candy_deleted += 1;
                Candy* tmp_candy = getCandy(j, i);
                switch(tmp_candy->getType())
                {
                case NORMAL:
                    sum_candy_values += 60 * multiplier;
                    break;
                case STRIPED_H: case STRIPED_V:
                    sum_candy_values += 120 * multiplier;
                    break;
                case WRAPPED:
                    sum_candy_values += 540 * multiplier;
                    break;
                }
                tmp_candy->setBreed(EMPTY);
                // display each candy being delted
                SDL_RenderClear(gRenderer);
                renderFrame();
                displayCandyBoard(gRenderer);
                SDL_RenderPresent(gRenderer);
                delay(30);
                // play colour bomb sound
                soundEffect[COLOUR_BOMB_DENOTATION].playOnce();
            }
        printf("Sum candy values: %d\n", sum_candy_values);
    }
    // if swap with striped -> transform all into striped then explode everything
    else if(lCandy -> getType() == STRIPED_H || lCandy -> getType() == STRIPED_V)
    {
        // to store exploding candy
        std::vector<Candy*> explode;
        // transform every candies with the same color to striped, either one
        for(int i = 0; i < mRows; i++)
            for(int j = 0; j < mColumns; j++)
            {
                Candy* tmp_candy = getCandy(j, i);
                if(tmp_candy->equal(lCandy) && tmp_candy -> getType() == NORMAL)
                {
                    // get the COLOUR_NORMAL
                    CandyBreed trans_breed = (CandyBreed) ((int) lCandy->getBreed() - (int) lCandy->getType() + (rand() % 2 + 1) );
                    tmp_candy->setBreed(trans_breed);
                    explode.push_back(tmp_candy);
                    // display after transforming
                    SDL_RenderClear(gRenderer);
                    renderFrame();
                    displayCandyBoard(gRenderer);
                    SDL_RenderPresent(gRenderer);
                    delay(30);
                }
            }
            // display after transformation
            SDL_RenderClear(gRenderer);
            renderFrame();
            displayCandyBoard(gRenderer);
            SDL_RenderPresent(gRenderer);
            delay(200);

        for(int i = 0; i < explode.size(); i++)
        {
            Candy* tmp_candy = explode[i];
            // if has not been deleted
            if(tmp_candy->getBreed() != EMPTY)
                // another check
                if(tmp_candy->getType() == STRIPED_H || tmp_candy -> getType() == STRIPED_V)
                {
                    number_of_candy_deleted += 1;
                    sum_candy_values += 120;
                    deleteBySpecialCandy(tmp_candy);
                    SDL_RenderClear(gRenderer);
                    renderFrame();
                    displayCandyBoard(gRenderer);
                    SDL_RenderPresent(gRenderer);
                    delay(200);

                }
        }
    }
    else if(lCandy -> getType() == WRAPPED)
    {
        // to store exploding candy
        std::vector<Candy*> explode;
        // transform every candies with the same color to striped, either one
        for(int i = 0; i < mRows; i++)
            for(int j = 0; j < mColumns; j++)
            {
                Candy* tmp_candy = getCandy(j, i);
                if(tmp_candy->equal(lCandy) && tmp_candy -> getType() == NORMAL)
                {
                    // get the COLOUR_NORMAL and add WRAPPED to it
                    CandyBreed trans_breed = (CandyBreed) ((int) lCandy->getBreed() - (int) lCandy->getType() + (int) WRAPPED );
                    tmp_candy->setBreed(trans_breed);
                    explode.push_back(tmp_candy);
                    // display after transforming
                    SDL_RenderClear(gRenderer);
                    renderFrame();
                    displayCandyBoard(gRenderer);
                    SDL_RenderPresent(gRenderer);
                    delay(30);
                }
            }
            // display after transformation
            SDL_RenderClear(gRenderer);
            renderFrame();
            displayCandyBoard(gRenderer);
            SDL_RenderPresent(gRenderer);
            delay(200);

        for(int i = 0; i < explode.size(); i++)
        {
            Candy* tmp_candy = explode[i];
            // if has not been deleted
            if(tmp_candy->getBreed() != EMPTY)
                // another check
                if(tmp_candy->getType() == WRAPPED)
                {
                    number_of_candy_deleted += 1;
                    sum_candy_values += 540;
                    deleteBySpecialCandy(tmp_candy);
                    SDL_RenderClear(gRenderer);
                    renderFrame();
                    displayCandyBoard(gRenderer);
                    SDL_RenderPresent(gRenderer);
                    delay(200);
                }
        }
    }
    else if(lCandy -> getBreed() != EMPTY)
    {
        // if lCandy is special candy -> not only delete all but also create special candy
        for(int i = 0; i < mRows; i++)
            for(int j = 0; j < mColumns; j++)
            {
                Candy* tmp_candy = getCandy(j, i);
                // not the candy and have the same colour
                if(tmp_candy != lCandy && tmp_candy->equal(lCandy))
                {
                    number_of_candy_deleted += 1;
                    switch(tmp_candy->getType())
                    {
                    case NORMAL:
                        sum_candy_values += 60 * multiplier;
                        break;
                    case STRIPED_H: case STRIPED_V:
                        sum_candy_values += 120 * multiplier;
                        break;
                    case WRAPPED:
                        sum_candy_values += 540 * multiplier;
                        break;
                    }
                    // it's special candy, use its effect
                    if(tmp_candy->getType() != NORMAL)
                        deleteBySpecialCandy(tmp_candy);
                    // else delete it
                    else
                    {
                        tmp_candy->setBreed(EMPTY);
                        // display being disappeared
                        SDL_RenderClear(gRenderer);
                        renderFrame();
                        displayCandyBoard(gRenderer);
                        SDL_RenderPresent(gRenderer);
                         // play colour bomb sound before delaying
                        soundEffect[COLOUR_BOMB_DENOTATION].playOnce();
                        delay(100);
                    }

                }
            }
    }
    if(lCandy->getType() != NORMAL)
        deleteBySpecialCandy(lCandy);
    else
        lCandy->setBreed(EMPTY);
    // update score
    gScoreNumber += (sum_candy_values * number_of_candy_deleted);
    // update multiplier
    multiplier += 1;
    return (number_of_candy_deleted);
}

int CandyBoard::deleteMatchInDirectionCoord(int cX, int cY, DIRECTION direction)
{
    int posX = (cX - mX) / Candy::CandyWidth;
    int posY = (cY - mY) / Candy::CandyHeight;
    return deleteMatchInDirection(posX, posY, direction);
}

void CandyBoard::displayCandyBoard(SDL_Renderer* lRenderer)
{
    for(int i = 0; i < mRows; i++)
        for(int j = 0; j < mColumns; j++)
        {
            //printf("%d %d\n", i, j);
            candies[i][j].render(lRenderer);
        }
}

void CandyBoard::fallingBoard(SDL_Renderer* lRenderer)
{
    std::vector< std::vector<Animation> > anime;
    anime.assign(mRows, std::vector<Animation>());

    for(int i = 0; i < mRows; i++)
        for(int j = 0; j < mColumns; j++)
        {
            Animation tmp_anime;
            tmp_anime.setTexture(getCandy(i ,j)->getTexture());
            // same column, start at minus Y
            tmp_anime.setPosition(getCandy(i, j)->getX(), getCandy(i, j)->getY() - getBoardHeight());
            tmp_anime.setDest(getCandy(i ,j)->getX(), getCandy(i ,j)->getY());
            anime[i].push_back(tmp_anime);
            printf("%d %d %d %d\n",i ,j, getCandy(i ,j)->getX(), getCandy(i ,j)->getY());
        }
    int falling = true;
    do
    {
        SDL_RenderClear(lRenderer);
        renderFrame();
        for(int i = 0; i < mRows; i++)
            for(int j = 0; j < mColumns; j++)
            {
                anime[i][j].fall(lRenderer);
                if(anime[i][j].getStatus() == STANDING_STILL)
                    falling = false;
            }
        SDL_RenderPresent(lRenderer);
    } while(falling);
    // play fall sound
    soundEffect[CANDY_LAND1].play();
}

int CandyBoard::getBoardWidth()
{
    return mWidth;
}

int CandyBoard::getBoardHeight()
{
    return mHeight;
}

int CandyBoard::getX()
{
    return mX;
}

int CandyBoard::getY()
{
     return mY;
}

// Get candy at coordinate (x,y)
Candy* CandyBoard::getCandy(int x, int y)
{
    return (Candy*)(&candies[y][x]);
}

Candy* CandyBoard::getCandyCoord(int x, int y)
{
    int posX = (x - mX) / Candy::CandyWidth;
    int posY = (y - mY) / Candy::CandyHeight;
    return (Candy*)(&candies[posY][posX]);
}

int CandyBoard::getPositionX(int cx)
{
    return (cx - mX) / Candy::CandyWidth;
}

int CandyBoard::getPositionY(int cy)
{
    return (cy - mY) / Candy::CandyHeight;
}
