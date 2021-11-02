#include "player.hpp"
#include "render.hpp"


int Xoffset = 0; // SCREEN_WIDTH / 2;

uint8_t wallColor = 0xA0;

int renderRay(SDL_Renderer *renderer, enum collision_ray_type colliType, int dist, int X)
{

    if (colliType == COLLISION_RAY_NONE)
        return -1;

    int heightProjected = BLOCK_SIZE * distPlayer / dist;

    int Ya = (SCREEN_HEIGHT - heightProjected) / 2;
    int Yb = (SCREEN_HEIGHT + heightProjected) / 2;

    //uint8_t colorOffset = - dist / 4;
    uint8_t colorOffset = 0;

    if (colliType != COLLISION_RAY_HORIZONTAL)
		  SDL_SetRenderDrawColor(renderer, 0xA0 + colorOffset, 0xA0 + colorOffset, 0xA0 + colorOffset, 0xff);
    else
		  SDL_SetRenderDrawColor(renderer, 0x80 + colorOffset, 0x80 + colorOffset, 0x80 + colorOffset, 0xff);


    SDL_RenderDrawLine(renderer, X + Xoffset, Ya, X + Xoffset, Yb);
}


int renderRayTextured(SDL_Renderer *renderer, enum collision_ray_type colliType, int dist, int X, pointFloat *C, Uint8 *pixelsWall)
{

    int tileFactor = 1;

    if (colliType == COLLISION_RAY_NONE)
        return -1;

    if(dist <= 0)
      return -1;

    int heightProjected = BLOCK_SIZE * distPlayer / dist / 2;

    int Ya = (SCREEN_HEIGHT - heightProjected) / 2;
    int Yb = (SCREEN_HEIGHT + heightProjected) / 2;

    uint8_t colorOffset = 0;
    float textureX = 0;

    if (colliType == COLLISION_RAY_HORIZONTAL) {
      int toRemove = (int)C->x / BLOCK_SIZE;
      textureX = floor(C->x) - toRemove;
    }
    else { 
      int toRemove = (int)C->y / BLOCK_SIZE;
      textureX = floor(C->y) - toRemove;
    }

    textureX *= 2;

    int drawX, drawY;

    int textureW = 128; //TODO: set properly
    int textureH = 128; //TODO: set properly

    float ratio = (float)heightProjected / (float)textureH;

    int i = Ya < 0 ? -Ya : 0;
    int toDrawY = Ya + i;

    //printf("textureX = %d; C(%f, %f)\n", (int)textureX, C->x, C->y);
    int texturePixelY = (int)((float) i / ratio);

    while(i< heightProjected && toDrawY < SCREEN_HEIGHT) {
      int texturePixelY = (int)((float) i / ratio);
      int pixelIdx = 4 * (texturePixelY * textureW + (int)textureX);

      pixelIdx = (pixelIdx * tileFactor) % (128 * 128 * 4); // TODO: store then insert here the texture size

      int r = pixelsWall[pixelIdx + 2];
      int g = pixelsWall[pixelIdx + 1];
      int b = pixelsWall[pixelIdx + 0];

      if (colliType == COLLISION_RAY_HORIZONTAL)
		    SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
      else
		    SDL_SetRenderDrawColor(renderer, r*3/4, g*3/4, b*3/4, 0xFF);
      
      SDL_RenderDrawPoint(renderer, X + Xoffset, toDrawY);
      toDrawY = Ya + i;
      i++;
    }

    return 0;
}


int renderMinimap(SDL_Renderer *renderer, Player *player, char map[MAP_HEIGHT][MAP_WIDTH], Uint8 map_scale)
{
    Uint8 alpha = 0x05;

    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = BLOCK_SIZE * MAP_WIDTH / map_scale;
    rect.h = BLOCK_SIZE * MAP_HEIGHT / map_scale;
		
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, alpha);
    SDL_RenderFillRect(renderer, &rect);

    rect.w = BLOCK_SIZE / map_scale;
    rect.h = BLOCK_SIZE / map_scale;

    // draw color => Blue
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, alpha);

    for(int i = 0; i < MAP_HEIGHT; i++) {
        for(int j = 0; j < MAP_WIDTH; j++) {
            if(map[i][j] > 0) {
                rect.x = BLOCK_SIZE * j / map_scale;
                rect.y = BLOCK_SIZE * i / map_scale;
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    // draw color => Gray
		SDL_SetRenderDrawColor(renderer, 0x90, 0x90, 0x90, 0xFF);


    int V1x = 0;
    int V1y = 0;
    int V2x = 0;
    int V2y = MAP_HEIGHT * BLOCK_SIZE / map_scale;
    for(int i = 0; i < MAP_WIDTH; i++) {
        V1x = BLOCK_SIZE * i / map_scale;
        V2x = BLOCK_SIZE * i / map_scale;
        SDL_RenderDrawLine(renderer, V1x, V1y, V2x, V2y);
    }

    int H1x = 0;
    int H1y = 0;
    int H2x = MAP_WIDTH * BLOCK_SIZE / map_scale;
    int H2y = 0;
    for(int i = 0; i < MAP_HEIGHT; i++) {
        H1y = BLOCK_SIZE * i / map_scale;
        H2y = BLOCK_SIZE * i / map_scale;
        SDL_RenderDrawLine(renderer, H1x, H1y, H2x, H2y);
    }

    // DRAW PLAYER ////////////////////////////////////////////////////////////////
    circleRGBA (renderer, player->getX() / map_scale, player->getY() / map_scale, 10, 0xff, 0, 0, 0xff);

		SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);

    SDL_RenderDrawLine( renderer,
                        player->getX() / map_scale,
                        player->getY() / map_scale,
                        (player->getX() + cosf(player->getAlpha())*10.f) / map_scale,
                        (player->getY() - sinf(player->getAlpha())*10.f) / map_scale  );

  return 0;
}


int renderMinimapRay(SDL_Renderer *renderer, Player *player, pointFloat *C, float rayAlpha, enum collision_ray_type colliType, Uint8 map_scale)
{

  SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0xff, 0xff);
  if (colliType != COLLISION_RAY_NONE)
      SDL_RenderDrawLine( renderer,
          player->getX() / map_scale,
          player->getY() / map_scale,
          C->x / map_scale,
          C->y / map_scale);
  else
      SDL_RenderDrawLine( renderer,
                      player->getX() / map_scale,
                      player->getY() / map_scale,
                      (player->getX() + cosf(rayAlpha)*1000.f) / map_scale,
                      (player->getY() - sinf(rayAlpha)*1000.f)  / map_scale );

  return 0;
}

