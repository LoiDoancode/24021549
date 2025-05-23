#ifndef _INIT__H
#define _INIT__H
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
#include "initialize.h"
using namespace std;

struct GameResources {
    Graphics graphics;
    TTF_Font* font = nullptr;
    SDL_Texture* bulletTexture = nullptr;
    SDL_Texture* manTexture = nullptr;
    SDL_Texture* blockTexture = nullptr;
    SDL_Texture* backgroundTexture = nullptr;
    Mix_Music* bgMusic = nullptr;
    Mix_Chunk* gunSound = nullptr;
    Mix_Chunk* popSound = nullptr;
    vector<Bullet> bullets;
    vector<Block> blocks;
    Sprite man;
};

bool initGame(GameResources& res) {
    srand(time(0));
    res.graphics.init();

    // Khởi tạo SDL_ttf
    if (TTF_Init() == -1) {
        SDL_Log("SDL_ttf could not initialize! TTF_Error: %s", TTF_GetError());
        res.graphics.quit();
        return false;
    }

    // Khởi tạo SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("SDL_mixer could not initialize! Mix_Error: %s", Mix_GetError());
        TTF_Quit();
        res.graphics.quit();
        return false;
    }

    // Tải font
    res.font = TTF_OpenFont("Purisa-BoldOblique.ttf", 24);
    if (!res.font) {
        SDL_Log("Failed to load font! TTF_Error: %s", TTF_GetError());
        Mix_CloseAudio();
        TTF_Quit();
        res.graphics.quit();
        return false;
    }

    // Tải các texture
    res.bulletTexture = res.graphics.loadTexture("bullet.png");
    if (!res.bulletTexture) {
        SDL_Log("Failed to load bullet.png: %s", IMG_GetError());
        TTF_CloseFont(res.font);
        Mix_CloseAudio();
        TTF_Quit();
        res.graphics.quit();
        return false;
    }
    res.manTexture = res.graphics.loadTexture("spaceship.png");
    if (!res.manTexture) {
        SDL_Log("Failed to load spaceship.png: %s", IMG_GetError());
        TTF_CloseFont(res.font);
        Mix_CloseAudio();
        TTF_Quit();
        res.graphics.quit();
        return false;
    }
    res.blockTexture = res.graphics.loadTexture("skulls.png");
    if (!res.blockTexture) {
        SDL_Log("Failed to load skulls.png: %s", IMG_GetError());
        TTF_CloseFont(res.font);
        Mix_CloseAudio();
        TTF_Quit();
        res.graphics.quit();
        return false;
    }
    res.backgroundTexture = res.graphics.loadTexture("cl.jpg");
    if (!res.backgroundTexture) {
        SDL_Log("Failed to load cl.jpg: %s", IMG_GetError());
        TTF_CloseFont(res.font);
        Mix_CloseAudio();
        TTF_Quit();
        res.graphics.quit();
        return false;
    }

    // Tải nhạc nền
    res.bgMusic = Mix_LoadMUS("Music.mp3");
    if (!res.bgMusic) {
        SDL_Log("Failed to load background music! Mix_Error: %s", Mix_GetError());
        TTF_CloseFont(res.font);
        Mix_CloseAudio();
        TTF_Quit();
        res.graphics.quit();
        return false;
    }
    Mix_PlayMusic(res.bgMusic, -1); // Lặp vô hạn

    // Tải âm thanh bắn
    res.gunSound = Mix_LoadWAV("gun.mp3");
    if (!res.gunSound) {
        SDL_Log("Failed to load gun.mp3: %s", Mix_GetError());
        Mix_FreeMusic(res.bgMusic);
        TTF_CloseFont(res.font);
        Mix_CloseAudio();
        TTF_Quit();
        res.graphics.quit();
        return false;
    }

    // Tải âm thanh khi bắn trúng block
    res.popSound = Mix_LoadWAV("pop.mp3");
    if (!res.popSound) {
        SDL_Log("Failed to load pop.mp3: %s", Mix_GetError());
        Mix_FreeChunk(res.gunSound);
        Mix_FreeMusic(res.bgMusic);
        TTF_CloseFont(res.font);
        Mix_CloseAudio();
        TTF_Quit();
        res.graphics.quit();
        return false;
    }

    // Khởi tạo sprite tàu
    res.man.init(res.manTexture, MAN_FRAMES, MAN_CLIPS);
    return true;
}
#endif
