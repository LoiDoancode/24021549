#ifndef _BLOCK__H
#define _BLOCK__H
struct Block {
    int x, y;
    int width = 30, height = 30;
    int speed;
    bool active = true;
    SDL_Texture* texture;
    int frame = 0;

    Block(int _x, int _y, SDL_Texture* _texture, int _speed)
        : x(_x), y(_y), texture(_texture), speed(_speed) {}

    void update() {
        y += speed;
        if (y > SCREEN_HEIGHT) active = false;
        frame = (frame + 1) % RECTS_FRAMES;
    }

    void render(SDL_Renderer* renderer) {
        if (active) {
            SDL_Rect clip = {RECTS_CLIPs[frame][0], RECTS_CLIPs[frame][1], RECTS_CLIPs[frame][2], RECTS_CLIPs[frame][3]};
            SDL_Rect dest = {x, y, width, height};
            SDL_RenderCopy(renderer, texture, &clip, &dest);
        }
    }
};

bool checkCollision(const Bullet& bullet, const Block& block) {
    return bullet.x < block.x + block.width &&
           bullet.x + bullet.width > block.x &&
           bullet.y < block.y + block.height &&
           bullet.y + bullet.height > block.y;
}
#endif // _BLOCK__H
