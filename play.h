#ifndef _PLAY__H
#define _PLAY__H
#include "defs.h"
#include "graphics.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include "initialize.h"
#include "bullets.h"
#include "block.h" // Thêm để truy cập kiểu Block
using namespace std;

// Prototype của resetGame
void resetGame(vector<Bullet>& bullets, vector<Block>& blocks, int& score, int& blockSpeed, int& frameCount, int& manX, int& manY, int& manSpeedX, int& manSpeedY);

void play(vector<Bullet>&bullets, SDL_Texture* bulletTexture, Mix_Chunk* gunSound, int& gameState,
          SDL_Rect& retryRect, SDL_Rect& exitRect, vector<Block>& blocks, int& score, int& blockSpeed,
          int& frameCount, int& manX, int& manY, int& manSpeedX, int& manSpeedY) {
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if (gameState == 0) { // Màn Start
                gameState = 1;
            }
            else if (gameState == 2) { // Màn Game Over
                // Kiểm tra nhấn vào "Thử lại"
                if (x >= retryRect.x && x <= retryRect.x + retryRect.w &&
                    y >= retryRect.y && y <= retryRect.y + retryRect.h) {
                    // Đặt lại trò chơi
                    resetGame(bullets, blocks, score, blockSpeed, frameCount, manX, manY, manSpeedX, manSpeedY);
                    gameState = 1; // Chuyển về trạng thái Playing
                }
                // Kiểm tra nhấn vào "Thoát"
                if (x >= exitRect.x && x <= exitRect.x + exitRect.w &&
                    y >= exitRect.y && y <= exitRect.y + exitRect.h) {
                    quit = true; // Thoát game
                }
            }
        }
        else if (e.type == SDL_KEYDOWN && gameState == 1) { // Chỉ xử lý phím khi đang chơi
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
            }
        }
    }
}
#endif
