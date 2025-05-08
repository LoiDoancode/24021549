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
#include "gift.h"
using namespace std;

void renderGame(Graphics& graphics, SDL_Texture* backgroundTexture, vector<Bullet>& bullets,
                vector<Block>& blocks, TextComponents& comp) {
    graphics.prepareScene();
    SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(graphics.renderer, backgroundTexture, nullptr, &backgroundRect);

    // Cập nhật alpha để tạo hiệu ứng nhấp nháy cho "Start", "Quit", "Retry", và "Continue"
    if (comp.gameState == 0 || comp.gameState == 2 || comp.gameState == 3) {
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
    }

    if (comp.gameState == 0) { // Trạng thái Start
        // Hiển thị tiêu đề "Space Shooter"
        int titleW, titleH;
        SDL_QueryTexture(comp.titleTexture, nullptr, nullptr, &titleW, &titleH);
        SDL_Rect titleRect = {(SCREEN_WIDTH - titleW) / 2, SCREEN_HEIGHT / 2 - 100, titleW, titleH};
        SDL_RenderCopy(graphics.renderer, comp.titleTexture, nullptr, &titleRect);

        // Hiển thị "Start" với hiệu ứng nhấp nháy
        int texW, texH;
        SDL_QueryTexture(comp.startTexture, nullptr, nullptr, &texW, &texH);
        comp.startRect = {(SCREEN_WIDTH - texW) / 2, (SCREEN_HEIGHT - texH) / 2, texW, texH};
        SDL_SetTextureAlphaMod(comp.startTexture, comp.startAlpha);
        SDL_RenderCopy(graphics.renderer, comp.startTexture, nullptr, &comp.startRect);

        // Hiển thị "Quit" với hiệu ứng nhấp nháy, ngay dưới "Start"
        int exitW, exitH;
        SDL_QueryTexture(comp.exitTexture, nullptr, nullptr, &exitW, &exitH);
        comp.startScreenExitRect = {(SCREEN_WIDTH - exitW) / 2, (SCREEN_HEIGHT - exitH) / 2 + texH + 20, exitW, exitH};
        SDL_SetTextureAlphaMod(comp.exitTexture, comp.startAlpha);
        SDL_RenderCopy(graphics.renderer, comp.exitTexture, nullptr, &comp.startScreenExitRect);

        // Hiển thị Best Score dưới "Quit"
        if (comp.bestScoreTexture) {
            int bestW, bestH;
            SDL_QueryTexture(comp.bestScoreTexture, nullptr, nullptr, &bestW, &bestH);
            SDL_Rect bestRect = {(SCREEN_WIDTH - bestW) / 2, (SCREEN_HEIGHT - bestH) / 2 + texH + exitH + 40, bestW, bestH};
            SDL_RenderCopy(graphics.renderer, comp.bestScoreTexture, nullptr, &bestRect);
        }

        // Vẽ các ngôi sao với animation
        for (auto& star : comp.stars) {
            if (star.active) {
                SDL_Rect clip = STAR_CLIPS[star.currentFrame];
                SDL_Rect renderRect = {star.rect.x, star.rect.y,
                                      (clip.w == 600) ? 32 : 16,
                                      (clip.h == 600) ? 32 : 16};
                SDL_RenderCopy(graphics.renderer, comp.starsTexture, &clip, &renderRect);
            }
        }
    }
    else if (comp.gameState == 1) { // Trạng thái Playing
        graphics.render(manX, manY, comp.man);
        for (auto &bullet : bullets) {
            bullet.render(graphics.renderer);
        }
        for (auto &block : blocks) {
            block.render(graphics.renderer);
        }
        for (auto &gift : gifts) {
            gift.render(graphics.renderer);
        }
        if (showSpeedText && comp.speedUpTexture) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - speedTextStartTime < SPEED_TEXT_DURATION) {
            int texW, texH;
            SDL_QueryTexture(comp.speedUpTexture, nullptr, nullptr, &texW, &texH);
            SDL_Rect textRect = {speedTextX, speedTextY, texW, texH};
            SDL_RenderCopy(graphics.renderer, comp.speedUpTexture, nullptr, &textRect);
        } else {
            showSpeedText = false;
        }
    }

        if (comp.scoreTexture) {
            int texW, texH;
            SDL_QueryTexture(comp.scoreTexture, nullptr, nullptr, &texW, &texH);
            SDL_Rect scoreRect = {10, 10, texW, texH};
            SDL_RenderCopy(graphics.renderer, comp.scoreTexture, nullptr, &scoreRect);
        }

        // Hiển thị Best Score dưới điểm số hiện tại
        if (comp.bestScoreTexture) {
            int bestW, bestH;
            SDL_QueryTexture(comp.bestScoreTexture, nullptr, nullptr, &bestW, &bestH);
            SDL_Rect bestRect = {10, 40, bestW, bestH};
            SDL_RenderCopy(graphics.renderer, comp.bestScoreTexture, nullptr, &bestRect);
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

        // Hiển thị Best Score dưới điểm số
        if (comp.bestScoreTexture) {
            int bestW, bestH;
            SDL_QueryTexture(comp.bestScoreTexture, nullptr, nullptr, &bestW, &bestH);
            SDL_Rect bestRect = {(SCREEN_WIDTH - bestW) / 2, SCREEN_HEIGHT / 2 - 20, bestW, bestH};
            SDL_RenderCopy(graphics.renderer, comp.bestScoreTexture, nullptr, &bestRect);
        }

        // Hiển thị "Retry" với hiệu ứng nhấp nháy
        int retryW, retryH;
        SDL_QueryTexture(comp.retryTexture, nullptr, nullptr, &retryW, &retryH);
        comp.retryRect = {(SCREEN_WIDTH - retryW) / 2 - 100, SCREEN_HEIGHT / 2 + 20, retryW, retryH};
        SDL_SetTextureAlphaMod(comp.retryTexture, comp.startAlpha);
        SDL_RenderCopy(graphics.renderer, comp.retryTexture, nullptr, &comp.retryRect);

        // Hiển thị "Quit" với hiệu ứng nhấp nháy
        int exitW, exitH;
        SDL_QueryTexture(comp.exitTexture, nullptr, nullptr, &exitW, &exitH);
        comp.exitRect = {(SCREEN_WIDTH - exitW) / 2 + 100, SCREEN_HEIGHT / 2 + 20, exitW, exitH};
        SDL_SetTextureAlphaMod(comp.exitTexture, comp.startAlpha);
        SDL_RenderCopy(graphics.renderer, comp.exitTexture, nullptr, &comp.exitRect);
    }
    else if (comp.gameState == 3) { // Trạng thái Pause
        // Hiển thị "Continue" với hiệu ứng nhấp nháy
        int continueW, continueH;
        SDL_QueryTexture(comp.continueTexture, nullptr, nullptr, &continueW, &continueH);
        comp.continueRect = {(SCREEN_WIDTH - continueW) / 2 - 100, SCREEN_HEIGHT / 2, continueW, continueH};
        SDL_SetTextureAlphaMod(comp.continueTexture, comp.startAlpha);
        SDL_RenderCopy(graphics.renderer, comp.continueTexture, nullptr, &comp.continueRect);

        // Hiển thị "Quit" với hiệu ứng nhấp nháy
        int exitW, exitH;
        SDL_QueryTexture(comp.exitTexture, nullptr, nullptr, &exitW, &exitH);
        comp.exitRect = {(SCREEN_WIDTH - exitW) / 2 + 100, SCREEN_HEIGHT / 2, exitW, exitH};
        SDL_SetTextureAlphaMod(comp.exitTexture, comp.startAlpha);
        SDL_RenderCopy(graphics.renderer, comp.exitTexture, nullptr, &comp.exitRect);
    }

    graphics.presentScene();
}

void gameLoop(Graphics& graphics, Resources& res, TextComponents& comp,
              vector<Bullet>& bullets, vector<Block>& blocks) {
    const int MAX_STARS = 7;
    int fixedPositions[][2] = {
        {100, 100}, {500, 150}, {900, 200}, {200, 300}, {600, 105}, {800, 260}, {300, 370}
    };

    if (comp.stars.empty() && comp.gameState == 0) {
        for (int i = 0; i < MAX_STARS; ++i) {
            Star newStar;
            newStar.rect.x = fixedPositions[i][0];
            newStar.rect.y = fixedPositions[i][1];
            newStar.rect.w = 32;
            newStar.rect.h = 32;
            newStar.active = true;
            newStar.frameCounter = 0;
            newStar.currentFrame = 0;
            comp.stars.push_back(newStar);
        }
    }

    while (!quit) {
        play(bullets, res.bulletTexture, res.gunSound, comp.gameState, comp.retryRect, comp.exitRect, blocks, score, blockSpeed, frameCount, manX, manY, manSpeedX, manSpeedY, comp.startRect, comp.startScreenExitRect, comp.continueRect);
        updateGameState(comp, res, graphics, bullets, blocks);
        if (comp.gameState == 0) {
            for (auto& star : comp.stars) {
                if (star.active) {
                    star.frameCounter++;
                    if (star.frameCounter >= 10) {
                        star.currentFrame = (star.currentFrame + 1) % STAR_FRAMES;
                        star.frameCounter = 0;
                    }
                }
            }
        }
        renderGame(graphics, res.backgroundTexture, bullets, blocks, comp);
        SDL_Delay(55);
    }
}
#endif
