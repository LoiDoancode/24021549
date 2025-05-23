#ifndef ASSETS_H
#define ASSETS_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "graphics.h"
bool initAssets(
    Graphics& graphics,
    TTF_Font*& font,
    SDL_Texture*& bulletTexture,
    SDL_Texture*& manTexture,
    SDL_Texture*& blockTexture,
    SDL_Texture*& backgroundTexture,
    Mix_Music*& bgMusic,
    Mix_Chunk*& gunSound,
    Mix_Chunk*& popSound
) {
    graphics.init();
    if (graphics.renderer == nullptr || graphics.window == nullptr) {
        SDL_Log("Graphics initialization failed!");
        return false;
    }
    if (TTF_Init() == -1) {
        SDL_Log("SDL_ttf init failed: %s", TTF_GetError());
        return false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("SDL_mixer init failed: %s", Mix_GetError());
        TTF_Quit();
        return false;
    }
    font = TTF_OpenFont("Purisa-BoldOblique.ttf", 24);
    if (!font) {
        SDL_Log("Font load failed: %s", TTF_GetError());
        Mix_CloseAudio(); TTF_Quit(); return false;
    }
    bulletTexture = graphics.loadTexture("bullet.png");
    manTexture = graphics.loadTexture("spaceship.png");
    blockTexture = graphics.loadTexture("skulls.png");
    backgroundTexture = graphics.loadTexture("cl.jpg");

    if (!bulletTexture || !manTexture || !blockTexture || !backgroundTexture) {
        SDL_Log("One or more textures failed to load.");
        TTF_CloseFont(font); Mix_CloseAudio(); TTF_Quit(); return false;
    }
    bgMusic = Mix_LoadMUS("Music.mp3");
    gunSound = Mix_LoadWAV("gun.mp3");
    popSound = Mix_LoadWAV("pop.mp3");
    if (!bgMusic || !gunSound || !popSound) {
        SDL_Log("One or more sounds failed to load.");
        if (bgMusic) Mix_FreeMusic(bgMusic);
        if (gunSound) Mix_FreeChunk(gunSound);
        if (popSound) Mix_FreeChunk(popSound);
        TTF_CloseFont(font); Mix_CloseAudio(); TTF_Quit(); return false;
    }
    Mix_PlayMusic(bgMusic, -1);
    return true;
}
#endif
