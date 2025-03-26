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

int main(int argc, char *argv[]) {
    srand(time(0));
    Graphics graphics;
    graphics.init();

    SDL_Texture* bulletTexture = graphics.loadTexture("bullet.png");
    if (!bulletTexture) return 1;

    SDL_Texture* manTexture = graphics.loadTexture("new.png");
    if (!manTexture) return 1;

    Sprite man;
    man.init(manTexture, MAN_FRAMES, MAN_CLIPS);

    vector<Bullet> bullets;
    bool quit = false;
    SDL_Event e;
    int manX = 100, manY = 600;
    int speed = 15;
    int manSpeedX = 0, manSpeedY = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_w: manSpeedX = 0; manSpeedY = -speed; break;
                    case SDLK_s: manSpeedX = 0; manSpeedY = speed; break;
                    case SDLK_a: manSpeedX = -speed; manSpeedY = 0; break;
                    case SDLK_d: manSpeedX = speed; manSpeedY = 0; break;
                    case SDLK_SPACE:
                        bullets.push_back(Bullet(manX + 147, manY + 16, bulletTexture));
                        break;
                }
            } else if (e.type == SDL_KEYUP) {
                if (e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_s) {
                    manSpeedY = 0;
                }
                if (e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_d) {
                    manSpeedX = 0;
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
            if (!it->active) {
                it = bullets.erase(it);
            } else {
                ++it;
            }
        }

        graphics.prepareScene();
        graphics.render(manX, manY, man);

        for (auto &bullet : bullets) {
            bullet.render(graphics.renderer);
        }

        graphics.presentScene();
        SDL_Delay(75);
    }

    SDL_DestroyTexture(bulletTexture);
    SDL_DestroyTexture(manTexture);
    graphics.quit();
    return 0;
}
