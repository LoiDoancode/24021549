#ifndef _GAME_STATE__H
#define _GAME_STATE__H
#include <SDL.h>
#include <SDL_ttf.h>
#include "graphics.h"
#include "bullets.h"
#include "block.h"
#include "playb.h"
#include "resource_init.h"
#include "text_init.h"
#include "initialize.h"
#include "gift.h"
#include <fstream>
using namespace std;

void updateGameState(TextComponents& comp, Resources& res, Graphics& graphics,
                     vector<Bullet>& bullets, vector<Block>& blocks) {
    if (comp.gameState == 1) {
        comp.man.tick();
        playb(bullets, blocks, res.blockTexture, res.popSound, comp.gameState);

        // Tạo gift ngẫu nhiên
        createGift(gifts, comp.giftTexture);

        // Cập nhật vị trí gift
        for (auto it = gifts.begin(); it != gifts.end();) {
            it->update();
            if (!it->active) {
                it = gifts.erase(it);
            } else {
                ++it;
            }
        }

        // Kiểm tra va chạm giữa đạn và gift
        for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();) {
            bool bulletRemoved = false;
            for (auto giftIt = gifts.begin(); giftIt != gifts.end() && !bulletRemoved;) {
                SDL_Rect bulletRect = bulletIt->getRect();
                SDL_Rect giftRect = giftIt->getRect();
                if (SDL_HasIntersection(&bulletRect, &giftRect)) {
                    giftIt->active = false; // Xóa gift
                    bulletIt = bullets.erase(bulletIt); // Xóa đạn
                    speed += 1; // Tăng tốc độ tàu
                    Mix_PlayChannel(-1, comp.rewardSound, 0); // Phát âm thanh
                    bulletRemoved = true;
                } else {
                    ++giftIt;
                }
            }
            if (!bulletRemoved) {
                ++bulletIt;
            }
        }

        // Kiểm tra va chạm giữa tàu và gift
        SDL_Rect manRect = getManRect();
        for (auto giftIt = gifts.begin(); giftIt != gifts.end();) {
            SDL_Rect giftRect = giftIt->getRect();
            if (SDL_HasIntersection(&manRect, &giftRect)) {
                giftIt->active = false; // Xóa gift
                speed += 1; // Tăng tốc độ tàu
                Mix_PlayChannel(-1, comp.rewardSound, 0); // Phát âm thanh
                giftIt = gifts.erase(giftIt);
            } else {
                ++giftIt;
            }
        }

        if (score != comp.lastScore) {
            if (comp.scoreTexture) {
                SDL_DestroyTexture(comp.scoreTexture);
            }
            string scoreText = "Score: " + to_string(score);
            SDL_Surface* textSurface = TTF_RenderText_Solid(res.font, scoreText.c_str(), comp.textColor);
            if (!textSurface) {
                SDL_Log("Unable to render text surface! TTF_Error: %s", TTF_GetError());
            } else {
                comp.scoreTexture = SDL_CreateTextureFromSurface(graphics.renderer, textSurface);
                SDL_FreeSurface(textSurface);
            }
            comp.lastScore = score;

            // Cập nhật Best Score nếu score hiện tại cao hơn
            if (score > comp.bestScore) {
                comp.bestScore = score;
                if (comp.bestScoreTexture) {
                    SDL_DestroyTexture(comp.bestScoreTexture);
                }
                string bestScoreText = "Best Score: " + to_string(comp.bestScore);
                SDL_Surface* bestScoreSurface = TTF_RenderText_Solid(res.font, bestScoreText.c_str(), comp.textColor);
                if (!bestScoreSurface) {
                    SDL_Log("Unable to render best score text surface! TTF_Error: %s", TTF_GetError());
                } else {
                    comp.bestScoreTexture = SDL_CreateTextureFromSurface(graphics.renderer, bestScoreSurface);
                    SDL_FreeSurface(bestScoreSurface);
                }

                // Ghi Best Score vào file
                std::ofstream outFile("best_score.txt");
                if (outFile.is_open()) {
                    outFile << comp.bestScore;
                    outFile.close();
                } else {
                    SDL_Log("Unable to write best score to file!");
                }
            }
        }
    }
    else if (comp.gameState == 2) {
        // Phát âm thanh khi vào trạng thái Game Over
        if (!playedEndSound) {
            Mix_PlayChannel(-1, comp.endSound, 0);
            playedEndSound = true;
        }

        if (comp.gameOverScoreTexture) {
            SDL_DestroyTexture(comp.gameOverScoreTexture);
        }
        string scoreText = "Score: " + to_string(score);
        SDL_Surface* textSurface = TTF_RenderText_Solid(res.font, scoreText.c_str(), comp.textColor);
        if (!textSurface) {
            SDL_Log("Unable to render game over score text surface! TTF_Error: %s", TTF_GetError());
        } else {
            comp.gameOverScoreTexture = SDL_CreateTextureFromSurface(graphics.renderer, textSurface);
            SDL_FreeSurface(textSurface);
        }
    }
    // gameState == 3 (Pause): Không cập nhật gameplay
}
#endif
