#ifndef _QUIT__H
#define _QUIT__H

using namespace std;
void endg(SDL_Texture* bulletTexture, SDL_Texture* manTexture, SDL_Texture* blockTexture){
    SDL_DestroyTexture(bulletTexture);
    SDL_DestroyTexture(manTexture);
    SDL_DestroyTexture(blockTexture);
}
#endif // _QUIT__H
