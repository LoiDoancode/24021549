#ifndef _TEXT_INIT__H
#define _TEXT_INIT__H
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <fstream>
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
    SDL_Texture* speedUpTexture = nullptr;
    SDL_Texture* scoreTexture = nullptr;
    SDL_Texture* startTexture = nullptr;
    SDL_Texture* gameOverScoreTexture = nullptr;
    SDL_Texture* retryTexture = nullptr;
    SDL_Texture* exitTexture = nullptr;
    SDL_Texture* titleTexture = nullptr;
    SDL_Texture* starsTexture = nullptr;
    SDL_Texture* continueTexture = nullptr;
    SDL_Texture* bestScoreTexture = nullptr;
    SDL_Texture* giftTexture = nullptr;
    Mix_Chunk* rewardSound = nullptr; // Âm thanh khi trúng gift
    Mix_Chunk* endSound = nullptr; // Âm thanh khi thua
    SDL_Color textColor = {255, 255, 255, 255};
    TTF_Font* font = nullptr;

    float startScale = 1.0f;
    float retryScale = 1.0f;
    float continueScale = 1.0f;
    float exitScale = 1.0f;

    const float HOVER_SCALE = 1.2f;
    const float SCALE_SPEED = 0.1f;
    int lastScore = -1;
    int bestScore = 0;
    int gameState = 0; // 0: Start, 1: Playing, 2: Game Over, 3: Pause
    SDL_Rect retryRect, exitRect;
    SDL_Rect startRect, startScreenExitRect;
    SDL_Rect continueRect;
    Uint8 startAlpha = 255;
    bool alphaIncreasing = false;
    std::vector<Star> stars;
};

bool initTextComponents(TextComponents& comp, TTF_Font* font, SDL_Renderer* renderer,
                        SDL_Texture* manTexture, Mix_Chunk* gunSound, Mix_Chunk* popSound,
                        Mix_Music* bgMusic) {
    // Đọc Best Score từ file
    comp.font = font;

    // Tạo texture "+1 Speed"
    SDL_Color textColor = {255, 255, 255, 255}; // Màu vàng
    SDL_Surface* speedUpSurface = TTF_RenderText_Solid(font, "+1 Speed", textColor);
    if (!speedUpSurface) {
        SDL_Log("Failed to render +1 speed text: %s", TTF_GetError());
        return false;
    } else {
        comp.speedUpTexture = SDL_CreateTextureFromSurface(renderer, speedUpSurface);
        SDL_FreeSurface(speedUpSurface);
    }
    std::ifstream inFile("best_score.txt");
    if (inFile.is_open()) {
        inFile >> comp.bestScore;
        inFile.close();
    } else {
        comp.bestScore = 0;
    }

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

    SDL_Surface* continueSurface = TTF_RenderText_Solid(font, "Continue", comp.textColor);
    if (!continueSurface) {
        SDL_Log("Unable to render continue text surface! TTF_Error: %s", TTF_GetError());
        SDL_DestroyTexture(comp.startTexture);
        SDL_DestroyTexture(comp.titleTexture);
        SDL_DestroyTexture(comp.starsTexture);
        SDL_DestroyTexture(comp.retryTexture);
        SDL_DestroyTexture(comp.exitTexture);
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        return false;
    }
    comp.continueTexture = SDL_CreateTextureFromSurface(renderer, continueSurface);
    SDL_FreeSurface(continueSurface);
    if (!comp.continueTexture) {
        SDL_Log("Unable to create continue texture! SDL_Error: %s", SDL_GetError());
        SDL_DestroyTexture(comp.startTexture);
        SDL_DestroyTexture(comp.titleTexture);
        SDL_DestroyTexture(comp.starsTexture);
        SDL_DestroyTexture(comp.retryTexture);
        SDL_DestroyTexture(comp.exitTexture);
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        return false;
    }

    SDL_Surface* giftSurface = IMG_Load(GIFT_FILE);
    if (!giftSurface) {
        SDL_Log("Unable to load gift.png! SDL_Error: %s", IMG_GetError());
        SDL_DestroyTexture(comp.startTexture);
        SDL_DestroyTexture(comp.titleTexture);
        SDL_DestroyTexture(comp.starsTexture);
        SDL_DestroyTexture(comp.retryTexture);
        SDL_DestroyTexture(comp.exitTexture);
        SDL_DestroyTexture(comp.continueTexture);
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        return false;
    }
    comp.giftTexture = SDL_CreateTextureFromSurface(renderer, giftSurface);
    SDL_FreeSurface(giftSurface);
    if (!comp.giftTexture) {
        SDL_Log("Unable to create gift texture! SDL_Error: %s", SDL_GetError());
        SDL_DestroyTexture(comp.startTexture);
        SDL_DestroyTexture(comp.titleTexture);
        SDL_DestroyTexture(comp.starsTexture);
        SDL_DestroyTexture(comp.retryTexture);
        SDL_DestroyTexture(comp.exitTexture);
        SDL_DestroyTexture(comp.continueTexture);
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        return false;
    }

    comp.rewardSound = Mix_LoadWAV("reward.mp3");
    if (!comp.rewardSound) {
        SDL_Log("Unable to load reward.mp3! SDL_Error: %s", Mix_GetError());
        SDL_DestroyTexture(comp.startTexture);
        SDL_DestroyTexture(comp.titleTexture);
        SDL_DestroyTexture(comp.starsTexture);
        SDL_DestroyTexture(comp.retryTexture);
        SDL_DestroyTexture(comp.exitTexture);
        SDL_DestroyTexture(comp.continueTexture);
        SDL_DestroyTexture(comp.giftTexture);
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        return false;
    }

    comp.endSound = Mix_LoadWAV("end.mp3");
    if (!comp.endSound) {
        SDL_Log("Unable to load end.mp3! SDL_Error: %s", Mix_GetError());
        SDL_DestroyTexture(comp.startTexture);
        SDL_DestroyTexture(comp.titleTexture);
        SDL_DestroyTexture(comp.starsTexture);
        SDL_DestroyTexture(comp.retryTexture);
        SDL_DestroyTexture(comp.exitTexture);
        SDL_DestroyTexture(comp.continueTexture);
        SDL_DestroyTexture(comp.giftTexture);
        Mix_FreeChunk(comp.rewardSound);
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        return false;
    }

    string bestScoreText = "Best Score: " + to_string(comp.bestScore);
    SDL_Surface* bestScoreSurface = TTF_RenderText_Solid(font, bestScoreText.c_str(), comp.textColor);
    if (!bestScoreSurface) {
        SDL_Log("Unable to render best score text surface! TTF_Error: %s", TTF_GetError());
        SDL_DestroyTexture(comp.startTexture);
        SDL_DestroyTexture(comp.titleTexture);
        SDL_DestroyTexture(comp.starsTexture);
        SDL_DestroyTexture(comp.retryTexture);
        SDL_DestroyTexture(comp.exitTexture);
        SDL_DestroyTexture(comp.continueTexture);
        SDL_DestroyTexture(comp.giftTexture);
        Mix_FreeChunk(comp.rewardSound);
        Mix_FreeChunk(comp.endSound);
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        return false;
    }
    comp.bestScoreTexture = SDL_CreateTextureFromSurface(renderer, bestScoreSurface);
    SDL_FreeSurface(bestScoreSurface);
    if (!comp.bestScoreTexture) {
        SDL_Log("Unable to create best score texture! SDL_Error: %s", SDL_GetError());
        SDL_DestroyTexture(comp.startTexture);
        SDL_DestroyTexture(comp.titleTexture);
        SDL_DestroyTexture(comp.starsTexture);
        SDL_DestroyTexture(comp.retryTexture);
        SDL_DestroyTexture(comp.exitTexture);
        SDL_DestroyTexture(comp.continueTexture);
        SDL_DestroyTexture(comp.giftTexture);
        Mix_FreeChunk(comp.rewardSound);
        Mix_FreeChunk(comp.endSound);
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
