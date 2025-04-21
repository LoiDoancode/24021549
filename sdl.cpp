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
using namespace std;

// Hàm đặt lại trạng thái trò chơi
void resetGame(vector<Bullet>& bullets, vector<Block>& blocks, int& score, int& blockSpeed, int& frameCount, int& manX, int& manY, int& manSpeedX, int& manSpeedY) {
    bullets.clear();
    blocks.clear();
    score = 0;
    blockSpeed = 3;
    frameCount = 0;
    manX = SCREEN_WIDTH / 2 - 90;
    manY = SCREEN_HEIGHT - 195;
    manSpeedX = 0;
    manSpeedY = 0;
}

int main(int argc, char *argv[]) {
    srand(time(0));
    Graphics graphics;
    graphics.init();

    // Khởi tạo SDL_ttf
    if (TTF_Init() == -1) {
        SDL_Log("SDL_ttf could not initialize! TTF_Error: %s", TTF_GetError());
        graphics.quit();
        return 1;
    }

    // Khởi tạo SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("SDL_mixer could not initialize! Mix_Error: %s", Mix_GetError());
        TTF_Quit();
        graphics.quit();
        return 1;
    }

    // Tải font
    TTF_Font* font = TTF_OpenFont("Purisa-BoldOblique.ttf", 24);
    if (!font) {
        SDL_Log("Failed to load font! TTF_Error: %s", TTF_GetError());
        Mix_CloseAudio();
        TTF_Quit();
        graphics.quit();
        return 1;
    }

    // Tải các texture
    SDL_Texture* bulletTexture = graphics.loadTexture("bullet.png");
    if (!bulletTexture) {
        SDL_Log("Failed to load bullet.png: %s", IMG_GetError());
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        graphics.quit();
        return 1;
    }
    SDL_Texture* manTexture = graphics.loadTexture("spaceship.png");
    if (!manTexture) {
        SDL_Log("Failed to load spaceship.png: %s", IMG_GetError());
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        graphics.quit();
        return 1;
    }
    SDL_Texture* blockTexture = graphics.loadTexture("skulls.png");
    if (!blockTexture) {
        SDL_Log("Failed to load skulls.png: %s", IMG_GetError());
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        graphics.quit();
        return 1;
    }
    SDL_Texture* backgroundTexture = graphics.loadTexture("cl.jpg");
    if (!backgroundTexture) {
        SDL_Log("Failed to load cl.jpg: %s", IMG_GetError());
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        graphics.quit();
        return 1;
    }

    // Tải nhạc nền
    Mix_Music* bgMusic = Mix_LoadMUS("Music.mp3");
    if (!bgMusic) {
        SDL_Log("Failed to load background music! Mix_Error: %s", Mix_GetError());
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        graphics.quit();
        return 1;
    }
    Mix_PlayMusic(bgMusic, -1); // Lặp vô hạn

    // Tải âm thanh bắn
    Mix_Chunk* gunSound = Mix_LoadWAV("gun.mp3");
    if (!gunSound) {
        SDL_Log("Failed to load gun.mp3: %s", Mix_GetError());
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        graphics.quit();
        return 1;
    }

    // Tải âm thanh khi bắn trúng block
    Mix_Chunk* popSound = Mix_LoadWAV("pop.mp3");
    if (!popSound) {
        SDL_Log("Failed to load pop.mp3: %s", Mix_GetError());
        Mix_FreeChunk(gunSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        graphics.quit();
        return 1;
    }

    vector<Bullet> bullets;
    vector<Block> blocks;
    Sprite man;
    man.init(manTexture, MAN_FRAMES, MAN_CLIPS);

    SDL_Texture* scoreTexture = nullptr;
    SDL_Texture* startTexture = nullptr;
    SDL_Texture* gameOverScoreTexture = nullptr;
    SDL_Texture* retryTexture = nullptr;
    SDL_Texture* exitTexture = nullptr;
    SDL_Color textColor = {255, 255, 255, 255};
    int lastScore = -1;
    int gameState = 0; // 0: Start screen, 1: Playing, 2: Game Over
    SDL_Rect retryRect, exitRect; // Vùng nhấn cho "Thử lại" và "Thoát"

    // Tạo texture cho dòng chữ "Start"
    SDL_Surface* startSurface = TTF_RenderText_Solid(font, "Start", textColor);
    if (!startSurface) {
        SDL_Log("Unable to render start text surface! TTF_Error: %s", TTF_GetError());
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        graphics.quit();
        return 1;
    }
    startTexture = SDL_CreateTextureFromSurface(graphics.renderer, startSurface);
    SDL_FreeSurface(startSurface);
    if (!startTexture) {
        SDL_Log("Unable to create start texture! SDL_Error: %s", SDL_GetError());
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        graphics.quit();
        return 1;
    }

    // Tạo texture cho dòng chữ "Thử lại"
    SDL_Surface* retrySurface = TTF_RenderText_Solid(font, "Retry", textColor);
    if (!retrySurface) {
        SDL_Log("Unable to render retry text surface! TTF_Error: %s", TTF_GetError());
        SDL_DestroyTexture(startTexture);
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        graphics.quit();
        return 1;
    }
    retryTexture = SDL_CreateTextureFromSurface(graphics.renderer, retrySurface);
    SDL_FreeSurface(retrySurface);
    if (!retryTexture) {
        SDL_Log("Unable to create retry texture! SDL_Error: %s", SDL_GetError());
        SDL_DestroyTexture(startTexture);
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        graphics.quit();
        return 1;
    }

    // Tạo texture cho dòng chữ "Thoát"
    SDL_Surface* exitSurface = TTF_RenderText_Solid(font, "Quit", textColor);
    if (!exitSurface) {
        SDL_Log("Unable to render exit text surface! TTF_Error: %s", TTF_GetError());
        SDL_DestroyTexture(startTexture);
        SDL_DestroyTexture(retryTexture);
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        graphics.quit();
        return 1;
    }
    exitTexture = SDL_CreateTextureFromSurface(graphics.renderer, exitSurface);
    SDL_FreeSurface(exitSurface);
    if (!exitTexture) {
        SDL_Log("Unable to create exit texture! SDL_Error: %s", SDL_GetError());
        SDL_DestroyTexture(startTexture);
        SDL_DestroyTexture(retryTexture);
        Mix_FreeChunk(gunSound);
        Mix_FreeChunk(popSound);
        Mix_FreeMusic(bgMusic);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        graphics.quit();
        return 1;
    }

    while (!quit) {
        play(bullets, bulletTexture, gunSound, gameState, retryRect, exitRect, blocks, score, blockSpeed, frameCount, manX, manY, manSpeedX, manSpeedY);

        if (gameState == 1) { // Trạng thái Playing
            man.tick();
            playb(bullets, blocks, blockTexture, popSound, gameState);

            if (score != lastScore) {
                if (scoreTexture) {
                    SDL_DestroyTexture(scoreTexture);
                }
                string scoreText = "Score: " + to_string(score);
                SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
                if (!textSurface) {
                    SDL_Log("Unable to render text surface! TTF_Error: %s", TTF_GetError());
                } else {
                    scoreTexture = SDL_CreateTextureFromSurface(graphics.renderer, textSurface);
                    SDL_FreeSurface(textSurface);
                }
                lastScore = score;
            }
        }
        else if (gameState == 2) { // Trạng thái Game Over
            // Cập nhật texture điểm số cho màn Game Over
            if (gameOverScoreTexture) {
                SDL_DestroyTexture(gameOverScoreTexture);
            }
            string scoreText = "Score: " + to_string(score);
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
            if (!textSurface) {
                SDL_Log("Unable to render game over score text surface! TTF_Error: %s", TTF_GetError());
            } else {
                gameOverScoreTexture = SDL_CreateTextureFromSurface(graphics.renderer, textSurface);
                SDL_FreeSurface(textSurface);
            }
        }

        graphics.prepareScene();
        SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(graphics.renderer, backgroundTexture, nullptr, &backgroundRect);

        if (gameState == 0) { // Trạng thái Start
            int texW, texH;
            SDL_QueryTexture(startTexture, nullptr, nullptr, &texW, &texH);
            SDL_Rect startRect = {(SCREEN_WIDTH - texW) / 2, (SCREEN_HEIGHT - texH) / 2, texW, texH};
            SDL_RenderCopy(graphics.renderer, startTexture, nullptr, &startRect);
        }
        else if (gameState == 1) { // Trạng thái Playing
            graphics.render(manX, manY, man);
            for (auto &bullet : bullets) {
                bullet.render(graphics.renderer);
            }
            for (auto &block : blocks) {
                block.render(graphics.renderer);
            }

            if (scoreTexture) {
                int texW, texH;
                SDL_QueryTexture(scoreTexture, nullptr, nullptr, &texW, &texH);
                SDL_Rect scoreRect = {10, 10, texW, texH};
                SDL_RenderCopy(graphics.renderer, scoreTexture, nullptr, &scoreRect);
            }
        }
        else if (gameState == 2) { // Trạng thái Game Over
            // Hiển thị điểm số
            if (gameOverScoreTexture) {
                int texW, texH;
                SDL_QueryTexture(gameOverScoreTexture, nullptr, nullptr, &texW, &texH);
                SDL_Rect scoreRect = {(SCREEN_WIDTH - texW) / 2, SCREEN_HEIGHT / 2 - 50, texW, texH};
                SDL_RenderCopy(graphics.renderer, gameOverScoreTexture, nullptr, &scoreRect);
            }

            // Hiển thị "Thử lại"
            int retryW, retryH;
            SDL_QueryTexture(retryTexture, nullptr, nullptr, &retryW, &retryH);
            retryRect = {(SCREEN_WIDTH - retryW) / 2 - 100, SCREEN_HEIGHT / 2 + 20, retryW, retryH};
            SDL_RenderCopy(graphics.renderer, retryTexture, nullptr, &retryRect);

            // Hiển thị "Thoát"
            int exitW, exitH;
            SDL_QueryTexture(exitTexture, nullptr, nullptr, &exitW, &exitH);
            exitRect = {(SCREEN_WIDTH - exitW) / 2 + 100, SCREEN_HEIGHT / 2 + 20, exitW, exitH};
            SDL_RenderCopy(graphics.renderer, exitTexture, nullptr, &exitRect);
        }

        graphics.presentScene();
        SDL_Delay(75);
    }

    // Kết thúc nhạc và âm thanh
    Mix_HaltMusic();
    Mix_FreeChunk(gunSound);
    Mix_FreeChunk(popSound);
    Mix_FreeMusic(bgMusic);

    // Giải phóng tài nguyên
    if (scoreTexture) {
        SDL_DestroyTexture(scoreTexture);
    }
    if (startTexture) {
        SDL_DestroyTexture(startTexture);
    }
    if (gameOverScoreTexture) {
        SDL_DestroyTexture(gameOverScoreTexture);
    }
    if (retryTexture) {
        SDL_DestroyTexture(retryTexture);
    }
    if (exitTexture) {
        SDL_DestroyTexture(exitTexture);
    }
    Mix_CloseAudio();
    TTF_CloseFont(font);
    TTF_Quit();
    endg(bulletTexture, manTexture, blockTexture);
    SDL_DestroyTexture(backgroundTexture);
    graphics.quit();
    return 0;
}
