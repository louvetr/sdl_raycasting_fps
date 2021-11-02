#ifndef MAIN_H
#define MAIN_H

#include <errno.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PId6    0.523598f
#define PId3    1.047197f
#define PI      3.141592f
#define PI2     6.283185f

#define MAP_WIDTH   16
#define MAP_HEIGHT  12

#define BLOCK_SIZE  64

/*const int SCREEN_WIDTH_VRAM = 1024;
const int SCREEN_HEIGHT_VRAM = 32;*/

const int SCREEN_WIDTH = /*720*/ 1280 /*1920*/;
const int SCREEN_HEIGHT = 720;

const float FOV = PI / 3.f;     // TODO set at init
const float rayRadian = FOV / (float) (SCREEN_WIDTH /*/ 2*/); // TODO: change when no more split screen

const int distPlayer = (int)((float)(SCREEN_WIDTH /*/ 2*/) / tanf(PI/6.f));


typedef struct pointInt {
    int x;
    int y;
} _pointInt;

typedef struct pointFloat {
    float x;
    float y;
} _poinFloat;

static inline int tan2degree(float radians)
{   return (int)(radians * (57.2957795131)); }

#endif