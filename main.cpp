#include "collision.hpp"
#include "control.hpp"
#include "gfx.hpp"
#include "main.hpp"
#include "map.hpp"
#include "player.hpp"
#include "render.hpp"


SDL_Window *window;
SDL_Texture *texture;
SDL_Surface *surface;
SDL_Renderer *renderer;

int scale = 1;


SDL_Window* getWindow()
{
    return window;
}

SDL_Renderer* getRenderer()
{
    return renderer;
}


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
	window = SDL_CreateWindow("BalaFps", SDL_WINDOWPOS_UNDEFINED,
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


    gfxLoadTexture();

    uint8_t op_length, op_duration;
    uint32_t op_cpt = 0;
    // cpu loop


    uint32_t time_frame = 4;
    uint32_t time_cpu = 4;
    uint32_t opcode_nb = 0;

    int minimap_scale = 4;

    mapObj map = mapObj(MAP_WIDTH, MAP_HEIGHT, 1);
    mapFloor floor = mapFloor(MAP_WIDTH, MAP_HEIGHT);
    *map.floors = floor;
    map.build(); 



    // Main loop
    while(!ctrl_getQuit()) {

        Player playerPrev = player;

        // controls management
        ctrl_main(&player, &map);

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
            enum collision_ray_type colliType = collisioRayCheck (P, rayAlpha, &map, &C, &dist);
            
            // remove fish eye effect
            dist *= cosf(subAlpha);

            //map[(int)C.y/BLOCK_SIZE][(int)C.x/BLOCK_SIZE]-1
            // C.y/BLOCK_SIZE * map.mapHeight + C.x/BLOCK_SIZE

            //renderRay(renderer, colliType, dist, i);
            renderRayTextured(renderer, colliType, dist, i, &C, &map,
                                P, rayAlpha, subAlpha);

            // TODO:  useless ? hidden by minimap
            renderMinimapRay(renderer, &player, &C, subAlpha, colliType, minimap_scale);
            //printf("[i=%d] subAlpha = %f\n", i - SCREEN_WIDTH / 2, subAlpha);

        }

        renderMinimap(renderer, &player, &map, minimap_scale);

		//update screen
		SDL_RenderPresent(renderer);

        // TODO: smooth timing with delta % frame compute time
		SDL_Delay(1000 / 30);
    }

    return 0;
}

