#ifndef MAIN_H
#define MAIN_H

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PI  3.141592f
#define PI2 6.283185f

const int SCREEN_WIDTH_VRAM = 1024;
const int SCREEN_HEIGHT_VRAM = 32;

const int SCREEN_WIDTH = 720 /*1280*/;
const int SCREEN_HEIGHT = 720;


#define MAP_WIDTH   10
#define MAP_HEIGHT  10

#define BLOCK_SIZE  64


typedef struct pointInt {
    int x;
    int y;
} _pointInt;

typedef struct pointFloat {
    float x;
    float y;
} _poinFloat;

#endif