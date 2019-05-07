#ifndef TEXTURE_H
#define TEXTURE_H
#include "Texture.h"
#endif // TEXTURE_H

#ifndef SDL_TTF_H
#define SDL_TTF_H 1
#include <SDL_ttf.h>
#endif // SDL_TTF_H

#ifndef SDL_MIXER_H
#define SDL_MIXER_H 1
#include <SDL_mixer.h>
#endif // SDL_MIXER_H

#ifndef AUDIO_H
#define AUDIO_H
#include "Audio.h"
#endif // AUDIO_H

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int CANDY_BOARD_START_X;
extern int CANDY_BOARD_START_Y;
const int FALLING_ACCELERATOR = 1;
extern Texture* mCandyTexture;
extern SDL_Renderer* gRenderer;
extern void renderFrame();
extern int ANIMATION_MODE;
extern int gNumberOfBackground;
extern TTF_Font* gCandiceFont;
extern unsigned long long gScoreNumber;
extern int gMoves;

enum Sound
{
    ONE_STAR,
    TWO_STAR,
    THREE_STAR,
    BOMB_SOUND,
    CANDY_LAND1,
    CANDY_LAND2,
    CANDY_LAND3,
    CANDY_LAND4,
    COLOUR_BOMB_AND_COLOUR_BOMB,
    COLOUR_BOMB_CREATE,
    COLOUR_BOMB_DENOTATION,
    COMBO_SOUND1,
    COMBO_SOUND2,
    COMBO_SOUND3,
    COMBO_SOUND4,
    COMBO_SOUND5,
    COMBO_SOUND6,
    COMBO_SOUND7,
    COMBO_SOUND8,
    COMBO_SOUND9,
    COMBO_SOUND10,
    COMBO_SOUND11,
    COMBO_SOUND12,
    NEGATIVE_SWITCH,
    NORMAL_SWITCH,
    STRIPED_CANDY_BLAST,
    STRIPED_CANDY_CREATE,
    WRAPPED_CANDY_CREATED,
    WRAPPED_CANDY_DENOTATION,
    NUMBER_OF_SOUND
};

extern Audio soundEffect[NUMBER_OF_SOUND];

enum Voice
{
    SWEET,
    TASTY,
    DELICIOUS,
    DIVINE,
    SUGAR_CRUSH,
    NUMBER_OF_VOICE
};

extern Audio oldManVoice[NUMBER_OF_VOICE];
extern Texture oldManTexture[NUMBER_OF_VOICE];
