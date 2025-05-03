#ifndef _GAME_STATE__H
#define _GAME_STATE__H
#include <SDL.h>
#include <SDL_ttf.h>
#include "graphics.h"
#include "bullets.h"
#include "block.h"
#include "playb.h"
#include "resource_init.h"
#include "text_init.h"
#include "initialize.h"
using namespace std;

void updateGameState(TextComponents& comp, Resources& res, Graphics& graphics,
                     vector<Bullet>& bullets, vector<Block>& blocks) {
    if (comp.gameState == 1) {
        comp.man.tick();
        playb(bullets, blocks, res.blockTexture, res.popSound, comp.gameState);

        if (score != comp.lastScore) {
            if (comp.scoreTexture) {
                SDL_DestroyTexture(comp.scoreTexture);
            }
            string scoreText = "Score: " + to_string(score);
            SDL_Surface* textSurface = TTF_RenderText_Solid(res.font, scoreText.c_str(), comp.textColor);
            if (!textSurface) {
                SDL_Log("Unable to render text surface! TTF_Error: %s", TTF_GetError());
            } else {
                comp.scoreTexture = SDL_CreateTextureFromSurface(graphics.renderer, textSurface);
                SDL_FreeSurface(textSurface);
            }
            comp.lastScore = score;
        }
    }
    else if (comp.gameState == 2) {
        if (comp.gameOverScoreTexture) {
            SDL_DestroyTexture(comp.gameOverScoreTexture);
        }
        string scoreText = "Score: " + to_string(score);
        SDL_Surface* textSurface = TTF_RenderText_Solid(res.font, scoreText.c_str(), comp.textColor);
        if (!textSurface) {
            SDL_Log("Unable to render game over score text surface! TTF_Error: %s", TTF_GetError());
        } else {
            comp.gameOverScoreTexture = SDL_CreateTextureFromSurface(graphics.renderer, textSurface);
            SDL_FreeSurface(textSurface);
        }
    }
}
#endif
