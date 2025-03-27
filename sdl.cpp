#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "defs.h"

using namespace std;

struct Bullet {
    int x, y;
    int speed = 10;
    int width = 30, height = 30;
    bool active = true;
    SDL_Texture* texture;
    int frame = 0;

    Bullet(int _x, int _y, SDL_Texture* _texture)
        : x(_x), y(_y), texture(_texture) {}

    void update() {
        y -= speed;
        if (y + height < 0) active = false;
        frame = (frame + 1) % BULLETS_FRAMES;
    }

    void render(SDL_Renderer* renderer) {
        SDL_Rect clip = {BULLETS_CLIPs[frame][0], BULLETS_CLIPs[frame][1], BULLETS_CLIPs[frame][2], BULLETS_CLIPs[frame][3]};
        SDL_Rect dest = {x, y, width, height};
        SDL_RenderCopy(renderer, texture, &clip, &dest);
    }
};

struct Block {
    int x, y;
    int width = 50, height = 50;
    int speed = 5;
    bool active = true;
    SDL_Texture* texture;
    int frame = 0;

    Block(int _x, int _y, SDL_Texture* _texture)
        : x(_x), y(_y), texture(_texture) {}

    void update() {
        y += speed;
        if (y > SCREEN_HEIGHT) active = false;
        frame = (frame + 1) % RECTS_FRAMES;
    }

    void render(SDL_Renderer* renderer) {
        if (active) {
            SDL_Rect clip = {RECTS_CLIPs[0][0], RECTS_CLIPs[0][1], RECTS_CLIPs[0][2], RECTS_CLIPs[0][3]};
            SDL_Rect dest = {x, y, width, height};
            SDL_RenderCopy(renderer, texture, &clip, &dest);
        }
    }
};

bool checkCollision(const Bullet& bullet, const Block& block) {
    return bullet.x < block.x + block.width &&
           bullet.x + bullet.width > block.x &&
           bullet.y < block.y + block.height &&
           bullet.y + bullet.height > block.y;
}

bool checkGameOver(const Block& block, int manX, int manY) {
    return (block.x < manX + 64 && block.x + block.width > manX &&
            block.y < manY + 64 && block.y + block.height > manY) ||
           (block.y >= SCREEN_HEIGHT);
}

int main(int argc, char *argv[]) {
    srand(time(0));
    Graphics graphics;
    graphics.init();

    SDL_Texture* bulletTexture = graphics.loadTexture("bullet.png");
    SDL_Texture* manTexture = graphics.loadTexture("new.png");
    SDL_Texture* blockTexture = graphics.loadTexture("skulls.png");
    if (!bulletTexture || !manTexture || !blockTexture) return 1;

    Sprite man;
    man.init(manTexture, MAN_FRAMES, MAN_CLIPS);

    vector<Bullet> bullets;
    vector<Block> blocks;
    int score = 0;

    bool quit = false;
    SDL_Event e;
    int manX = 100, manY = 600;
    int speed = 15;
    int manSpeedX = 0, manSpeedY = 0;
    int frameCount = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_w: manSpeedY = -speed; break;
                    case SDLK_s: manSpeedY = speed; break;
                    case SDLK_a: manSpeedX = -speed; break;
                    case SDLK_d: manSpeedX = speed; break;
                    case SDLK_SPACE:
                        bullets.push_back(Bullet(manX + 58, manY, bulletTexture));
                        break;
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        manSpeedX = 0;
                        manSpeedY = 0;
                        break;
                }
            }
        }
        man.tick();

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
            blocks.push_back(Block(rand() % (SCREEN_WIDTH - 50), 0, blockTexture));
        }
        frameCount++;

        graphics.prepareScene();
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

    SDL_DestroyTexture(bulletTexture);
    SDL_DestroyTexture(manTexture);
    SDL_DestroyTexture(blockTexture);
    graphics.quit();
    return 0;
}
