

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>


#include "collision.hpp"
#include "control.hpp"
#include "main.hpp"
#include "player.hpp"
#include "render.hpp"

SDL_Window *window;
SDL_Texture *texture;
SDL_Surface *surface;
SDL_Renderer *renderer;

int scale = 1;




char map[MAP_HEIGHT][MAP_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,3,3,0,0,0,2,2,0,0,0,0,0,1},
    {1,0,0,3,3,0,0,0,2,2,2,2,2,0,0,1},
    {1,0,0,0,0,0,0,0,2,0,0,0,0,0,0,1},
    {1,1,0,0,0,0,0,0,2,0,0,0,0,0,0,1},
    {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,4,4,4,0,0,0,0,0,5,5,0,0,1},
    {1,0,0,0,0,4,0,0,0,0,5,5,5,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    };


int SDL_init()
{
	int ret;

	// init SDL
	ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0) {
		printf("SDL_Init ERROR: %s\n", SDL_GetError());
		return ret;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		printf("Warning: Linear texture filtering not enabled!");
	}

	// create window
	window = SDL_CreateWindow("BalaBoy", SDL_WINDOWPOS_UNDEFINED,
				  SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * scale,
				  SCREEN_HEIGHT * scale, SDL_WINDOW_SHOWN);
	if (!window) {
		printf("SDL_SetVideoMode ERROR: %s\n", SDL_GetError());
		return -1;
	}

	//SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xFF, 0x00);

	// get window surface
	surface = SDL_GetWindowSurface(window);
	if (!surface) {
		printf("SDL_GetWindowSurface ERROR: %s\n", SDL_GetError());
		return -1;
	}


	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		printf("Renderer could not be created! SDL Error: %s\n",
		       SDL_GetError());
		return -1;
	}

	/*int img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(img_flags) & img_flags)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n",
		       IMG_GetError());
		return -1;
	}*/

	return 0;
}

SDL_Texture *textureWall = NULL;
Uint8 *pixelsWalls[10];

/*
Copies the pixels from a SDL2 surface.
You should free() the returned pixels when you're done with it.
*/
Uint8* copySurfacePixels(
  SDL_Surface* surface,  // surface to take pixels from
  Uint32 pixelFormat,    // usually SDL_GetWindowPixelFormat(window)
  SDL_Renderer* renderer,// main SDL2 renderer
  int* width,            // stores result width
  int* height,           // stores result height
  int* pitch)            // stores result pitch
{
    Uint8* pixels = 0;
    SDL_Surface* tmpSurface = 0;
    SDL_Texture* texture = 0;
    int sizeInBytes = 0;
    void* tmpPixels = 0;
    int tmpPitch = 0;

    tmpSurface = SDL_ConvertSurfaceFormat(surface, pixelFormat, 0);
    if (tmpSurface) {
        texture = SDL_CreateTexture( renderer, pixelFormat,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    tmpSurface->w, tmpSurface->h );
        if(!texture)
            printf("[%s:%d] SDL_CreateTexture FAILED\n", __func__, __LINE__);
    }
    else
        printf("[%s:%d] SDL_ConvertSurfaceFormat FAILED\n", __func__, __LINE__);


    if (texture) {
        if (width)
            *width = tmpSurface->w;
        
        if (height)
            *height = tmpSurface->h;
    
        if (pitch)
        *pitch = tmpSurface->pitch;
        
        sizeInBytes = tmpSurface->pitch * tmpSurface->h;
        printf("[%s:%d] sizeInBytes = %d\n", __func__, __LINE__, sizeInBytes);
        pixels = (Uint8*)malloc( sizeInBytes );
        SDL_LockTexture( texture, 0, &tmpPixels, &tmpPitch );
        memcpy( pixels, tmpSurface->pixels, sizeInBytes);
        SDL_UnlockTexture( texture );
    }

    // Cleanup
    if (texture) 
        SDL_DestroyTexture(texture);

    if (tmpSurface)
        SDL_FreeSurface(tmpSurface);
  
    return pixels;
}

Uint8* getPixelsFromTextureFile (const char *path)
{
	SDL_Surface *surface = IMG_Load(path);
	if (!surface) {
		SDL_Log("[%s] Unable to load image %s! SDL_image Error: %s\n",
			__func__,
			path,
			IMG_GetError());
		return NULL;
	}
    Uint32 pf = SDL_GetWindowPixelFormat(window);
    printf("pixelFormat = %d\n", pf);
    int w=0, h=0, p=0;
    Uint8* pixels = copySurfacePixels(surface, pf, renderer, &w, &h, &p);

    return pixels;
}

void leave()
{
	/*SDL_DestroyTexture(texture);
	texture = NULL;*/

	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	//destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	//quit SDL
	//IMG_Quit();
	SDL_Quit();
}

int main(int argc, char** argv)
{
    int ret;

    /*if(argc < 2 || argc > 3) {
        printf("Invalid command usage, shall be:\n");
        printf("        ./balaboy <rom_path> <screen_scale>\n");
        printf("Example:\n");
        printf("        ./balaboy tetris.gb 3\n");
        //goto exit;
        return -1;
    }*/

    /*printf("argc = %d\n", argc);
    printf("argv[0] = %s\n", argv[0]);
    printf("argv[1] = %s\n", argv[1]);*/

    // inits
    SDL_init();
    Player player((MAP_WIDTH - 2) * BLOCK_SIZE /2, MAP_HEIGHT*BLOCK_SIZE/2, 0);
    /*cpu_init();
    input_init();
    mem_init();
    time_init();*/


    // Load texture
    //load_texture_from_file(textureWall, "./texture/default_brick.png");
    pixelsWalls[0] = getPixelsFromTextureFile ("./texture/default_silver_sandstone_block.png");
    if(!pixelsWalls[0]) {
        printf("getPixelsFromTextureFile ERROR\n");
        return -1;
    }
    pixelsWalls[1] = getPixelsFromTextureFile ("./texture/default_brick.png");
    if(!pixelsWalls[1]) {
        printf("getPixelsFromTextureFile ERROR\n");
        return -1;
    }
    pixelsWalls[2] = getPixelsFromTextureFile ("./texture/default_stone_brick.png");
    if(!pixelsWalls[2]) {
        printf("getPixelsFromTextureFile ERROR\n");
        return -1;
    }

    pixelsWalls[3] = getPixelsFromTextureFile ("./texture/default_wood.png");
    if(!pixelsWalls[3]) {
        printf("getPixelsFromTextureFile ERROR\n");
        return -1;
    }

    pixelsWalls[4] = getPixelsFromTextureFile ("./texture/default_obsidian_block.png");
    if(!pixelsWalls[4]) {
        printf("getPixelsFromTextureFile ERROR\n");
        return -1;
    }

    uint8_t op_length, op_duration;
    uint32_t op_cpt = 0;
    // cpu loop


    uint32_t time_frame = 4;
    uint32_t time_cpu = 4;
    uint32_t opcode_nb = 0;

    int minimap_scale = 4;

    // Main loop
    while(!ctrl_getQuit()) {

        Player playerPrev = player;

        // controls management
        ctrl_main(&player, map);

		// clear screen
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

        // DRAW SKY ////////////////////////////////////////////////////////////////
        SDL_Rect rectBg;
        SDL_SetRenderDrawColor(renderer, 0x87, 0xCE, 0xEB, 0xFF);
        rectBg.x = 0 /*SCREEN_WIDTH / 2*/;
        rectBg.y = 0;
        rectBg.w = SCREEN_WIDTH /*/ 2*/;
        rectBg.h = SCREEN_HEIGHT / 2;
        SDL_RenderFillRect(renderer, &rectBg);

        // DRAW GRASS GRADIANT ////////////////////////////////////////////////////////////
        int nbSubSteps = 50;
        int gradiantStepH = SCREEN_HEIGHT / 2 / nbSubSteps;
        int gradiantOffset = 2;

        Uint8 red = 0x66 - nbSubSteps * gradiantOffset ;
        Uint8 green = 0x8D - nbSubSteps * gradiantOffset ;
        Uint8 blue = 0x56 - nbSubSteps * gradiantOffset ;

        rectBg.h = gradiantStepH;
        rectBg.y = SCREEN_HEIGHT / 2;

        for (int i = 0; i < nbSubSteps; i++) {
            SDL_SetRenderDrawColor(renderer, red, green, blue, 0xFF);
            SDL_RenderFillRect(renderer, &rectBg);
            rectBg.y += gradiantStepH;
            red += gradiantOffset;
            green += gradiantOffset;
            blue += gradiantOffset;
        }

        // Ray collision ////////////////////////////////////////////////////////////////

		SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0xff, 0xff);

        float rayAlpha = player.getAlpha();
        pointFloat P = {    .x = player.getX(),
                            .y = player.getY()};
        pointFloat C;

        for (int i = 0; i < SCREEN_WIDTH /*/ 2*/; i++) {

            float subAlpha = PI / 6.f - rayRadian * (float)i;
            float rayAlpha = player.getAlpha() + subAlpha;
            if (rayAlpha < 0.f)
                rayAlpha += PI2;
            else if (rayAlpha > PI2)
                rayAlpha -= PI2;

            int dist = 0;
            enum collision_ray_type colliType = collisioRayCheck (P, rayAlpha, map, &C, &dist);
            
            // remove fish eye effect
            dist *= cosf(subAlpha);

            //renderRay(renderer, colliType, dist, i);
            renderRayTextured(renderer, colliType, dist, i, &C, pixelsWalls[map[(int)C.y/BLOCK_SIZE][(int)C.x/BLOCK_SIZE]-1]);

            // TODO:  useless ? hidden by minimap
            //renderMinimapRay(renderer, &player, &C, subAlpha, colliType, minimap_scale);
            //printf("[i=%d] subAlpha = %f\n", i - SCREEN_WIDTH / 2, subAlpha);

        }

        renderMinimap(renderer, &player, map, minimap_scale);

		//update screen
		SDL_RenderPresent(renderer);

        // TODO: smooth timing with delta % frame compute time
		SDL_Delay(1000 / 30);
    }

    return 0;
}

