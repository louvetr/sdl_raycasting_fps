#ifndef RENDER_H
#define RENDER_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "collision.hpp"

int renderRay(SDL_Renderer *renderer, enum collision_ray_type colliType, int dist, int X);
int renderRayTextured(SDL_Renderer *renderer, enum collision_ray_type colliType, int dist, int X, pointFloat *C, Uint8 *pixelsWall);
//int renderMinimap(SDL_Renderer *renderer, pointFloat *P, char map[MAP_HEIGHT][MAP_WIDTH], Uint8 map_scale);
int renderMinimap(SDL_Renderer *renderer, Player *player, char map[MAP_HEIGHT][MAP_WIDTH], Uint8 map_scale);
int renderMinimapRay(SDL_Renderer *renderer, Player *player, pointFloat *C, float rayAlpha, enum collision_ray_type colliType, Uint8 map_scale);

#endif
