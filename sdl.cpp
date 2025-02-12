#include <SDL.h>
#include <iostream>
#include <SDL_image.h>

int width=600;
int height=800;
void LogErrorAndExit(const char* mb, const char* kb){
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "FALSE", mb, kb);
    SDL_Quit();
}
void Wait(){
    SDL_Event e;
    while(true){
        if(SDL_PollEvent(&e)!=0&&(e.type==SDL_KEYDOWN||e.type==SDL_QUIT))
            return;
        SDL_Delay(100);
    }
}
SDL_Window* CreateW(int width, int height){
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0)LogErrorAndExit("FalseW", SDL_GetError());
    SDL_Window* openW=SDL_CreateWindow("HELLO", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if(openW==nullptr)LogErrorAndExit("FalseW", SDL_GetError());
    return openW;
}
SDL_Renderer* CreateR(SDL_Window* openW){
    SDL_Renderer* drawW=SDL_CreateRenderer(openW,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(drawW==nullptr)LogErrorAndExit("FalseR",SDL_GetError());
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"best");
    SDL_RenderSetLogicalSize(drawW, width, height);
    return drawW;

}
void DrawSt(SDL_Window* openW, SDL_Renderer* drawW){
    SDL_SetRenderDrawColor(drawW,0,200,0,255);
    SDL_RenderDrawPoint(drawW,0,0);
    SDL_Rect filled_rect;
    filled_rect.x=100;
    filled_rect.y=100;
    filled_rect.w=100;
    filled_rect.h=100;
    SDL_SetRenderDrawColor(drawW,255,255,0,255);
    SDL_RenderDrawRect(drawW, &filled_rect);
    SDL_SetRenderDrawColor(drawW,255,0,0,255);
    SDL_RenderDrawLine(drawW,100,100,200,200);
}
void QuitAndExit(SDL_Window* openW, SDL_Renderer* drawW){
    SDL_DestroyWindow(openW);
    SDL_DestroyRenderer(drawW);
    SDL_Quit();
}
int main(int argc, char* argv[]){
    SDL_Window* openW=CreateW(width, height);
    SDL_Renderer* drawW=CreateR(openW);
    SDL_RenderClear(drawW);
    DrawSt(openW, drawW);
    SDL_RenderPresent(drawW);
    Wait();
    QuitAndExit(openW, drawW);
}
