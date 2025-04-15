#ifndef _CHECK__H
#define _CHECK_H
bool checkGameOver(const Block& block, int manX, int manY) {
    return (block.x < manX + 64 && block.x + block.width > manX &&
            block.y < manY + 64 && block.y + block.height > manY) ||
           (block.y >= SCREEN_HEIGHT);
}
#endif // _CHECK__H

