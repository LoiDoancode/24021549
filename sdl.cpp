#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "graphics.h"
#include "defs.h"
#include "bullets.h"
#include "block.h"
#include "check.h"
#include "play.h"
#include "initialize.h"
#include "quit.h"
#include "playb.h"
#include "text_init.h"
#include "resource_init.h"
#include "cleanup.h"
#include "game_state.h"
#include "render.h"
using namespace std;
int main(int argc, char *argv[]) {
    srand(time(0));
    Graphics graphics;
    graphics.init();
    // Khởi tạo tài nguyên
    Resources res;
    if (!initResources(res, graphics)) {
        graphics.quit();
        return 1;
    }
    vector<Bullet> bullets;
    vector<Block> blocks;
    // Khởi tạo các thành phần văn bản và sprite tàu
    TextComponents comp;
    if (!initTextComponents(comp, res.font, graphics.renderer, res.manTexture, res.gunSound, res.popSound, res.bgMusic)) {
        graphics.quit();
        return 1;
    }
    // Chạy vòng lặp chính
    gameLoop(graphics, res, comp, bullets, blocks);
    cleanupResources(graphics, res, comp);
    return 0;
}
