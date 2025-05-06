#ifndef _CHECK__H
#define _CHECK__H
bool checkGameOver(const Block& block, int manX, int manY) {
    return (block.x < manX + 180 && block.x + block.width > manX &&
            block.y < manY + 160 && block.y + block.height > manY+80) ||
           (block.y >= SCREEN_HEIGHT+80);
}
#endif
