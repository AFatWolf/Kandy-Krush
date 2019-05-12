#ifndef BASIC_LIB
#define BASIC_LIB
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#endif // BASIC_LIB

#include <sstream>

#ifndef SDL_TTF_H
#define SDL_TTF_H 1
#include <SDL_ttf.h>
#endif // SDL_TTF_H

#ifndef SDL_MIXER_H
#define SDL_MIXER_H 1
#include <SDL_mixer.h>
#endif // SDL_MIXER_H

#ifndef TEXTURE_H
#define TEXTURE_H
#include "Texture.h"
#endif // TEXTURE_H

#ifndef CANDY_H
#define CANDY_H
#include "Candy.h"
#endif // CANDY_H

#ifndef CANDY_BOARD_H
#define CANDY_BOARD_H
#include "CandyBoard.h"
#endif // CANDY_BOARD_H

#ifndef HEADER_H
#define HEADER_H 1
#include "Headers.h"

#endif // HEADER_H

#ifndef BACKGROUND_H
#define BACKGROUND_H 1
#include "Background.h"
#endif // BACKGROUND_H

// can be changed to full-screen size later
int SCREEN_WIDTH = 1170;
int SCREEN_HEIGHT = 650;
int CANDY_BOARD_START_X;
int CANDY_BOARD_START_Y;
int CANDY_BOARD_WIDTH;
int CANDY_BOARD_HEIGHT;

int SCORE_BOARD_START_X = 70;
int SCORE_BOARD_START_Y = 100;
int MOVES_BOARD_START_X;
int MOVES_BOARD_START_Y;

int gNumberOfBackground = 6;

int ANIMATION_MODE = false;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
Texture* mCandyTexture = NULL;
TTF_Font* gCandiceFont = NULL;
unsigned long long gScoreNumber = 0;
int gMoves = 0;
Texture gScoreTexture;
Texture gMovesTexture;
Background gBackground;
Texture score_board;
Texture moves_board;
Texture candy_board;
Texture candy_board_background;
Audio soundEffect[NUMBER_OF_SOUND];
Audio oldManVoice[NUMBER_OF_VOICE];
Texture oldManTexture[NUMBER_OF_VOICE];

enum Screen
{
    START_SCREEN,
    LOSING_END_SCREEN,
    WINNING_END_SCREEN
};

bool init();

bool loadAudio();

bool loadMedia();

Texture* loadCandyTexture();

void close();

void delay(int miliseconds);

void startAndEndScreen();

bool startGame();

bool init()
{
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        if(!SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!\n");
        }

        gWindow = SDL_CreateWindow("Kandy Krush", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(!gWindow)
        {
            printf("Window cannot be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            SDL_GetWindowSize(gWindow, &SCREEN_WIDTH, &SCREEN_HEIGHT);
            printf("SCREEN WIDTH: %d SCREEN HEIGHT %d\n", SCREEN_WIDTH, SCREEN_HEIGHT);

            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if(!gRenderer)
            {
                printf("Renderer cannot be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
                // if the imgFlags is turned on correctly
                if( !(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not be initialized! SDL Error: %s\n", SDL_GetError());
                    success = false;
                }

                // init TTF
                if(TTF_Init() == -1)
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }

                // init audio
                if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                {
                    printf("SDL_mixer cannot initialize! SDL_mixer Error: %s\n", Mix_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}

void renderFrame()
{
    gBackground.display();
    candy_board.setAlpha(200);
    candy_board.render(gRenderer, CANDY_BOARD_START_X, CANDY_BOARD_START_Y);
    score_board.setAlpha(230);
    score_board.render(gRenderer, SCORE_BOARD_START_X, SCORE_BOARD_START_Y);
    gScoreTexture.render(gRenderer, SCORE_BOARD_START_X + 10, SCORE_BOARD_START_Y + 10);
    //gMovesTexture.render(gRenderer, SCORE_BOARD_START_X + 10, SCORE_BOARD_START_Y * 2);

    Texture score_number_texture;
    std::stringstream score;
    SDL_Color textColor = {255, 255, 255};
    score << gScoreNumber;
    score_number_texture.loadFromRenderedText(gRenderer, score.str(), textColor, gCandiceFont);
    score_number_texture.render(gRenderer, SCORE_BOARD_START_X + 10 + 10, SCORE_BOARD_START_Y + 10 + 65);

    moves_board.render(gRenderer, MOVES_BOARD_START_X, MOVES_BOARD_START_Y);
    gMovesTexture.render(gRenderer, MOVES_BOARD_START_X + 10, MOVES_BOARD_START_Y + 10);
    Texture moves_number_texture;
    std::stringstream moves;
    moves << gMoves;
    moves_number_texture.loadFromRenderedText(gRenderer,"    " + moves.str(), textColor, gCandiceFont);
    moves_number_texture.render(gRenderer, MOVES_BOARD_START_X + 10 + 10, MOVES_BOARD_START_Y + 10 + 65);
}

bool loadAudio()
{

    bool success = true;
    std::vector<std::string> path;
    path.assign(NUMBER_OF_SOUND, std::string());
    std::string audio_folder = "audio/";
    for(int i = 0;i < NUMBER_OF_SOUND; i++)
    {
        switch(i)
        {
            case ONE_STAR:
                path[i] = "1_star.wav";
                break;
            case TWO_STAR:
                path[i] = "2_star.wav";
                break;
            case THREE_STAR:
                path[i] = "3_star.wav";
                break;
            case BOMB_SOUND:
                path[i] = "Bomb_sound1.wav";
                break;
            case CANDY_LAND1:
                path[i] = "Candy_land1.wav";
                break;
            case CANDY_LAND2:
                path[i] = "Candy_land2.wav";
                break;
            case CANDY_LAND3:
                path[i] = "Candy_land3.wav";
                break;
            case CANDY_LAND4:
                path[i] = "Candy_land4.wav";
                break;
            case COLOUR_BOMB_AND_COLOUR_BOMB:
                path[i] = "Colour_bomb_and_colour_bomb.wav";
                break;
            case COLOUR_BOMB_CREATE:
                path[i] = "Colour_bomb_created.wav";
                break;
            case COLOUR_BOMB_DENOTATION:
                path[i] = "Colour_bomb_denotation.wav";
                break;
            case COMBO_SOUND1:
                path[i] = "Combo_sound1.wav";
                break;
            case COMBO_SOUND2:
                path[i] = "Combo_sound2.wav";
                break;
            case COMBO_SOUND3:
                path[i] = "Combo_sound3.wav";
                break;
            case COMBO_SOUND4:
                path[i] = "Combo_sound4.wav";
                break;
            case COMBO_SOUND5:
                path[i] = "Combo_sound5.wav";
                break;
            case COMBO_SOUND6:
                path[i] = "Combo_sound6.wav";
                break;
            case COMBO_SOUND7:
                path[i] = "Combo_sound7.wav";
                break;
            case COMBO_SOUND8:
                path[i] = "Combo_sound8.wav";
                break;
            case COMBO_SOUND9:
                path[i] = "Combo_sound9.wav";
                break;
            case COMBO_SOUND10:
                path[i] = "Combo_sound10.wav";
                break;
            case COMBO_SOUND11:
                path[i] = "Combo_sound11.wav";
                break;
            case COMBO_SOUND12:
                path[i] = "Combo_sound12.wav";
                break;
            case NEGATIVE_SWITCH:
                path[i] = "Negative_switch.wav";
                break;
            case NORMAL_SWITCH:
                path[i] = "Normal_switch.wav";
                break;
            case STRIPED_CANDY_BLAST:
                path[i] = "Striped_candy_blasted.wav";
                break;
            case STRIPED_CANDY_CREATE:
                path[i] = "Striped_candy_created1.wav";
                break;
            case WRAPPED_CANDY_CREATED:
                path[i] = "Wrapped_candy_created1.wav";
                break;
            case WRAPPED_CANDY_DENOTATION:
                path[i] = "Wrapped_candy_denotation.wav";
                break;
        }
    }

    for(int i = 0; i < NUMBER_OF_SOUND; i++)
    {
        if(!soundEffect[i].loadFromFile((audio_folder + path[i])))
        {
            printf("Fail to load sound effect: %s! SDL_mixer Error:%s\n", path[i].c_str(), Mix_GetError());
            success = false;
        }
    }

    for(int i = 0; i < NUMBER_OF_VOICE; i++)
    {
        std::string pathToVoice;
        switch(i)
        {
        case SWEET:
            pathToVoice = "Sweet.wav";
            break;
        case TASTY:
            pathToVoice = "Tasty.wav";
            break;
        case DELICIOUS:
            pathToVoice = "Delicious.wav";
            break;
        case DIVINE:
            pathToVoice = "Divine.wav";
            break;
        case SUGAR_CRUSH:
            pathToVoice = "Sugar Crush.wav";
            break;
        default:
            pathToVoice = "Sweet.wav";
            break;
        }
        if(!oldManVoice[i].loadFromFile(audio_folder + pathToVoice))
        {
            printf("Unable to load file: %s! Mixer Error: %s\n", pathToVoice.c_str(), Mix_GetError());
            success = false;
        }
    }
    return success;
}

bool loadMedia()
{
    bool success = true;
    ANIMATION_MODE = false;
    mCandyTexture = loadCandyTexture();
    if(!mCandyTexture)
    {
        printf("Cannot generate candy texture!\n");
    }

    // init candice font
    std::string candice_font_file = "CANDY.TTF";
    gCandiceFont = TTF_OpenFont(candice_font_file.c_str(), 65);
    if(gCandiceFont == NULL)
    {
        printf("Failed to load Candy Font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    SDL_Color textColor = {0xFF, 0xFF, 0xFF};
    if(!gScoreTexture.loadFromRenderedText(gRenderer, "Score:", textColor, gCandiceFont))
    {
        printf("Unable to load Score text!\n");
        success = false;
    }
    if(!gMovesTexture.loadFromRenderedText(gRenderer, "Moves:", textColor, gCandiceFont))
    {
        printf("Unable to load Moves text!\n");
        success = false;
    }


    // get candyboard info
    CandyBoard CandyBoard;
    CANDY_BOARD_START_X = CandyBoard.getX();
    CANDY_BOARD_START_Y = CandyBoard.getY();
    CANDY_BOARD_WIDTH = CandyBoard.getBoardWidth();
    CANDY_BOARD_HEIGHT = CandyBoard.getBoardHeight();

    for(int i = 1; i <= gNumberOfBackground; i++)
    {
        std::stringstream number;
        number << i;
        std::string background_file = "img/CandyKingdomBackground" + number.str() + ".png";
        std::cout << background_file << std::endl;
        gBackground.addBackgroundFromFile(background_file);
    }

    std::string candy_board_file = "img/CandyBoard.png";
    candy_board.loadFromFile(gRenderer, candy_board_file);
    candy_board.setX(CANDY_BOARD_START_X);
    candy_board.setY(CANDY_BOARD_START_Y);

    std::string score_board_file = "img/ScoreBoard.png";
    score_board.loadFromFile(gRenderer, score_board_file);
    score_board.setX(SCORE_BOARD_START_X);
    score_board.setY(SCORE_BOARD_START_Y);

    std::string moves_board_file = score_board_file;
    moves_board.loadFromFile(gRenderer, moves_board_file);
    MOVES_BOARD_START_X = SCORE_BOARD_START_X;
    MOVES_BOARD_START_Y = SCORE_BOARD_START_Y + score_board.getHeight() + 50;
    moves_board.setX(MOVES_BOARD_START_X);
    moves_board.setY(MOVES_BOARD_START_Y);

    // load old man voice and audio
    success &= loadAudio();
    for(int i = 0; i < NUMBER_OF_VOICE; i++)
    {
        std::string pathToVoice;
        std::string imgFolder = "img/oldMan/";
        switch(i)
        {

        case SWEET:
            pathToVoice = "Sweet.png";
            break;
        case TASTY:
            pathToVoice = "Tasty.png";
            break;
        case DELICIOUS:
            pathToVoice = "Delicious.png";
            break;
        case DIVINE:
            pathToVoice = "Divine.png";
            break;
        case SUGAR_CRUSH:
            pathToVoice = "Sugar Crush.png";
            break;
        default:
            pathToVoice = "";
            break;
        }
        oldManTexture[i].loadFromFile(gRenderer, (imgFolder + pathToVoice).c_str());
    }


    return success;
}

Texture* loadCandyTexture()
{
    Texture* newCandyTexture = new Texture[NUMBER_OF_CANDY_TYPES];
    std::string candyFolder = "img/Candies/";
    std::string path[NUMBER_OF_CANDY_TYPES];
    for(int i = 0; i < NUMBER_OF_CANDY_TYPES; i++){
        // candy_color
        int candy_color = (i / 4);
        // candy_type
        int candy_type = i - candy_color * 4;
        //std::cout << "Color: " << candy_color << " Type:" << candy_type << std::endl;
        if(i == COLOUR_BOMB)
        {
            path[i] = candyFolder + "Colour_Bomb.png";
        }
        else if (i == EMPTY)
        {
            path[i] = candyFolder + "Blue.png";
        }
        else
        {
            path[i] = candyFolder;
            switch(candy_color)
            {
            // why also EMPTY? down below
            case BLUE:
                path[i] += "Blue";
                break;
            case GREEN:
                path[i] += "Green";
                break;
            case ORANGE:
                path[i] += "Orange";
                break;
            case PURPLE:
                path[i] += "Purple";
                break;
            case RED:
                path[i] += "Red";
                break;
            case YELLOW:
                path[i] += "Yellow";
                break;
            default:
                path[i] = "Blue";
                break;
            }
            switch(candy_type)
            {
            case NORMAL:
                path[i] += "";
                break;
            case STRIPED_V:
                path[i] += "_Striped_V";
                break;
            case STRIPED_H:
                path[i] += "_Striped_H";
                break;
            case WRAPPED:
                path[i] += "_Wrapped";
                break;
            default:
                path[i] += "";
                break;
            }
            path[i] += ".png";
        }
    }

    for(int i = 0; i < NUMBER_OF_CANDY_TYPES; i++){
        //std::cout << i << " Path: " << path[i] << std::endl;
        if(!newCandyTexture[i].loadFromFile(gRenderer, path[i]))
        {
            printf("Failed to load candy texture %s\n", path[i].c_str());
        }
        // so instead of creating an empty texture for EMPTY, we take an arbitrary candy and set its alpha to 0
        if(i == EMPTY)
        {
            newCandyTexture[i].setAlpha(0x00);
        }
    }
    return newCandyTexture;
}

void close()
{
    for(int i = 0; i < NUMBER_OF_CANDY_TYPES; i++)
    {
        mCandyTexture[i].free();
    }

    for(int i = 0; i < NUMBER_OF_SOUND; i++)
    {
        soundEffect[i].free();
    }

    for(int i = 0; i < NUMBER_OF_VOICE; i++)
    {
        oldManTexture[i].free();
    }


    TTF_CloseFont(gCandiceFont);
    gCandiceFont = NULL;

    SDL_DestroyRenderer( gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow( gWindow);
    gWindow = NULL;

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

// wait for miliseconds
void delay(int miliseconds)
{
    miliseconds = (unsigned int ) miliseconds;
    unsigned int startTime = SDL_GetTicks();
    unsigned int currentTime;
    do
    {
        currentTime = SDL_GetTicks();
        SDL_PumpEvents();
    } while(currentTime - startTime < miliseconds);
    SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);
    SDL_FlushEvent(SDL_MOUSEMOTION);
    SDL_FlushEvent(SDL_MOUSEBUTTONUP);
}

void startAndEndScreen(Screen screen_type)
{
    Texture start_screen;
    std::string path;
    switch(screen_type)
    {
    case START_SCREEN:
        path = "img/Slogan/StartGame.png";
        break;
    case LOSING_END_SCREEN:
        path = "img/Slogan/YouFail.png";
        break;
    case WINNING_END_SCREEN:
        path = "img/Slogan/YouWin.png";
        break;
    }
    int start_y = SCREEN_HEIGHT / 3;
    int start_x = 0;
    start_screen.loadFromFile(gRenderer, path);
    SDL_RenderClear(gRenderer);
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = SCREEN_WIDTH;
    rect.h = SCREEN_HEIGHT;
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gRenderer, 0x09, 0x09, 0x09, 150);

    // walking from the left to right
    Animation drop;
    drop.setTexture(&start_screen);
    drop.setDest(start_x, start_y);
    drop.setPosition(0, - start_screen.getY());
    do
    {
        SDL_RenderClear(gRenderer);
        renderFrame();
        SDL_RenderFillRect(gRenderer, &rect);
        drop.fall(gRenderer);
        SDL_RenderPresent(gRenderer);
        delay(10);
    } while(drop.getStatus() != STANDING_STILL);

    SDL_RenderClear(gRenderer);
    renderFrame();
    // display a black rectangle

    SDL_RenderFillRect(gRenderer, &rect);
    start_screen.render(gRenderer, start_x, start_y);
    SDL_RenderPresent(gRenderer);
    delay(2000);
}

bool startGame()
{
    bool quit = false;
    SDL_Event e;
    CandyBoard candyBoard;

    // reset
    gScoreNumber = 0;
    gMoves = 40;

    startAndEndScreen(START_SCREEN);
    // display falling board
    candyBoard.fallingBoard(gRenderer);
    while(!quit && gMoves > 0)
    {
        while(!SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                quit = true;
            }

            SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF);

            candyBoard.handleEvent(&e);
            if(!ANIMATION_MODE)
            {
                SDL_RenderClear( gRenderer);
                renderFrame();
                candyBoard.displayCandyBoard(gRenderer);
            }
            SDL_RenderPresent( gRenderer);

        }
    }
    if(quit == false)
    {
        if(gScoreNumber >= 60000)
        {
            // sugar crush
            // display sugar crush texture falling down
            int old_man_start_x = SCREEN_WIDTH / 8;
            int old_man_start_y = SCREEN_HEIGHT / 3;
            oldManVoice[SUGAR_CRUSH].play();
            Animation drop;
            drop.setTexture(&oldManTexture[SUGAR_CRUSH]);
            drop.setPosition(old_man_start_x, 0);
            drop.setDest(old_man_start_x, old_man_start_y);
            do
            {
                SDL_RenderClear(gRenderer);
                renderFrame();
                candyBoard.displayCandyBoard(gRenderer);
                drop.fall(gRenderer);
                SDL_RenderPresent(gRenderer);
                delay(10);
            } while(drop.getStatus() != STANDING_STILL);
            delay(1000);

            // display board before
            SDL_RenderClear(gRenderer);
            renderFrame();
            candyBoard.displayCandyBoard(gRenderer);
            SDL_RenderPresent(gRenderer);
            delay(200);

            // start to sugar crush
            candyBoard.sugarCrush();
            delay(2000);

            startAndEndScreen(WINNING_END_SCREEN);
        }
        else
        {
            startAndEndScreen(LOSING_END_SCREEN);
        }
    }
    return !quit;
}

int main(int argc, char* args[])
{
    if(!init() ){
        printf("Failed to load media!\n");
    }
    else
    {
        if(!loadMedia())
        {
            printf("Failed to load media!\n");
        }
        else
        {

            mCandyTexture = loadCandyTexture();
            while(startGame() == true)
            {
                // pass;
            };

        }
    }
}
