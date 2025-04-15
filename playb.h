#ifndef _PLAYP__H
#define _PLAYP__H
#include "block.h"
#include "bullets.h"
using namespace std;
void playb(vector<Bullet>&bullets, vector<Block>& blocks, SDL_Texture* blockTexture){
        manX += manSpeedX;
        manY += manSpeedY;
        if (manX < 0) manX = 0;
        if (manX + 64 > SCREEN_WIDTH) manX = SCREEN_WIDTH - 64;
        if (manY < 0) manY = 0;
        if (manY + 64 > SCREEN_HEIGHT) manY = SCREEN_HEIGHT - 64;
        for (auto it = bullets.begin(); it != bullets.end(); ) {
            it->update();
            bool bulletRemoved = false;
            for (auto &block : blocks) {
                if (block.active && checkCollision(*it, block)) {
                    block.active = false;
                    score++;
                    if (score % 10 == 0) {
                        blockSpeed += 3;
                    }
                    it = bullets.erase(it);
                    bulletRemoved = true;
                    break;
                }
            }
            if (!bulletRemoved) ++it;
        }
        for (auto it = blocks.begin(); it != blocks.end(); ) {
            it->update();
            if (checkGameOver(*it, manX, manY)) {
                cout << "Game Over" << endl;
                cout << "Score: " << score << endl;
                quit = true;
                break;
            }
            if (!it->active) {
                it = blocks.erase(it);
            } else {
                ++it;
            }
        }
        if (frameCount % 30 == 0) {
            int minX = 50;
            int maxX = SCREEN_WIDTH - 50 - 50;
            int blockX = rand() % (maxX - minX + 1) + minX;
            blocks.push_back(Block(blockX, 0, blockTexture, blockSpeed));
        }
        frameCount++;
}
#endif // _PLAYP__H
