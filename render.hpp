#ifndef RENDER_H
#define RENDER_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "collision.hpp"

int renderRay(SDL_Renderer *renderer, enum collision_ray_type colliType, int dist, int X);
int renderRayTextured(SDL_Renderer *renderer, enum collision_ray_type colliType, int dist, int X, pointFloat *C, Uint8 *pixelsWall);

#endif
