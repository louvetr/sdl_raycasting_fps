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


int renderRayTextured(SDL_Renderer *renderer, enum collision_ray_type colliType, int dist, int X, pointFloat *C, Uint8 *pixelsWall, pointFloat playerPoint, float rayAlpha, float subAlpha, Uint8 *pixelsFloor)
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

    if (colliType == COLLISION_RAY_HORIZONTAL)
      textureX = fmodf(C->x, (float)BLOCK_SIZE) * 2;
    else
      textureX = fmodf(C->y, (float)BLOCK_SIZE) * 2;

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


    // Draw Floor
    /*if(X < SCREEN_WIDTH / 2 - 5 || X > SCREEN_WIDTH / 2 + 5)
      return 0;*/

    
		//var targetIndex=lastBottomOfWall*(this.offscreenCanvasPixels.width*bytesPerPixel)+(bytesPerPixel*castColumn);
    for(int row = toDrawY /*+ 1*/; row < SCREEN_HEIGHT; row++) {

			float ratioFloor = 32.f / (float)(row - SCREEN_HEIGHT / 2);
			float diagonalDistance = distPlayer * ratioFloor / cosf(subAlpha) / 2; // !!!!! / 2 is weird

      // floor point coordinates
			float xEnd = (playerPoint.x + floorf(diagonalDistance * cosf(rayAlpha)));
			float yEnd = (playerPoint.y - floorf(diagonalDistance * sinf(rayAlpha)));
		
			// get matching map block
			int cellX = floorf(xEnd / BLOCK_SIZE);
			int cellY = floorf(yEnd / BLOCK_SIZE);

        static int cpt = 1;
        if(cpt % 10 == 0 && X == SCREEN_WIDTH / 2 && row == toDrawY + 1) {
          printf("[FLOOR] ratioFloor=%f, distPlayer=%d, diagonalDistance=%f\n", ratioFloor, distPlayer, diagonalDistance);
          printf("[FLOOR] playerPoint=(%f, %f), endXY=(%f, %f), CellXY=(%d, %d)\n",
                  playerPoint.x, playerPoint.y, xEnd, yEnd, cellX, cellY);
        }
			//Make sure the tile is within our map
			if ((cellX<MAP_WIDTH) && (cellY<MAP_HEIGHT) && cellX>=0 && cellY>=0)
			{            
				// Find pixel X,Y in texture
        int tileX = (int)floorf(fmodf(xEnd, (float)BLOCK_SIZE * 2));
        int tileY = (int)floorf(fmodf(yEnd, (float)BLOCK_SIZE * 2));
        int pixelIdx = 4 * (tileX * textureW + (int)tileY);
        int tileFactorFloor = 1;

        pixelIdx = (pixelIdx * tileFactorFloor)  % (128 * 128 * 4); // TODO: store then insert here the texture size

        int r = pixelsFloor[pixelIdx + 2];
        int g = pixelsFloor[pixelIdx + 1];
        int b = pixelsFloor[pixelIdx + 0];

        if(cpt % 10 == 0 && X == SCREEN_WIDTH / 2 && toDrawY + 1 && row == toDrawY + 1)
          printf("floor pixel: (%d, %d) => %d\n", tileX, tileY, pixelIdx);

		    SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
        SDL_RenderDrawPoint(renderer, X + Xoffset, row );


			}                                                              
          cpt++;

    }


    return 0;
}


int renderMinimap(SDL_Renderer *renderer, Player *player, char map[MAP_HEIGHT][MAP_WIDTH], Uint8 map_scale)
{
    Uint8 alpha = 0xA0;

    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = BLOCK_SIZE * MAP_WIDTH / map_scale;
    rect.h = BLOCK_SIZE * MAP_HEIGHT / map_scale;
		
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

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
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    circleRGBA (renderer, player->getX() / map_scale, player->getY() / map_scale, BLOCK_SIZE / map_scale / 2, 0, 0, 0, 0xff);

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);

    SDL_RenderDrawLine( renderer,
                        player->getX() / map_scale,
                        player->getY() / map_scale,
                        (player->getX() + cosf(player->getAlpha())*30.f) / map_scale,
                        (player->getY() - sinf(player->getAlpha())*30.f) / map_scale  );

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

