#ifndef MAIN_H
#define MAIN_H

#define PI  3.141592f
#define PI2 6.283185f

const int SCREEN_WIDTH_VRAM = 1024;
const int SCREEN_HEIGHT_VRAM = 32;

const int SCREEN_WIDTH = 720 /*1280*/;
const int SCREEN_HEIGHT = 720;


typedef struct pointInt {
    int x;
    int y;
} _pointInt;

typedef struct pointFloat {
    float x;
    float y;
} _poinFloat;

#endif