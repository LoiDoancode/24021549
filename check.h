#ifndef _CHECK__H
#define _CHECK__H
#include "gift.h"
#include "block.h"
bool checkGameOver(const Block& block, int manX, int manY) {

    if(block.x < manX + 100 && block.x > manX + 20 && block.y < manY + 18 && block.y > manY - 15)
        return true;

    if(block.x < manX + 140 && block.x > manX - 10 && block.y < manY + 50 && block.y > manY + 18)
        return true;

    if((block.x < manX + 150 && block.x > manX - 20 && block.y < manY + 110 && block.y > manY + 55) || (block.y >= SCREEN_HEIGHT))
        return true;

    if(block.x < manX + 125 && block.x > manX +5 && block.y < manY + 125 && block.y > manY + 110)
        return true;
    return false;

}

#endif
