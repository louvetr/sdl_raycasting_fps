
#include "collision.hpp"


int collisionIsWall(int x, int y, char map[MAP_HEIGHT][MAP_WIDTH])
{
    if (x >= MAP_WIDTH || y >= MAP_HEIGHT || x < 0 || y < 0 ||
        map[y][x] == 0)
        return 0;
    else
        return 1;
}

// TODO: use pointInt for P
static int collisioRayCheckVertical (pointFloat P, float rayAlpha, char map[MAP_HEIGHT][MAP_WIDTH], pointInt *C, int *rayDist)
{
    int ret = 1;
    pointInt B, Bprev;
    int Xa;

    int face_east = 0;
    if(rayAlpha < PI / 2.f || rayAlpha > 3.f*PI / 2.f) {
        face_east = 1;
        B.x = (int)P.x/BLOCK_SIZE * BLOCK_SIZE + BLOCK_SIZE;
        Xa = BLOCK_SIZE;
    }
    else{
        B.x = (int)P.x/BLOCK_SIZE * BLOCK_SIZE -1;
        Xa = -BLOCK_SIZE;
    }
    B.y = P.y + (P.x - B.x) * tanf(rayAlpha);

    int map_x = (int)(B.x/BLOCK_SIZE);
    int map_y = (int)(B.y/BLOCK_SIZE);
    int Ya = (int)(64.f * tanf(rayAlpha));
    int cpt = 0;
    /*printf("V collision check: faceEast = %d, Ya = %d\n", face_east, Ya);    
    printf("V collision check #%d: B(%d,%d), map(%d,%d)\n", cpt, B.x, B.y, map_x, map_y);    */

    // While B point is not on a wall, compute and test next block
    while (map_x >= MAP_WIDTH || map_y >= MAP_HEIGHT ||
            map_x < 0 || map_y < 0 ||
            map[map_y][map_x] == 0) {
        
        Bprev = B;

        if (face_east) {
            B.x = Bprev.x + BLOCK_SIZE;
            B.y = Bprev.y - Ya;
        } else {
            B.x = Bprev.x - BLOCK_SIZE;
            B.y = Bprev.y + Ya;
        }

        map_x = (int)(B.x/BLOCK_SIZE);
        map_y = (int)(B.y/BLOCK_SIZE);

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


static int collisioRayCheckHorizontal (pointFloat P, float rayAlpha, char map[MAP_HEIGHT][MAP_WIDTH], pointInt *C, int *rayDist)
{
    int ret = 1;

    pointInt A, Aprev;
    int face_north = 0;

    // ray faces south
    if (rayAlpha > PI) {
        A.y = (int)P.y/BLOCK_SIZE * BLOCK_SIZE + BLOCK_SIZE;
    }
    // ray faces north
    else {
        A.y = (int)P.y/BLOCK_SIZE * BLOCK_SIZE - 1;
        face_north = 1;
    }

    A.x = (int)P.x + ((int)P.y - (float)A.y) / tanf(rayAlpha);

    int map_x = (int)(A.x/BLOCK_SIZE);
    int map_y = (int)(A.y/BLOCK_SIZE);

    // Check if 1st block hit by the ray is a wall
    int Xa = (int)((float)BLOCK_SIZE / tanf(rayAlpha));

    int cpt = 0;

    // While A point is not on a wall, compute and test next block
    while (map_x >= MAP_WIDTH || map_y >= MAP_HEIGHT ||
            map_x < 0 || map_y < 0 ||
            map[map_y][map_x] == 0) {
        
        Aprev = A;

        if (face_north) {
            A.x = Aprev.x + Xa;
            A.y = Aprev.y - BLOCK_SIZE;
        } else {
            A.x = Aprev.x - Xa;
            A.y = Aprev.y + BLOCK_SIZE;
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

enum collision_ray_type collisioRayCheck (pointFloat P, float rayAlpha, char map[MAP_HEIGHT][MAP_WIDTH], pointInt *C, int *dist)
{
    enum collision_ray_type ret = COLLISION_RAY_NONE;

    pointInt H, V;
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