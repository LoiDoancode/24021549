#ifndef _DEFS__H
#define _DEFS__H

const int SCREEN_WIDTH = 1020;
const int SCREEN_HEIGHT = 800;
const char* WINDOW_TITLE = "Hello World!";

const char* MAN_SPRITE_FILE = "spaceship.png";
const int MAN_CLIPS[][4] = {
    { 50, 10, 300, 340 },
    { 395,10, 300, 340},
    { 734, 10, 300, 340 },
    { 1075, 10, 300, 340 },
    { 734, 10, 300, 340 },
    { 395,10, 300, 340},
   };
const int MAN_FRAMES = sizeof(MAN_CLIPS)/sizeof(int)/4;

const char* RECTS= "skulls.png";
const int RECTS_CLIPs[][4]={
{0,0,800,760},
{800,0,800,760}
};
const int RECTS_FRAMES= sizeof(RECTS_CLIPs)/sizeof(int)/4;

const char* BULLETS= "butletP.png";
const int BULLETS_CLIPs[][4]={
    {160,140,80,170},
    {560,140,80,170},
    {960,140,80,170},
    {1360,140,80,170}
};
const int BULLETS_FRAMES= sizeof(BULLETS_CLIPs)/sizeof(int)/4;
const int STAR_FRAMES = 2;
const SDL_Rect STAR_CLIPS[] = {
    {0, 0, 600, 600},  // Frame 1: Ngôi sao lớn
    {600, 0, 600, 600}  // Frame 2: Ngôi sao nhỏ
};
#endif
