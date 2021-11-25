#ifndef COLLISION_H
#define COLLISION_H

#include "main.hpp"
#include "map.hpp"

enum collision_ray_type {
	COLLISION_RAY_NONE = 0,
	COLLISION_RAY_SQUARE,
	COLLISION_RAY_HORIZONTAL,
	COLLISION_RAY_VERTICAL,
	COLLISION_RAY_SPECIAL,
	COLLISION_RAY_DIAG_BL2TR,
	COLLISION_RAY_DIAG_TL2BR,
};

enum collision_ray_type collisioRayCheck(pointFloat P, float rayAlpha, mapObj *map, pointFloat *C, int *dist);


#endif