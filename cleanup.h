#ifndef _CLEANUP__H
#define _CLEANUP__H
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "graphics.h"
#include "quit.h"
#include "resource_init.h"
#include "text_init.h"

void cleanupResources(Graphics& graphics, Resources& res, TextComponents& comp) {
    // Kết thúc nhạc và âm thanh
    Mix_HaltMusic();
    Mix_FreeChunk(res.gunSound);
    Mix_FreeChunk(res.popSound);
    Mix_FreeMusic(res.bgMusic);

    // Giải phóng tài nguyên
    if (comp.scoreTexture) {
        SDL_DestroyTexture(comp.scoreTexture);
    }
    if (comp.startTexture) {
        SDL_DestroyTexture(comp.startTexture);
    }
    if (comp.gameOverScoreTexture) {
        SDL_DestroyTexture(comp.gameOverScoreTexture);
    }
    if (comp.retryTexture) {
        SDL_DestroyTexture(comp.retryTexture);
    }
    if (comp.exitTexture) {
        SDL_DestroyTexture(comp.exitTexture);
    }
    if (comp.titleTexture) {
        SDL_DestroyTexture(comp.titleTexture);
    }
    if (comp.starsTexture) { // Dọn dẹp starsTexture
        SDL_DestroyTexture(comp.starsTexture);
    }
    Mix_CloseAudio();
    TTF_CloseFont(res.font);
    TTF_Quit();
    endg(res.bulletTexture, res.manTexture, res.blockTexture);
    SDL_DestroyTexture(res.backgroundTexture);
    graphics.quit();
}
#endif
