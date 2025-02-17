#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include<cmath>
#include "defs.h"
#include "drawEaC.h"
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
    SDL_Window* openW=SDL_CreateWindow("HELLO", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_SHOWN);
    if(openW==nullptr)LogErrorAndExit("FalseW", SDL_GetError());
    if(!IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG))LogErrorAndExit("imgError",IMG_GetError());
    return openW;
}
SDL_Renderer* CreateR(SDL_Window* openW){
    SDL_Renderer* drawW=SDL_CreateRenderer(openW,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(drawW==nullptr)LogErrorAndExit("FalseR",SDL_GetError());
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"best");
    SDL_RenderSetLogicalSize(drawW, width, height);
    return drawW;

}
SDL_Texture* CreatT(SDL_Renderer* drawW, const char* filename){
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_INFO,"image",filename);
    SDL_Texture* file=IMG_LoadTexture(drawW, filename);
    if(file==NULL)SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_ERROR,"fileError",IMG_GetError());
    return file;
}
void SDLText(SDL_Renderer* drawW, int x, int y, SDL_Texture* Text){
    SDL_Rect dest;
    dest.x=x;
    dest.y=y;
    dest.w=150;
    dest.h=150;
   // SDL_QueryTexture(Text,NULL,NULL,&dest.w,&dest.h);
    SDL_RenderCopy(drawW,Text,NULL,&dest);
}
void DrawSt(SDL_Window* openW, SDL_Renderer* drawW){
    SDL_SetRenderDrawColor(drawW,0,200,0,255);
    SDL_RenderDrawPoint(drawW,0,0);
    SDL_Rect filled_rect;
    filled_rect.x=100;
    filled_rect.y=100;
    filled_rect.w=100;
    filled_rect.h=100;
    SDL_SetRenderDrawColor(drawW,255,255,255,255);
    SDL_RenderFillRect(drawW, &filled_rect);
    SDL_SetRenderDrawColor(drawW,255,0,0,255);
    SDL_RenderDrawLine(drawW,100,100,200,200);
}
void QuitAndExit(SDL_Window* openW, SDL_Renderer* drawW){
    IMG_Quit();
    SDL_DestroyWindow(openW);
    SDL_DestroyRenderer(drawW);
    SDL_Quit();

}
int main(int argc, char* argv[]){
    SDL_Window* openW=CreateW(width, height);
    SDL_Renderer* drawW=CreateR(openW);
    SDL_Texture* background=CreatT(drawW,"bikiniBottom.jpg");
    SDL_RenderCopy(drawW,background,NULL,NULL);
    SDL_RenderPresent(drawW);

    SDL_RenderPresent(drawW);
    //SDL_RenderClear(drawW);
    SDL_SetRenderDrawColor(drawW, 255, 255, 255, 255);
    fillElip(drawW,50,100,150,150);
    fillElip(drawW,50,100,450,150);
    fillElip(drawW,80,150,300,500);
    SDL_Texture* bob=CreatT(drawW,"Spongebob.png");
    SDLText(drawW,220,440,bob);
    SDL_SetRenderDrawColor(drawW, 255, 0, 0, 255);
    fillCircle(drawW,150,150,50);
    fillCircle(drawW,450,150,50);
    SDL_RenderDrawLine(drawW,150,170,150,400);
    SDL_RenderDrawLine(drawW,450,170,450,370);
    //SDL_RenderDrawLine(drawW,40,400,560,400);
    SDL_SetRenderDrawColor(drawW, 0, 0, 0, 255);

    fillCircle(drawW,150,150,20);
    fillCircle(drawW,450,150,20);
    SDL_RenderPresent(drawW);

  //  DrawSt(openW, drawW);
   // SDL_RenderPresent(drawW);*/
    Wait();
    SDL_DestroyTexture(bob);
    bob=nullptr;
    SDL_DestroyTexture(background);
    background=nullptr;
    QuitAndExit(openW, drawW);
}
