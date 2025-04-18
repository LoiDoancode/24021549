#ifndef _CHECK__H
#define _CHECK__H
bool checkGameOver(const Block& block, int manX, int manY) {
    return (block.x < manX + 180 && block.x + block.width > manX &&
            block.y < manY + 195 && block.y + block.height > manY) ||
           (block.y >= SCREEN_HEIGHT);
}
#endif // _CHECK__H
