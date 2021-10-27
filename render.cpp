#include "render.hpp"


int Xoffset = SCREEN_WIDTH / 2;

uint8_t wallColor = 0xA0;

int renderRay(SDL_Renderer *renderer, enum collision_ray_type colliType, int dist, int X)
{

    if (colliType == COLLISION_RAY_NONE)
        return -1;

    int heightProjected = BLOCK_SIZE * distPlayer / dist;

    int Ya = (SCREEN_HEIGHT - heightProjected) / 2;
    int Yb = (SCREEN_HEIGHT + heightProjected) / 2;

    uint8_t colorOffset = - dist / 4;

    if (colliType != COLLISION_RAY_HORIZONTAL)
		SDL_SetRenderDrawColor(renderer, 0xA0 + colorOffset, 0xFF + colorOffset, 0xA0 + colorOffset, 0xff);
    else
		SDL_SetRenderDrawColor(renderer, 0xA0 + colorOffset, 0xA0 + colorOffset, 0xFF + colorOffset, 0xff);


    SDL_RenderDrawLine(renderer, X + Xoffset, Ya, X + Xoffset, Yb);
}