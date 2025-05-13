#ifndef _PLAY__H
#define _PLAY__H
#include "defs.h"
#include "graphics.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include "initialize.h"
#include "bullets.h"
#include "block.h"
#include "reset.h"
using namespace std;

void resetGame(vector<Bullet>& bullets, vector<Block>& blocks, int& score, int& blockSpeed, int& frameCount, int& manX, int& manY, int& manSpeedX, int& manSpeedY);

void play(vector<Bullet>& bullets, SDL_Texture* bulletTexture, Mix_Chunk* gunSound, int& gameState,
          SDL_Rect& retryRect, SDL_Rect& exitRect, vector<Block>& blocks, int& score, int& blockSpeed,
          int& frameCount, int& manX, int& manY, int& manSpeedX, int& manSpeedY, SDL_Rect& startRect,
          SDL_Rect& startScreenExitRect, SDL_Rect& continueRect) {
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if (gameState == 0) {
                if (x >= startRect.x && x <= startRect.x + startRect.w &&
                    y >= startRect.y && y <= startRect.y + startRect.h) {
                    gameState = 1; // Chuyển sang trạng thái chơi
                }
                if (x >= startScreenExitRect.x && x <= startScreenExitRect.x + startScreenExitRect.w &&
                    y >= startScreenExitRect.y && y <= startScreenExitRect.y + startScreenExitRect.h) {
                    quit = true; // Thoát game
                }
            }
            else if (gameState == 2) {
                if (x >= retryRect.x && x <= retryRect.x + retryRect.w &&
                    y >= retryRect.y && y <= retryRect.y + retryRect.h) {
                    resetGame(bullets, blocks, score, blockSpeed, frameCount, manX, manY, manSpeedX, manSpeedY);
                    gameState = 1;
                }
                if (x >= exitRect.x && x <= exitRect.x + exitRect.w &&
                    y >= exitRect.y && y <= exitRect.y + exitRect.h) {
                    quit = true;
                }
            }
            else if (gameState == 3) { // Trạng thái Pause
                if (x >= continueRect.x && x <= continueRect.x + continueRect.w &&
                    y >= continueRect.y && y <= continueRect.y + continueRect.h) {
                    gameState = 1; // Tiếp tục chơi
                }
                if (x >= exitRect.x && x <= exitRect.x + exitRect.w &&
                    y >= exitRect.y && y <= exitRect.y + exitRect.h) {
                    quit = true; // Thoát game
                }
            }
        }
        else if (e.type == SDL_KEYDOWN) {
            if (gameState == 1) {
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
                        bullets.push_back(Bullet(manX + 60, manY, bulletTexture));
                        Mix_PlayChannel(-1, gunSound, 0);
                        break;
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        manSpeedX = 0;
                        manSpeedY = 0;
                        break;
                    case SDLK_ESCAPE:
                        gameState = 3;
                        break;
                }
            }
        }
    }
}
#endif
