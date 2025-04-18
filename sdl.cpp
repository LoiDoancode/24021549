#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h> // <-- Thêm dòng này
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

    SDL_Texture* bulletTexture = graphics.loadTexture("bullet.png");
    SDL_Texture* manTexture = graphics.loadTexture("spaceship.png");
    SDL_Texture* blockTexture = graphics.loadTexture("skulls.png");
    SDL_Texture* backgroundTexture = graphics.loadTexture("cl.jpg");
    if (!bulletTexture || !manTexture || !blockTexture || !backgroundTexture) {
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        graphics.quit();
        return 1;
    }

    // Tải và phát nhạc nền
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

    vector<Bullet> bullets;
    vector<Block> blocks;
    Sprite man;
    man.init(manTexture, MAN_FRAMES, MAN_CLIPS);

    SDL_Texture* scoreTexture = nullptr;
    SDL_Color textColor = {255, 255, 255, 255};
    int lastScore = -1;

    while (!quit) {
        play(bullets, bulletTexture);
        man.tick();
        playb(bullets, blocks, blockTexture);

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

        graphics.prepareScene();
        SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(graphics.renderer, backgroundTexture, nullptr, &backgroundRect);
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

        graphics.presentScene();
        SDL_Delay(75);
    }

    // Kết thúc nhạc
    Mix_HaltMusic();

    // Giải phóng tài nguyên
    if (scoreTexture) {
        SDL_DestroyTexture(scoreTexture);
    }
    Mix_FreeMusic(bgMusic); // <-- Giải phóng nhạc
    Mix_CloseAudio();       // <-- Tắt hệ thống âm thanh
    TTF_CloseFont(font);
    TTF_Quit();
    endg(bulletTexture, manTexture, blockTexture);
    SDL_DestroyTexture(backgroundTexture);
    graphics.quit();
    return 0;
}
