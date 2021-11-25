
#include "collision.hpp"


int collisionIsWall(int x, int y, mapObj *map)
{
	if (x >= map->mapWidth || y >= map->mapHeight || x < 0 || y < 0 ||
	    map->floors[0].blocks[y * map->mapHeight + x]->type != MBTYPE_WALL)
		return 0;
	else
		return 1;
}

static enum collision_ray_type collisioRayCheckSpecial(pointFloat P, float rayAlpha, enum mapBlockType blockType, pointFloat *C, enum collision_ray_type *pColli)
{
	enum collision_ray_type colliType = COLLISION_RAY_NONE;
	pointFloat Ctmp;

	switch(blockType) {
		case MBTYPE_FLOOR:
			colliType = COLLISION_RAY_NONE;
			break;

		case MBTYPE_WALL_DIAG_TL2BR: 
		{
			//Ctmp.x = floor(fmodf(C->x, BLOCK_SIZE));	// TODO: retrieve from H,V colli fct
			//Ctmp.y = floor(fmodf(C->y, BLOCK_SIZE));
			Ctmp.x = fmodf(C->x, BLOCK_SIZE);	// TODO: retrieve from H,V colli fct
			Ctmp.y = fmodf(C->y, BLOCK_SIZE);
			float m = (C->y - P.y) / (C->x - P.x);
			float b = Ctmp.y - m * Ctmp.x;
			float offset = b / (1 - m);
			float deltaX = offset - Ctmp.x;
			float deltaY = offset - Ctmp.y;

			Ctmp.x += deltaX;
			Ctmp.y += deltaY;

			if(Ctmp.x <= BLOCK_SIZE && Ctmp.y <= BLOCK_SIZE && Ctmp.x >= 0 && Ctmp.y >= 0) {
				C->x += deltaX;
				C->y += deltaY;
				colliType = COLLISION_RAY_DIAG_TL2BR;
			}
			else {
				colliType = COLLISION_RAY_NONE;
			}
			break;
		}

		case MBTYPE_WALL_DIAG_BL2TR: 
		{
			Ctmp.x = fmodf(C->x, BLOCK_SIZE);	// TODO: retrieve from H,V colli fct
			Ctmp.y = fmodf(C->y, BLOCK_SIZE);
			float m = (C->y - P.y) / (C->x - P.x);
			float b = Ctmp.y - m * Ctmp.x;
			float Xoffset = (BLOCK_SIZE - b) / (m + 1);
			float Yoffset = BLOCK_SIZE - Xoffset;
			float deltaX = Xoffset - Ctmp.x;
			float deltaY = Yoffset - Ctmp.y;

			Ctmp.x += deltaX;
			Ctmp.y += deltaY;

			if(Ctmp.x <= BLOCK_SIZE && Ctmp.y <= BLOCK_SIZE && Ctmp.x >= 0 && Ctmp.y >= 0) {
				C->x += deltaX;
				C->y += deltaY;
				colliType = COLLISION_RAY_DIAG_BL2TR;
			}
			else {
				colliType = COLLISION_RAY_NONE;
			}
			break;
		}

		default: 
			colliType = COLLISION_RAY_SQUARE;
	}

	*pColli = colliType;
	return colliType;

}


// TODO: use pointInt for P
static enum collision_ray_type collisioRayCheckVertical(pointFloat P, float rayAlpha, mapObj *map, pointFloat *C, int *rayDist, int *mapBlockId)
{
	int ret = 1;
	enum collision_ray_type colliType = COLLISION_RAY_NONE;
	pointFloat B, Bprev;
		
	int degrees = tan2degree(rayAlpha);
	if (degrees == 90 || degrees == 270) {
		*rayDist = 999999999;
		return colliType;
	}

	int face_east = 0;
	if (rayAlpha < PI / 2.f || rayAlpha > 3.f * PI / 2.f) {
		face_east = 1;
		B.x = (int)P.x / BLOCK_SIZE * BLOCK_SIZE + BLOCK_SIZE;
	} else {
		B.x = (int)P.x / BLOCK_SIZE * BLOCK_SIZE - 1;
	}
	B.y = P.y + (P.x - B.x) * tanf(rayAlpha);

	int map_x = (int)B.x / BLOCK_SIZE;
	int map_y = (int)B.y / BLOCK_SIZE;
	float Ya = 64.f * tanf(rayAlpha);
	int cpt = 0;
	int blockIdx = map_y * map->mapHeight + map_x;

	// While B point is not on a wall, compute and test next block
	while (map_x >= MAP_WIDTH || map_y >= MAP_HEIGHT || map_x < 0 || map_y < 0 ||
	       collisioRayCheckSpecial(P, rayAlpha, map->floors[0].blocks[blockIdx]->type, &B, &colliType ) == COLLISION_RAY_NONE ) {

		if(colliType != COLLISION_RAY_NONE)
			SDL_Log("[%s:%d] colliType != COLLISION_RAY_NONE", __func__, __LINE__);

		Bprev = B;

		if (face_east) {
			B.x = Bprev.x + BLOCK_SIZE;
			B.y = Bprev.y - Ya;
		} else {
			B.x = Bprev.x - BLOCK_SIZE;
			B.y = Bprev.y + Ya;
		}

		map_x = (int)B.x / BLOCK_SIZE;
		map_y = (int)B.y / BLOCK_SIZE;

		cpt++;
		if (cpt > MAP_WIDTH) {
			ret = 0;
			break;
		}

		blockIdx = map_y * map->mapHeight + map_x;

	}

	if (ret || colliType != COLLISION_RAY_NONE) {

		if(colliType == COLLISION_RAY_SQUARE) 
			colliType = COLLISION_RAY_VERTICAL;

		*C = B;
		*rayDist = abs((P.x - B.x) / cosf(rayAlpha));
		*mapBlockId = blockIdx;
	}

	return colliType;
}


static enum collision_ray_type collisioRayCheckHorizontal(pointFloat P, float rayAlpha, mapObj *map, pointFloat *C, int *rayDist, int *mapBlockId)
{
	int ret = 1;
	enum collision_ray_type colliType = COLLISION_RAY_NONE;
	pointFloat A, Aprev;
	int face_north = 0;
	int degrees = tan2degree(rayAlpha);

	if (degrees == 0 || degrees == 180) {
		*rayDist = 999999999;
		return colliType;
	}

	// ray faces south
	if (rayAlpha > PI) {
		A.y = (int)P.y / BLOCK_SIZE * BLOCK_SIZE + BLOCK_SIZE;
	}
	// ray faces north
	else {
		A.y = (int)P.y / BLOCK_SIZE * BLOCK_SIZE - 1;
		face_north = 1;
	}

	A.x = P.x + (P.y - A.y) / tanf(rayAlpha);

	int map_x = (int)A.x / BLOCK_SIZE;
	int map_y = (int)A.y / BLOCK_SIZE;

	// Check if 1st block hit by the ray is a wall
	float Xa = (float)BLOCK_SIZE / tanf(rayAlpha);
	int cpt = 0;
	int blockIdx = map_y * map->mapHeight + map_x;

	// While A point is not on a wall, compute and test next block
	while (map_x >= MAP_WIDTH || map_y >= MAP_HEIGHT || map_x < 0 || map_y < 0 ||
	       collisioRayCheckSpecial(P, rayAlpha, map->floors[0].blocks[blockIdx]->type, &A, &colliType ) == COLLISION_RAY_NONE ) {

		if(colliType != COLLISION_RAY_NONE)
			SDL_Log("[%s:%d] colliType != COLLISION_RAY_NONE", __func__, __LINE__);

		Aprev = A;

		if (face_north) {
			A.x = Aprev.x + Xa;
			A.y = (int)Aprev.y - BLOCK_SIZE;
		} else {
			A.x = Aprev.x - Xa;
			A.y = (int)Aprev.y + BLOCK_SIZE;
		}

		map_x = (int)(A.x / BLOCK_SIZE);
		map_y = (int)(A.y / BLOCK_SIZE);
		cpt++;

		if (cpt > MAP_WIDTH) {
			ret = 0;
			break;
		}

		blockIdx = map_y * map->mapHeight + map_x;
	}

	if (ret || colliType != COLLISION_RAY_NONE) {
		if(colliType == COLLISION_RAY_SQUARE) 
			colliType = COLLISION_RAY_HORIZONTAL;

		*C = A;
		*rayDist = abs((P.y - A.y) / sinf(rayAlpha));
		*mapBlockId = blockIdx;
	}

	return colliType;
}

enum collision_ray_type collisioRayCheck(pointFloat P, float rayAlpha, mapObj *map, pointFloat *C, int *dist)
{
	enum collision_ray_type ret = COLLISION_RAY_NONE;

	pointFloat H, V;
	int distH, distV;
	int mapBlockIdH, mapBlockIdV, mapBlockId;
	enum collision_ray_type colliH = collisioRayCheckHorizontal(P, rayAlpha, map, &H, &distH, &mapBlockIdH);
	enum collision_ray_type colliV = collisioRayCheckVertical(P, rayAlpha, map, &V, &distV, &mapBlockIdV);

	// TODO; simply identify the nearest collision point
	if (colliH && colliV) {
		if (distH < distV) {
			*C = H;
			*dist = distH;
			ret = colliH;
			mapBlockId = mapBlockIdH;
		} else {
			*C = V;
			*dist = distV;
			ret = colliV;
			mapBlockId = mapBlockIdV;
		}
	} else if (colliH) {
		*C = H;
		*dist = distH;
		ret = colliH;
		mapBlockId = mapBlockIdH;
	} else if (colliV) {
		*C = V;
		*dist = distV;
		ret = colliV;
		mapBlockId = mapBlockIdV;
	} else {
		printf("SHALL NOT HAPPEN !!!!\n");
	}

	return ret;
}
