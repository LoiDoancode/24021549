#ifndef _DRAWEAC_H
#define _DRAWEAC_H
void fillCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    for (int w=-radius;w<=radius;w++) {
        for (int h=-radius;h<= radius;h++) {
            if (w*w+h*h<=radius*radius)
                SDL_RenderDrawPoint(renderer, x+w, y+h);
        }
    }
}
void fillElip(SDL_Renderer* drawW,int b,int a,int x, int y){
    for(int i=-b;i<=b;i++)
    for(int j=-a;j<=a;j++){
        if(((pow(i,2)/(b*b))+((pow(j,2))/(a*a)))<=1)
            SDL_RenderDrawPoint(drawW, j+x, i+y);
    }

}
#endif // _DEFS_H
