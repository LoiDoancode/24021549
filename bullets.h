#ifndef _BULLETS__H
#define _BULLETS__H
#include "graphics.h"
#include "defs.h"

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

    // Thêm hàm getRect để trả về vùng hình chữ nhật của viên đạn
    SDL_Rect getRect() const {
        return {x, y, width, height};
    }
};

#endif
