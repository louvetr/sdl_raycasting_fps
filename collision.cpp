
#include "collision.hpp"


int collisionIsWall(int x, int y, mapObj *map)
{
    if (x >= map->mapWidth || y >= map->mapHeight || x < 0 || y < 0 ||
		map->floors[0].blocks[y * map->mapHeight + x]->type != MBTYPE_WALL) 
        return 0;
    else
        return 1;
}

// TODO: use pointInt for P
static int collisioRayCheckVertical (pointFloat P, float rayAlpha, mapObj *map, pointFloat *C, int *rayDist)
{
    int ret = 1;
    pointFloat B, Bprev;

    int degrees = tan2degree(rayAlpha);
    if(degrees == 90 || degrees == 270) {
        *rayDist = 999999999;
        return 0;
    }

    int face_east = 0;
    if(rayAlpha < PI / 2.f || rayAlpha > 3.f*PI / 2.f) {
        face_east = 1;
        B.x = (int)P.x/BLOCK_SIZE * BLOCK_SIZE + BLOCK_SIZE;
    }
    else{
        B.x = (int) P.x/BLOCK_SIZE * BLOCK_SIZE - 1;
    }
    B.y = P.y + (P.x - B.x) * tanf(rayAlpha);

    int map_x = (int)B.x/BLOCK_SIZE;
    int map_y = (int)B.y/BLOCK_SIZE;
    float Ya = 64.f * tanf(rayAlpha);
    int cpt = 0;
    /*printf("V collision check: faceEast = %d, Ya = %d\n", face_east, Ya);    
    printf("V collision check #%d: B(%d,%d), map(%d,%d)\n", cpt, B.x, B.y, map_x, map_y);    */

    // While B point is not on a wall, compute and test next block
    while (map_x >= MAP_WIDTH || map_y >= MAP_HEIGHT ||
            map_x < 0 || map_y < 0 ||
            map->floors[0].blocks[map_y * map->mapHeight + map_x]->type == MBTYPE_FLOOR) {
        
        Bprev = B;

        if (face_east) {
            B.x = Bprev.x + BLOCK_SIZE;
            B.y = Bprev.y - Ya;
        } else {
            B.x = Bprev.x - BLOCK_SIZE;
            B.y = Bprev.y + Ya;
        }

        map_x = (int)B.x/BLOCK_SIZE;
        map_y = (int)B.y/BLOCK_SIZE;

        cpt++;
        //printf("V collision check #%d: B(%d,%d), map(%d,%d)\n", cpt, B.x, B.y, map_x, map_y);    
        if(cpt > MAP_WIDTH) {
            ret = 0;
            break;
        }
    }

    if (ret) {
        *C = B;
        *rayDist = abs((P.x-B.x)/cosf(rayAlpha));    
        //printf("V collision: B(%d,%d), map(%d,%d), dist = %d\n", B.x, B.y, map_x, map_y, *rayDist);    
    }

    return ret;
}


static int collisioRayCheckHorizontal (pointFloat P, float rayAlpha, mapObj *map, pointFloat *C, int *rayDist)
{
    int ret = 1;

    pointFloat A, Aprev;
    int face_north = 0;

    int degrees = tan2degree(rayAlpha);
    if(degrees == 0 || degrees == 180) {
        *rayDist = 999999999;
        //printf("SKIP H collision since degree = %d !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", degrees);
        return 0;
    }

    // ray faces south
    if (rayAlpha > PI) {
        A.y = (int)P.y/BLOCK_SIZE * BLOCK_SIZE + BLOCK_SIZE;
    }
    // ray faces north
    else {
        A.y = (int)P.y/BLOCK_SIZE * BLOCK_SIZE - 1;
        face_north = 1;
    }

    A.x = P.x + (P.y - A.y) / tanf(rayAlpha);

    int map_x = (int)A.x/BLOCK_SIZE;
    int map_y = (int)A.y/BLOCK_SIZE;

    // Check if 1st block hit by the ray is a wall
    float Xa = (float)BLOCK_SIZE / tanf(rayAlpha);

    int cpt = 0;

    // While A point is not on a wall, compute and test next block
    while (map_x >= MAP_WIDTH || map_y >= MAP_HEIGHT ||
            map_x < 0 || map_y < 0 ||
            //map->floors[0].blocks[map_y * map->mapHeight + map_x].type != MBTYPE_WALL) {
            map->floors[0].blocks[map_y * map->mapHeight + map_x]->type == MBTYPE_FLOOR) {
            //map[map_y][map_x] == 0) {
        
        Aprev = A;

        if (face_north) {
            A.x = Aprev.x + Xa;
            A.y = (int)Aprev.y - BLOCK_SIZE;
        } else {
            A.x = Aprev.x - Xa;
            A.y = (int)Aprev.y + BLOCK_SIZE;
        }

        map_x = (int)(A.x/BLOCK_SIZE);
        map_y = (int)(A.y/BLOCK_SIZE);
        cpt++;

        if(cpt > MAP_WIDTH) {
            ret = 0;
            break;
        }

    }

    if (ret) {
        *C = A;  
        *rayDist = abs((P.y-A.y) / sinf(rayAlpha));
        //printf("H collision: A(%d,%d), map(%d,%d), dist = %d\n", A.x, A.y, map_x, map_y, *rayDist);
    }

    return ret;
}

enum collision_ray_type collisioRayCheck (pointFloat P, float rayAlpha, mapObj *map, pointFloat *C, int *dist)
{
    enum collision_ray_type ret = COLLISION_RAY_NONE;

    pointFloat H, V;
    int distH, distV;
    int colliH = collisioRayCheckHorizontal (P, rayAlpha, map, &H, &distH);
    int colliV = collisioRayCheckVertical (P, rayAlpha, map, &V, &distV);
    /*int*/ //colliH = 0;

    // TODO; simply identify the nearest collision point
    if(colliH && colliV) {
        if (distH < distV) {
            //printf("distH < distV\n");
            *C = H;
            *dist = distH;
            ret = COLLISION_RAY_HORIZONTAL;
        } else {
            //printf("distH >= distV\n");
            *C = V;
            *dist = distV;
            ret = COLLISION_RAY_VERTICAL;
        }
    }
    else if (colliH) {
        *C = H;
        *dist = distH;
        ret = COLLISION_RAY_HORIZONTAL;
    } else if(colliV) {
        *C = V;
        *dist = distV;
        ret = COLLISION_RAY_VERTICAL;
    } else {
        printf("SHALL NOT HAPPEN !!!!\n");
    }

    /*printf("Collision type : (H, V) = (%d, %d), C(%d, %d), dist(%d, %d)\n",
            colliH, colliV, C->x, C->y, distH, distV);*/


    return ret;
}
