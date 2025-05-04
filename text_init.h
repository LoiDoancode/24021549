#ifndef _TEXT_INIT__H
#define _TEXT_INIT__H
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include "graphics.h"
#include "defs.h"

struct Star {
    SDL_Rect rect;
    bool active;
    int frameCounter;
    int currentFrame;
};

struct TextComponents {
    Sprite man;
    SDL_Texture* scoreTexture = nullptr;
    SDL_Texture* startTexture = nullptr;
    SDL_Texture* gameOverScoreTexture = nullptr;
    SDL_Texture* retryTexture = nullptr;
    SDL_Texture* exitTexture = nullptr;
    SDL_Texture* titleTexture = nullptr;
    SDL_Texture* starsTexture = nullptr;
    SDL_Color textColor = {255, 255, 255, 255};
    int lastScore = -1;
    int gameState = 0;
    SDL_Rect retryRect, exitRect; // Vùng nhấn cho "Retry" và "Quit" trong game over
    SDL_Rect startRect, startScreenExitRect; // Vùng nhấn cho "Start" và "Quit" trong màn hình start
    Uint8 startAlpha = 255;
    bool alphaIncreasing = false;
    std::vector<Star> stars;
};

bool initTextComponents(TextComponents& comp, TTF_Font* font, SDL_Renderer* renderer,
                        SDL_Texture* manTexture, Mix_Chunk* gunSound, Mix_Chunk* popSound,
                        Mix_Music* bgMusic) {
    comp.man.init(manTexture, MAN_FRAMES, MAN_CLIPS);

    SDL_Surface* startSurface = TTF_RenderText_Solid(font, "Start", comp.textColor);
    if (!startSurface) {
        SDL_Log("Unable to render start text surface! TTF_Error: %s", TTF_GetError());
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        return false;
    }
    comp.startTexture = SDL_CreateTextureFromSurface(renderer, startSurface);
    SDL_FreeSurface(startSurface);
    if (!comp.startTexture) {
        SDL_Log("Unable to create start texture! SDL_Error: %s", SDL_GetError());
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        return false;
    }

    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "Space Shooter", comp.textColor);
    if (!titleSurface) {
        SDL_Log("Unable to render title text surface! TTF_Error: %s", TTF_GetError());
        SDL_DestroyTexture(comp.startTexture);
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        return false;
    }
    comp.titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_FreeSurface(titleSurface);
    if (!comp.titleTexture) {
        SDL_Log("Unable to create title texture! SDL_Error: %s", SDL_GetError());
        SDL_DestroyTexture(comp.startTexture);
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        return false;
    }

    SDL_Surface* starsSurface = IMG_Load("stars.png");
    if (!starsSurface) {
        SDL_Log("Unable to load stars.png! SDL_Error: %s", IMG_GetError());
        SDL_DestroyTexture(comp.startTexture);
        SDL_DestroyTexture(comp.titleTexture);
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        return false;
    }
    comp.starsTexture = SDL_CreateTextureFromSurface(renderer, starsSurface);
    SDL_FreeSurface(starsSurface);
    if (!comp.starsTexture) {
        SDL_Log("Unable to create stars texture! SDL_Error: %s", SDL_GetError());
        SDL_DestroyTexture(comp.startTexture);
        SDL_DestroyTexture(comp.titleTexture);
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        return false;
    }

    SDL_Surface* retrySurface = TTF_RenderText_Solid(font, "Retry", comp.textColor);
    if (!retrySurface) {
        SDL_Log("Unable to render retry text surface! TTF_Error: %s", TTF_GetError());
        SDL_DestroyTexture(comp.startTexture);
        SDL_DestroyTexture(comp.titleTexture);
        SDL_DestroyTexture(comp.starsTexture);
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        return false;
    }
    comp.retryTexture = SDL_CreateTextureFromSurface(renderer, retrySurface);
    SDL_FreeSurface(retrySurface);
    if (!comp.retryTexture) {
        SDL_Log("Unable to create retry texture! SDL_Error: %s", SDL_GetError());
        SDL_DestroyTexture(comp.startTexture);
        SDL_DestroyTexture(comp.titleTexture);
        SDL_DestroyTexture(comp.starsTexture);
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        return false;
    }

    SDL_Surface* exitSurface = TTF_RenderText_Solid(font, "Quit", comp.textColor);
    if (!exitSurface) {
        SDL_Log("Unable to render exit text surface! TTF_Error: %s", TTF_GetError());
        SDL_DestroyTexture(comp.startTexture);
        SDL_DestroyTexture(comp.titleTexture);
        SDL_DestroyTexture(comp.starsTexture);
        SDL_DestroyTexture(comp.retryTexture);
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        return false;
    }
    comp.exitTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
    SDL_FreeSurface(exitSurface);
    if (!comp.exitTexture) {
        SDL_Log("Unable to create exit texture! SDL_Error: %s", SDL_GetError());
        SDL_DestroyTexture(comp.startTexture);
        SDL_DestroyTexture(comp.titleTexture);
        SDL_DestroyTexture(comp.starsTexture);
        SDL_DestroyTexture(comp.retryTexture);
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        return false;
    }

    return true;
}
#endif
