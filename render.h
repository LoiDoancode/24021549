#ifndef _RENDER__H
#define _RENDER__H
#include <SDL.h>
#include "graphics.h"
#include "bullets.h"
#include "block.h"
#include "play.h"
#include "game_state.h"
#include "resource_init.h"
#include "text_init.h"
#include "initialize.h"
using namespace std;

void renderGame(Graphics& graphics, SDL_Texture* backgroundTexture, vector<Bullet>& bullets,
                 vector<Block>& blocks, TextComponents& comp) {
    graphics.prepareScene();
    SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(graphics.renderer, backgroundTexture, nullptr, &backgroundRect);

    if (comp.gameState == 0) { // Trạng thái Start
        // Cập nhật alpha để tạo hiệu ứng nhấp nháy cho "Start"
        if (comp.alphaIncreasing) {
            comp.startAlpha += 5;
            if (comp.startAlpha >= 255) {
                comp.startAlpha = 255;
                comp.alphaIncreasing = false;
            }
        } else {
            comp.startAlpha -= 5;
            if (comp.startAlpha <= 100) {
                comp.startAlpha = 100;
                comp.alphaIncreasing = true;
            }
        }

        // Hiển thị tiêu đề "Space Shooter"
        int titleW, titleH;
        SDL_QueryTexture(comp.titleTexture, nullptr, nullptr, &titleW, &titleH);
        SDL_Rect titleRect = {(SCREEN_WIDTH - titleW) / 2, SCREEN_HEIGHT / 2 - 100, titleW, titleH};
        SDL_RenderCopy(graphics.renderer, comp.titleTexture, nullptr, &titleRect);

        // Hiển thị "Start" với hiệu ứng nhấp nháy
        int texW, texH;
        SDL_QueryTexture(comp.startTexture, nullptr, nullptr, &texW, &texH);
        SDL_Rect startRect = {(SCREEN_WIDTH - texW) / 2, (SCREEN_HEIGHT - texH) / 2, texW, texH};
        SDL_SetTextureAlphaMod(comp.startTexture, comp.startAlpha); // Áp dụng alpha cho "Start"
        SDL_RenderCopy(graphics.renderer, comp.startTexture, nullptr, &startRect);
    }
    else if (comp.gameState == 1) { // Trạng thái Playing
        graphics.render(manX, manY, comp.man);
        for (auto &bullet : bullets) {
            bullet.render(graphics.renderer);
        }
        for (auto &block : blocks) {
            block.render(graphics.renderer);
        }

        if (comp.scoreTexture) {
            int texW, texH;
            SDL_QueryTexture(comp.scoreTexture, nullptr, nullptr, &texW, &texH);
            SDL_Rect scoreRect = {10, 10, texW, texH};
            SDL_RenderCopy(graphics.renderer, comp.scoreTexture, nullptr, &scoreRect);
        }
    }
    else if (comp.gameState == 2) { // Trạng thái Game Over
        // Hiển thị điểm số
        if (comp.gameOverScoreTexture) {
            int texW, texH;
            SDL_QueryTexture(comp.gameOverScoreTexture, nullptr, nullptr, &texW, &texH);
            SDL_Rect scoreRect = {(SCREEN_WIDTH - texW) / 2, SCREEN_HEIGHT / 2 - 50, texW, texH};
            SDL_RenderCopy(graphics.renderer, comp.gameOverScoreTexture, nullptr, &scoreRect);
        }

        // Hiển thị "Thử lại"
        int retryW, retryH;
        SDL_QueryTexture(comp.retryTexture, nullptr, nullptr, &retryW, &retryH);
        comp.retryRect = {(SCREEN_WIDTH - retryW) / 2 - 100, SCREEN_HEIGHT / 2 + 20, retryW, retryH};
        SDL_RenderCopy(graphics.renderer, comp.retryTexture, nullptr, &comp.retryRect);

        // Hiển thị "Thoát"
        int exitW, exitH;
        SDL_QueryTexture(comp.exitTexture, nullptr, nullptr, &exitW, &exitH);
        comp.exitRect = {(SCREEN_WIDTH - exitW) / 2 + 100, SCREEN_HEIGHT / 2 + 20, exitW, exitH};
        SDL_RenderCopy(graphics.renderer, comp.exitTexture, nullptr, &comp.exitRect);
    }

    graphics.presentScene();
}

void gameLoop(Graphics& graphics, Resources& res, TextComponents& comp,
              vector<Bullet>& bullets, vector<Block>& blocks) {
    while (!quit) {
        play(bullets, res.bulletTexture, res.gunSound, comp.gameState, comp.retryRect, comp.exitRect, blocks, score, blockSpeed, frameCount, manX, manY, manSpeedX, manSpeedY);

        // Cập nhật trạng thái game
        updateGameState(comp, res, graphics, bullets, blocks);

        // Vẽ giao diện
        renderGame(graphics, res.backgroundTexture, bullets, blocks, comp);
        SDL_Delay(55);
    }
}
#endif
