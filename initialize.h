#ifndef _INITIALIZE__H
#define _INITIALIZE__H
#include <vector>
#include "gift.h"
#include "defs.h"

int score = 0;
int blockSpeed = 5;
int speed = 15; // Tốc độ tàu, sẽ tăng khi bắn trúng hoặc chạm gift

bool quit = false;
SDL_Event e;
int manX = 100, manY = 600;
int manSpeedX = 0, manSpeedY = 0;
int frameCount = 0;
bool playedEndSound = false; // Kiểm soát phát âm thanh khi thua
std::vector<Gift> gifts; // Vector lưu trữ các gift

// Hàm trả về vùng hình chữ nhật của tàu để kiểm tra va chạm
SDL_Rect getManRect() {
    return {manX, manY, MAN_CLIPS[0][2], MAN_CLIPS[0][3]}; // Kích thước từ MAN_CLIPS: 300x340
}

#endif
