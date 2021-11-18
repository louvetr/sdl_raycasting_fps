
#include "collision.hpp"


int collisionIsWall(int x, int y, mapObj *map)
{
	if (x >= map->mapWidth || y >= map->mapHeight || x < 0 || y < 0 ||
	    map->floors[0].blocks[y * map->mapHeight + x]->type != MBTYPE_WALL)
		return 0;
	else
		return 1;
}

static enum collision_ray_type collisioRayCheckSpecial(pointFloat P, float rayAlpha, enum mapBlockType blockType, pointFloat *C/*, int *rayDist, int *mapBlockId*/)
{
	enum collision_ray_type colliType = COLLISION_RAY_NONE;
	pointFloat Ctmp;

	switch(blockType) {
		case MBTYPE_WALL_DIAG_BL2TR:
			Ctmp.x = fmodf(C->x, BLOCK_SIZE);	// TODO: retrieve from H,V colli fct
			Ctmp.y = fmodf(C->y, BLOCK_SIZE);
			float m = (C->y - P.y) / (C->x - P.x);
			float b = Ctmp.y - m * Ctmp.x;
			float offset = b / (1 - m);
			float deltaX = offset - Ctmp.x;
			float deltaY = offset - Ctmp.y;

			float maxDeltaX = 0.f;
			float maxDeltaY = 0.f;
			float maxOffset = 0.f;

			if(deltaX > maxDeltaX)
				maxDeltaX = deltaX;
			if(deltaY > maxDeltaY)
				maxDeltaY = deltaY;
			if(offset > maxOffset)
				maxOffset = offset;

			SDL_Log("maxOffset = %f, maxDelta(%f, %f)", maxOffset, maxDeltaX, maxDeltaY);

			/*SDL_Log("ENTER case with *dist = %d", *dist);*/

			SDL_Log("C(%f, %f), Ctmp(%f, %f), m = %f, b = %f, offset = %f => Cnew(%f, %f)",
				C->x, C->y, Ctmp.x, Ctmp.y, m, b, offset, C->x + deltaX, C->y + deltaY);

			if(abs(offset) < BLOCK_SIZE) {
				C->x += deltaX;
				C->y += deltaY;
				//*dist += sqrtf(powf(Ctmp.x - offset, 2) +  powf(Ctmp.y - offset, 2));

				/*float deltaDist = sqrtf(powf(deltaX, 2) +  powf(deltaY, 2));
				float deltaFloat = (float)(*dist);
				float deltaSum = deltaFloat + deltaDist;

				SDL_Log("Cnew(%f, %f)", C->x, C->y);*/
				/*SDL_Log("IN case with *dist = %d", *dist);
				SDL_Log("powf(%f, 2) = %f, powf(%f, 2) = %f, sum = %f, deltaDist = %f",
						deltaX, powf(deltaX, 2), deltaY, powf(deltaY, 2), powf(deltaX, 2) +  powf(deltaY, 2), deltaDist);
*/
/*				SDL_Log("deltaDist = %f, *dist = %f, distNew = %f, distNewInt = %d, deltaFloat = %f, deltaSum = %f",
					deltaDist, *dist, (float)*dist + deltaDist, *dist + (int)deltaDist, deltaFloat, deltaSum);

				SDL_Log("deltaXY(%f, %f), deltaDist = %f, *dist = %f, distNew = %f, distNewInt = %d, deltaFloat = %f, deltaSum = %f",
					deltaX, deltaY, deltaDist, *dist, (float)*dist + deltaDist, *dist + (int)deltaDist, deltaFloat, deltaSum);
				*/
				//*dist += deltaDist;
				/**dist += (int)deltaDist;
				ret = COLLISION_RAY_DIAG_BL2TR;*/
			
				//SDL_Log("EXIT case with *dist = %d", *dist);
			
			}
			else
				SDL_Log("SKIP this diag");

			colliType = COLLISION_RAY_SPECIAL;

			break;
		
		/*default: 
			colliType = COLLISION_RAY_NONE;*/
	}

	//return colliType;
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
	/*printf("V collision check: faceEast = %d, Ya = %d\n", face_east, Ya);
	printf("V collision check #%d: B(%d,%d), map(%d,%d)\n", cpt, B.x, B.y, map_x, map_y);    */

	int blockIdx = map_y * map->mapHeight + map_x;

	// While B point is not on a wall, compute and test next block
	while (map_x >= MAP_WIDTH || map_y >= MAP_HEIGHT || map_x < 0 || map_y < 0 ||
	       map->floors[0].blocks[blockIdx]->type == MBTYPE_FLOOR) {

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
		// printf("V collision check #%d: B(%d,%d), map(%d,%d)\n", cpt, B.x, B.y, map_x, map_y);
		if (cpt > MAP_WIDTH) {
			ret = 0;
			break;
		}
		blockIdx = map_y * map->mapHeight + map_x;
	}

	if (ret) {

		colliType = collisioRayCheckSpecial(P, rayAlpha, map->floors[0].blocks[blockIdx]->type, &B /*, rayDist, blockIdx*/); 
		if(colliType == COLLISION_RAY_NONE) 
			colliType = COLLISION_RAY_VERTICAL;

		*C = B;
		*rayDist = abs((P.x - B.x) / cosf(rayAlpha));
		// printf("V collision: B(%d,%d), map(%d,%d), dist = %d\n", B.x, B.y, map_x, map_y, *rayDist);
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
		// printf("SKIP H collision since degree = %d !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", degrees);
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
	       // map->floors[0].blocks[map_y * map->mapHeight + map_x].type != MBTYPE_WALL) {
	       map->floors[0].blocks[blockIdx]->type == MBTYPE_FLOOR) {
		// map[map_y][map_x] == 0) {

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

		blockIdx = map_y * map->mapHeight + map_x;

		if (cpt > MAP_WIDTH) {
			ret = 0;
			break;
		}
	}

	if (ret) {
		colliType = collisioRayCheckSpecial(P, rayAlpha, map->floors[0].blocks[blockIdx]->type, &A /*, rayDist, blockIdx*/); 
		if(colliType == COLLISION_RAY_NONE) 
			colliType = COLLISION_RAY_HORIZONTAL;

		*C = A;
		*rayDist = abs((P.y - A.y) / sinf(rayAlpha));
		// printf("H collision: A(%d,%d), map(%d,%d), dist = %d\n", A.x, A.y, map_x, map_y, *rayDist);
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
			// printf("distH < distV\n");
			*C = H;
			*dist = distH;
			SDL_Log("== *dist = distH = %d", distH);
			ret = colliH;
			mapBlockId = mapBlockIdH;
		} else {
			// printf("distH >= distV\n");
			*C = V;
			*dist = distV;
			SDL_Log("== *dist = distV = %d", distV);
			ret = colliV;
			mapBlockId = mapBlockIdV;
		}
	} else if (colliH) {
		*C = H;
		*dist = distH;
		SDL_Log("*dist = distH = %d", distH);
		ret = colliH;
		mapBlockId = mapBlockIdH;
	} else if (colliV) {
		*C = V;
		*dist = distV;
		SDL_Log("*dist = distV = %d", distV);
		ret = colliV;
		mapBlockId = mapBlockIdV;
	} else {
		printf("SHALL NOT HAPPEN !!!!\n");
	}

	/*printf("Collision type : (H, V) = (%d, %d), C(%d, %d), dist(%d, %d)\n",
		colliH, colliV, C->x, C->y, distH, distV);*/


	return ret;
}
