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
bool playedEndSound = false;
std::vector<Gift> gifts;

// Hàm trả về vùng hình chữ nhật của tàu để kiểm tra va chạm
SDL_Rect getManRect() {
    return {manX, manY, MAN_CLIPS[0][2], MAN_CLIPS[0][3]};
}
// initialize.h
float scale = 1.0f;
float scaleDirection = 0.02f; // Tốc độ thay đổi kích thước
const float MIN_SCALE = 0.95f;
const float MAX_SCALE = 1.15f;
bool showSpeedText = false;
Uint32 speedTextStartTime = 0;
const Uint32 SPEED_TEXT_DURATION = 500; // Thời gian hiển thị (ms)
int speedTextX = 0, speedTextY = 0;
// Thêm vào phần biến toàn cục
#endif
