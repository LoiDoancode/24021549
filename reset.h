#ifndef _RESET__H
#define _RESET__H
#include "block.h"
#include "bullets.h"
using namespace std;
    void resetGame(vector<Bullet>& bullets, vector<Block>& blocks, int& score, int& blockSpeed, int& frameCount, int& manX, int& manY, int& manSpeedX, int& manSpeedY) {
    bullets.clear();
    blocks.clear();
    score = 0;
    blockSpeed = 3;
    frameCount = 0;
    manX = SCREEN_WIDTH / 2 - 90;
    manY = SCREEN_HEIGHT - 195;
    manSpeedX = 0;
    manSpeedY = 0;
}
#endif
