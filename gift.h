#ifndef _GIFT__H
#define _GIFT__H
#include <SDL.h>
#include <random>
#include "defs.h"

struct Gift {
    int x, y;
    SDL_Texture* texture;
    bool active;

    Gift(int _x, int _y, SDL_Texture* _texture) : x(_x), y(_y), texture(_texture), active(true) {}

    void update() {
        if (active) {
            y += GIFT_SPEED; // Di chuyển xuống
            if (y > SCREEN_HEIGHT + 200) {
                active = false; // Xóa gift khi ra khỏi màn hình
            }
        }
    }

    void render(SDL_Renderer* renderer) {
        if (active) {
            SDL_Rect dst = {x, y, GIFT_WIDTH, GIFT_HEIGHT};
            SDL_RenderCopy(renderer, texture, nullptr, &dst);
        }
    }

    SDL_Rect getRect() const {
        return {x, y, GIFT_WIDTH-10, GIFT_HEIGHT-20};
    }
};

// Tạo gift ngẫu nhiên
void createGift(std::vector<Gift>& gifts, SDL_Texture* giftTexture) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> disX(50, SCREEN_WIDTH - GIFT_WIDTH-50);
    static std::uniform_real_distribution<> disChance(0.0, 1.0);

    if (disChance(gen) < 0.05) {
        int x = disX(gen);
        gifts.emplace_back(x, -GIFT_HEIGHT, giftTexture);
    }
}

#endif
