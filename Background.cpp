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
        mStartTime = 0;
    }
    // invisible first if not the first one
    else
        background[last].setAlpha(0);

}

void Background::display()
{
    int loop = 5000;
    // per frame
    unsigned int counter = SDL_GetTicks();
    // change background every loop milisec
    unsigned int whichToDisplay = ((counter - mStartTime) / loop) % numberOfBackground;
    unsigned int nextToDisplay = (whichToDisplay + 1) % numberOfBackground;
    int whichAlpha = background[whichToDisplay].getAlpha();
    int nextAlpha = background[nextToDisplay].getAlpha();
    //printf("which: %d counter %d start: %d\n", whichToDisplay, counter, mStartTime);
    // we will display everything but set their alpha to 0 and only set the chosen one to full alpha
    // only the one which will be changed has the alpha more than 0
    // see if we are going to change background

    if(counter + 1000 > ((counter + loop) / loop) * loop)
    {
        int fadeSpeed = 12;

        // fade out
        background[whichToDisplay].setAlpha(std::max(whichAlpha - fadeSpeed, 0));
        // more clear
        // background[nextToDisplay].setAlpha(std::min(nextAlpha + fadeSpeed, 0xFF));
        if(background[nextToDisplay].getAlpha() != 0xFF)
            background[nextToDisplay].setAlpha(0xFF);
    }
    /*
    if(nextAlpha != 0)
        background[nextToDisplay].render(gRenderer, 0, 0);
    if(whichAlpha != 0)
        background[whichToDisplay].render(gRenderer, 0, 0);
    */
     for(int i = numberOfBackground; i >= 0; i--)
    //for(int i = 0; i < numberOfBackground; i++)
    {
        int displayBackground = i % numberOfBackground;

        // last background we do not want the [0] to display but instead the [numberOfBackground]
        if(i == 0 && background[numberOfBackground - 1].getAlpha() != 0)
            continue;

        if(background[displayBackground].getAlpha() != 0)
            background[displayBackground].render(gRenderer, 0, 0);
        //std::cout << background[i].pathToFile << std::endl;
    }
}
