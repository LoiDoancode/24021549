#ifndef _TEXT__H
#define _TEXT__H
#include <SDL.h>
#include <SDL_ttf.h>
#include "graphics.h"
#include "initialize.h"
using namespace std;

struct TextTextures {
    SDL_Texture* scoreTexture = nullptr;
    SDL_Texture* startTexture = nullptr;
    SDL_Texture* gameOverScoreTexture = nullptr;
    SDL_Texture* retryTexture = nullptr;
    SDL_Texture* exitTexture = nullptr;
    int lastScore = -1;
};

bool initTextTextures(TextTextures& texts, TTF_Font* font, SDL_Renderer* renderer) {
    SDL_Color textColor = {255, 255, 255, 255};

    // Tạo texture cho dòng chữ "Start"
    SDL_Surface* startSurface = TTF_RenderText_Solid(font, "Start", textColor);
    if (!startSurface) {
        SDL_Log("Unable to render start text surface! TTF_Error: %s", TTF_GetError());
        return false;
    }
    texts.startTexture = SDL_CreateTextureFromSurface(renderer, startSurface);
    SDL_FreeSurface(startSurface);
    if (!texts.startTexture) {
        SDL_Log("Unable to create start texture! SDL_Error: %s", SDL_GetError());
        return false;
    }

    // Tạo texture cho dòng chữ "Thử lại"
    SDL_Surface* retrySurface = TTF_RenderText_Solid(font, "Retry", textColor);
    if (!retrySurface) {
        SDL_Log("Unable to render retry text surface! TTF_Error: %s", TTF_GetError());
        SDL_DestroyTexture(texts.startTexture);
        return false;
    }
    texts.retryTexture = SDL_CreateTextureFromSurface(renderer, retrySurface);
    SDL_FreeSurface(retrySurface);
    if (!texts.retryTexture) {
        SDL_Log("Unable to create retry texture! SDL_Error: %s", SDL_GetError());
        SDL_DestroyTexture(texts.startTexture);
        return false;
    }

    // Tạo texture cho dòng chữ "Thoát"
    SDL_Surface* exitSurface = TTF_RenderText_Solid(font, "Quit", textColor);
    if (!exitSurface) {
        SDL_Log("Unable to render exit text surface! TTF_Error: %s", TTF_GetError());
        SDL_DestroyTexture(texts.startTexture);
        SDL_DestroyTexture(texts.retryTexture);
        return false;
    }
    texts.exitTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
    SDL_FreeSurface(exitSurface);
    if (!texts.exitTexture) {
        SDL_Log("Unable to create exit texture! SDL_Error: %s", SDL_GetError());
        SDL_DestroyTexture(texts.startTexture);
        SDL_DestroyTexture(texts.retryTexture);
        return false;
    }

    return true;
}

void updateScoreText(TextTextures& texts, TTF_Font* font, SDL_Renderer* renderer, int score, int gameState) {
    SDL_Color textColor = {255, 255, 255, 255};
    if (gameState == 1 && score != texts.lastScore) { // Trạng thái Playing
        if (texts.scoreTexture) {
            SDL_DestroyTexture(texts.scoreTexture);
        }
        string scoreText = "Score: " + to_string(score);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
        if (!textSurface) {
            SDL_Log("Unable to render text surface! TTF_Error: %s", TTF_GetError());
        } else {
            texts.scoreTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FreeSurface(textSurface);
        }
        texts.lastScore = score;
    }
    else if (gameState == 2) { // Trạng thái Game Over
        if (texts.gameOverScoreTexture) {
            SDL_DestroyTexture(texts.gameOverScoreTexture);
        }
        string scoreText = "Score: " + to_string(score);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
        if (!textSurface) {
            SDL_Log("Unable to render game over score text surface! TTF_Error: %s", TTF_GetError());
        } else {
            texts.gameOverScoreTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FreeSurface(textSurface);
        }
    }
}
#endif
