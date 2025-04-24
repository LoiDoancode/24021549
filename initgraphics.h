#ifndef _INITGRAPHICS__H
#define _INITGRAPHICS__H
#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include <vector>
#include "graphics.h"

inline void initg(){
    Graphics graphics;
    graphics.init();
}
#endif // _INITGRAPHICS__H
