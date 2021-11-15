#ifndef RENDER_H
#define RENDER_H


#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>

#include "collision.hpp"

int renderRay(SDL_Renderer *renderer, enum collision_ray_type colliType, int dist, int X);
int renderRayTextured(SDL_Renderer *renderer,
		      enum collision_ray_type colliType,
		      int dist,
		      int X,
		      pointFloat *C,
		      mapObj *map,
		      pointFloat playerPoint,
		      float rayAlpha,
		      float subAlpha);
int renderMinimap(SDL_Renderer *renderer, Player *player, mapObj *map, Uint8 map_scale);
int renderMinimapRay(SDL_Renderer *renderer,
		     Player *player,
		     pointFloat *C,
		     float rayAlpha,
		     enum collision_ray_type colliType,
		     Uint8 map_scale);

#endif
