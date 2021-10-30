#include "render.hpp"


int Xoffset = SCREEN_WIDTH / 2;

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


int renderRayTextured(SDL_Renderer *renderer, enum collision_ray_type colliType, int dist, int X, pointInt *C, Uint8 *pixelsWall)
{

    int tileFactor = 2;

    if (colliType == COLLISION_RAY_NONE)
        return -1;

    //int heightProjected = SCREEN_HEIGHT;
    if(dist <= 0)
      return -1;

    int heightProjected = BLOCK_SIZE * distPlayer / dist;
    /*if (heightProjected > SCREEN_HEIGHT)
      heightProjected = SCREEN_HEIGHT;*/

    int Ya = (SCREEN_HEIGHT - heightProjected) / 2;
    int Yb = (SCREEN_HEIGHT + heightProjected) / 2;

    //uint8_t colorOffset = - dist / 4;
    uint8_t colorOffset = 0;
    int textureX = 0;

    if (colliType == COLLISION_RAY_HORIZONTAL)
      textureX = C->x % (BLOCK_SIZE * 2);
    else
      textureX = C->y % (BLOCK_SIZE * 2);

    //SDL_RenderDrawLine(renderer, X + Xoffset, Ya, X + Xoffset, Yb);

    int drawX, drawY;

    int textureW = 128; //TODO: set properly
    int textureH = 128; //TODO: set properly

    float ratio = (float)heightProjected / (float)textureH;

    int i = 0;
    int toDrawY = Ya + i;
    if (Ya < 0)
      toDrawY = 0;

    while(i< heightProjected && toDrawY < SCREEN_HEIGHT) {
      int texturePixelY = (int)((float) i / ratio);
      int pixelIdx = 4 * (texturePixelY * textureW + textureX);

      pixelIdx = (pixelIdx * tileFactor) % (128 * 128 * 4); // store then insert here the texture size

      int r = pixelsWall[pixelIdx + 2];
      int g = pixelsWall[pixelIdx + 1];
      int b = pixelsWall[pixelIdx + 0];

		  SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
      SDL_RenderDrawPoint(renderer, X + Xoffset, toDrawY);
      toDrawY = Ya + i;
      i++;
    }

      int texturePixelY0 = (int)((float)(Ya) / ratio) / 4;
      int texturePixelYlast = (int)((float)(heightProjected-1) / ratio) / 4;

    //printf("draw texture: x=%d, y=[%d, %d]\n", textureX, texturePixelY0, texturePixelYlast);
}
