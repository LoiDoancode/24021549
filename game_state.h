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

// Kiểm tra điểm nằm trong hình chữ nhật
bool PointInRect(SDL_FPoint p, SDL_FRect rect) {
    return p.x >= rect.x && p.x <= rect.x + rect.w &&
           p.y >= rect.y && p.y <= rect.y + rect.h;
}

// Tính diện tích tam giác
float Area(SDL_FPoint a, SDL_FPoint b, SDL_FPoint c) {
    return abs((a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2.0f);
}

// Kiểm tra điểm nằm trong tam giác
bool PointInTriangle(SDL_FPoint p, SDL_FPoint a, SDL_FPoint b, SDL_FPoint c) {
    float A = Area(a, b, c);
    float A1 = Area(p, b, c);
    float A2 = Area(a, p, c);
    float A3 = Area(a, b, p);
    return abs(A - (A1 + A2 + A3)) < 0.0001f;
}

// Kiểm tra hai đoạn thẳng giao nhau
bool LinesIntersect(SDL_FPoint a1, SDL_FPoint a2, SDL_FPoint b1, SDL_FPoint b2) {
    float denom = (b2.y - b1.y) * (a2.x - a1.x) - (b2.x - b1.x) * (a2.y - a1.y);
    if (abs(denom) < 0.0001f) return false; // Song song

    float ua = ((b2.x - b1.x) * (a1.y - b1.y) - (b2.y - b1.y) * (a1.x - b1.x)) / denom;
    float ub = ((a2.x - a1.x) * (a1.y - b1.y) - (a2.y - a1.y) * (a1.x - b1.x)) / denom;

    return ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1;
}

// Kiểm tra va chạm tam giác-hình chữ nhật
bool TriangleRectCollision(SDL_FPoint a, SDL_FPoint b, SDL_FPoint c, SDL_FRect rect) {
    // B1: Đỉnh tam giác trong hình chữ nhật
    if (PointInRect(a, rect) || PointInRect(b, rect) || PointInRect(c, rect))
        return true;

    // B2: Đỉnh hình chữ nhật trong tam giác
    SDL_FPoint tl = {rect.x, rect.y};
    SDL_FPoint tr = {rect.x + rect.w, rect.y};
    SDL_FPoint br = {rect.x + rect.w, rect.y + rect.h};
    SDL_FPoint bl = {rect.x, rect.y + rect.h};

    if (PointInTriangle(tl, a, b, c) || PointInTriangle(tr, a, b, c) ||
        PointInTriangle(br, a, b, c) || PointInTriangle(bl, a, b, c))
        return true;

    // B3: Kiểm tra cạnh cắt nhau
    SDL_FPoint triEdges[3][2] = {{a, b}, {b, c}, {c, a}};
    SDL_FPoint rectEdges[4][2] = {{tl, tr}, {tr, br}, {br, bl}, {bl, tl}};

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 4; ++j)
            if (LinesIntersect(triEdges[i][0], triEdges[i][1], rectEdges[j][0], rectEdges[j][1]))
                return true;

    return false;
}

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
                    showSpeedText = true;
                    speedTextStartTime = SDL_GetTicks();
                    speedTextX = giftIt->x;
                    speedTextY = giftIt->y - 20;
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
