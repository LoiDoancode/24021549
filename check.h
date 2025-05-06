#ifndef _CHECK__H
#define _CHECK__H
bool checkGameOver(const Block& block, int manX, int manY) {
    return (block.x < manX + 150 && block.x + block.width > manX+30 &&
            block.y < manY + 140 && block.y + block.height > manY+80) ||
           (block.y >= SCREEN_HEIGHT);
}
#endif
