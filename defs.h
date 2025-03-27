
#ifndef _DEFS__H
#define _DEFS__H

const int SCREEN_WIDTH = 1020;
const int SCREEN_HEIGHT = 800;
const char* WINDOW_TITLE = "Hello World!";


const char* MAN_SPRITE_FILE = "New.png";
const int MAN_CLIPS[][4] = {
    { 170, 260, 290, 280 },
    { 770,260, 290, 280}
   };
const int MAN_FRAMES = sizeof(MAN_CLIPS)/sizeof(int)/4;

const char*  BIRD_SPRITE_FILE = "83127-sprite-area-line-animated-bird-film.png";
const int BIRD_CLIPS[][4] = {
    {0, 0, 182, 168},
    {181, 0, 182, 168},
    {364, 0, 182, 168},
    {547, 0, 182, 168},
    {728, 0, 182, 168},

    {0, 170, 182, 168},
    {181, 170, 182, 168},
    {364, 170, 182, 168},
    {547, 170, 182, 168},
    {728, 170, 182, 168},

    {0, 340, 182, 168},
    {181, 340, 182, 168},
    {364, 340, 182, 168},
    {547, 340, 182, 168}
};
const int BIRD_FRAMES = sizeof(BIRD_CLIPS)/sizeof(int)/4;

const char* RECTS= "skulls.png";
const int RECTS_CLIPs[][4]={
{0,0,800,800},
{800,0,800,800}
};
const int RECTS_FRAMES= sizeof(RECTS_CLIPs)/sizeof(int)/4;

const char* BULLETS= "butletP.png";
const int BULLETS_CLIPs[][4]={
    {160,135,80,170},
    {560,135,80,170},
    {960,135,80,170},
    {1360,135,80,170}
};
const int BULLETS_FRAMES= sizeof(BULLETS_CLIPs)/sizeof(int)/4;
#endif
