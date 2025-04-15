#ifndef _PLAY__H
#define _PLAY__H
#include "defs.h"
#include "graphics.h"
#include <SDL.h>
#include "initialize.h"
#include "bullets.h"
using namespace std;
void play(vector<Bullet>& bullets, SDL_Texture* bulletTexture) {
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) quit = true;
        else if (e.type == SDL_KEYDOWN) {
            Uint16 mod = SDL_GetModState();
            bool ctrlPressed = (mod & KMOD_CTRL) != 0;

            switch (e.key.keysym.sym) {
                case SDLK_w: manSpeedY = -speed; break;
                case SDLK_s: manSpeedY = speed; break;
                case SDLK_a:
                    if (ctrlPressed) {
                        manSpeedX = -speed * 2;
                    } else {
                        manSpeedX = -speed;
                    }
                    break;
                case SDLK_d:
                    if (ctrlPressed) {
                        manSpeedX = speed * 2;
                    } else {
                        manSpeedX = speed;
                    }
                    break;
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
}
#endif // _PLAY__H
