#ifndef _DEFS__H
#define _DEFS__H

const int SCREEN_WIDTH = 1020;
const int SCREEN_HEIGHT = 800;
const char* WINDOW_TITLE = "Hello World!";

const char* MAN_SPRITE_FILE = "spaceship.png";
const int MAN_CLIPS[][4] = {
    { 50, 10, 297, 340 },
    { 395,10, 297, 340},
    { 734, 10, 297, 340 },
    { 1075, 10, 297, 340 },
    { 734, 10, 297, 340 },
    { 395,10, 297, 340},
};
const int MAN_FRAMES = sizeof(MAN_CLIPS)/sizeof(int)/4;

const char* RECTS= "skulls.png";
const int RECTS_CLIPs[][4]={
{165,25,485,485},
{965,25,485,485}
};
const int RECTS_FRAMES= sizeof(RECTS_CLIPs)/sizeof(int)/4;

const char* BULLETS= "bullet.png";
const int BULLETS_CLIPs[][4]={
    {165,140,65,170},
    {565,140,65,170},
    {965,140,65,170},
    {1365,140,65,170}
};
const int BULLETS_FRAMES= sizeof(BULLETS_CLIPs)/sizeof(int)/4;

const char* GIFT_FILE = "gift.png"; // File ảnh cho Gift
const int GIFT_WIDTH = 50; // Kích thước Gift
const int GIFT_HEIGHT = 50;
const int GIFT_SPEED = 3; // Tốc độ di chuyển xuống của Gift

const int STAR_FRAMES = 2;
const SDL_Rect STAR_CLIPS[] = {
    {0, 0, 600, 600},  // Frame 1: Ngôi sao lớn
    {600, 0, 600, 600}  // Frame 2: Ngôi sao nhỏ
};

#endif
