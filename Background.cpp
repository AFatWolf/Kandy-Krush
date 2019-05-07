#ifndef BASIC_LIB
#define BASIC_LIB
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#endif // BASIC_LIB

#include <time.h>

#ifndef HEADER_H
#define HEADER_H 1
#include "Headers.h"
#endif // HEADER_H

#include "Background.h"

Background::Background()
{
    numberOfBackground = 0;
    // for changing background
    background.assign(gNumberOfBackground, Texture());
    mStartTime = 0;
}

Background::~Background()
{
    for(int i = 0; i < background.size(); i++)
    {
        background[i].free();
    }
}

void Background::addBackgroundFromFile(std::string pathToFile)
{


    numberOfBackground += 1;
    int last = numberOfBackground - 1;
    background[last].loadFromFile(gRenderer, pathToFile);
    background[last].setWidth(SCREEN_WIDTH);
    background[last].setHeight(SCREEN_HEIGHT);
    // if it is the first one
    if(numberOfBackground == 1)
    {
        background[last].setAlpha(0xFF);
        mStartTime = time(0);
    }
    // invisible first if not the first one
    else
        background[last].setAlpha(0);

}

void Background::display()
{
    int loop = 5;
    // per frame
    time_t counter = time(0) - mStartTime;
    int whichToDisplay = ((counter / loop) % numberOfBackground);
    // we will display everything but set their alpha to 0 and only set the chosen one to full alpha
    // only the one which will be changed has the alpha more than 0
    // see if we are going to change background
    //background[whichToDisplay].setAlpha(0xFF);
    //background[(whichToDisplay + numberOfBackground - 1)  % numberOfBackground].setAlpha(0);

    if(counter + 10 > ((counter + 1) / loop) * loop)
    {
        int fadeSpeed = 8;
        int nextToDisplay = (whichToDisplay + 1) % numberOfBackground;
        int whichAlpha = background[whichToDisplay].getAlpha();
        int nextAlpha = background[nextToDisplay].getAlpha();
        // fade out
        background[whichToDisplay].setAlpha(std::max(whichAlpha - fadeSpeed, 0));
        // more clear
        background[nextToDisplay].setAlpha(std::min(nextAlpha + fadeSpeed, 0xFF));
    }

    for(int i = 0; i < numberOfBackground; i++)
    {
        if(background[i].getAlpha() != 0)
            background[i].render(gRenderer, 0, 0);
        //std::cout << background[i].pathToFile << std::endl;
    }
}
