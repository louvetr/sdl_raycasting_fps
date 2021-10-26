#ifndef COLLISION_H
#define COLLISION_H

#include "main.hpp"

enum collision_ray_type {
    COLLISION_RAY_NONE = 0,
    COLLISION_RAY_HORIZONTAL,
    COLLISION_RAY_VERTICAL,
};

enum collision_ray_type collisioRayCheck (pointFloat P, float rayAlpha, char map[MAP_WIDTH][MAP_HEIGHT], pointInt *C);


#endif