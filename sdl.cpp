#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "defs.h"
#include "bullets.h"
#include "block.h"
#include "check.h"
#include "play.h"
#include "initialize.h"
#include "quit.h"
#include "playb.h"

using namespace std;

int main(int argc, char *argv[]) {
    srand(time(0));
    Graphics graphics;
    graphics.init();
    SDL_Texture* bulletTexture = graphics.loadTexture("bullet.png");
    SDL_Texture* manTexture = graphics.loadTexture("new.png");
    SDL_Texture* blockTexture = graphics.loadTexture("skulls.png");
    SDL_Texture* backgroundTexture = graphics.loadTexture("cl.jpg");
    if (!bulletTexture || !manTexture || !blockTexture || !backgroundTexture) return 1;
    vector<Bullet> bullets;
    vector<Block> blocks;
    Sprite man;
    man.init(manTexture, MAN_FRAMES, MAN_CLIPS);

    while (!quit) {
        play(bullets, bulletTexture);
        man.tick();
        playb(bullets, blocks, blockTexture);
        if (frameCount % 30 == 0) {
            int minX = 50;
            int maxX = SCREEN_WIDTH - 50 - 50;
            int blockX = rand() % (maxX - minX + 1) + minX;
            blocks.push_back(Block(blockX, 0, blockTexture, blockSpeed));
        }
        frameCount++;
        graphics.prepareScene();
        SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(graphics.renderer, backgroundTexture, nullptr, &backgroundRect);
        graphics.render(manX, manY, man);
        for (auto &bullet : bullets) {
            bullet.render(graphics.renderer);
        }
        for (auto &block : blocks) {
            block.render(graphics.renderer);
        }
        graphics.presentScene();
        SDL_Delay(75);
    }
    endg(bulletTexture, manTexture, blockTexture);
    SDL_DestroyTexture(backgroundTexture);
    graphics.quit();
    return 0;
}
