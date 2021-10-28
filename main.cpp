

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




char map[MAP_WIDTH][MAP_HEIGHT] = {
    {'#','#','#','#','#','#','#','#','#','#'},
    {'#','.','.','#','#','.','.','.','.','#'},
    {'#','.','.','#','#','.','.','.','.','#'},
    {'#','.','.','#','#','.','.','.','.','#'},
    {'#','.','.','.','.','.','#','#','.','#'},
    {'#','.','.','.','.','.','#','#','.','#'},
    {'#','#','#','.','.','.','#','#','.','#'},
    {'#','#','#','.','.','.','#','#','.','#'},
    {'#','#','#','.','.','.','.','.','.','#'},
    {'#','#','#','#','#','#','#','#','#','#'}
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
    Player player(MAP_WIDTH * BLOCK_SIZE /2, MAP_HEIGHT*BLOCK_SIZE/2, 0);
    /*cpu_init();
    input_init();
    mem_init();
    time_init();*/


    uint8_t op_length, op_duration;
    uint32_t op_cpt = 0;
    // cpu loop


    uint32_t time_frame = 4;
    uint32_t time_cpu = 4;
    uint32_t opcode_nb = 0;

    int map_scale = 1;
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = BLOCK_SIZE / map_scale;
    rect.h = BLOCK_SIZE / map_scale;

    // Main loop
    while(!ctrl_getQuit()) {

        Player playerPrev = player;

        // controls management
        ctrl_main(&player);


		// clear screen
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);


        // DRAW MAP ////////////////////////////////////////////////////////////////

        // draw coloe => Blue
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);

        for(int i = 0; i < MAP_HEIGHT; i++) {
            for(int j = 0; j < MAP_WIDTH; j++) {
                if(map[i][j] == '#') {
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
        for(int i = 0; i < MAP_HEIGHT; i++) {
            V1x = BLOCK_SIZE * i / map_scale;
            V2x = BLOCK_SIZE * i / map_scale;
            SDL_RenderDrawLine(renderer, V1x, V1y, V2x, V2y);
        }

        int H1x = 0;
        int H1y = 0;
        int H2x = MAP_WIDTH * BLOCK_SIZE / map_scale;
        int H2y = 0;
        for(int i = 0; i < MAP_WIDTH; i++) {
            H1y = BLOCK_SIZE * i / map_scale;
            H2y = BLOCK_SIZE * i / map_scale;
            SDL_RenderDrawLine(renderer, H1x, H1y, H2x, H2y);
        }
        
        // DRAW MAP (END) ////////////////////////////////////////////////////////////////




        // DRAW PLAYER ////////////////////////////////////////////////////////////////

        if( playerPrev.getX() != player.getX() ||
            playerPrev.getY() != player.getY() ||
            playerPrev.getAlpha() != player.getAlpha())
            printf("[%s:%d] player info : (%f, %f, %f)\n",
                    __func__, __LINE__, player.getX(), player.getY(), player.getAlpha());
        circleRGBA (renderer, player.getX() / map_scale, player.getY() / map_scale, 10, 0xff, 0, 0, 0xff);

		SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);

        SDL_RenderDrawLine( renderer,
                            player.getX() / map_scale,
                            player.getY() / map_scale,
                            (player.getX() + cosf(player.getAlpha())*10.f) / map_scale,
                            (player.getY() - sinf(player.getAlpha())*10.f) / map_scale  );

        // DRAW PLAYER (END) ////////////////////////////////////////////////////////////////


        // DRAW SKY ////////////////////////////////////////////////////////////////
        SDL_Rect rectBg;
        SDL_SetRenderDrawColor(renderer, 0x87, 0xCE, 0xEB, 0xFF);
        rectBg.x = SCREEN_WIDTH / 2;
        rectBg.y = 0;
        rectBg.w = SCREEN_WIDTH / 2;
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
        pointInt A, Aprev;
        pointFloat P = {    .x = player.getX(),
                            .y = player.getY()};

        pointInt C;

        

        for (int i = 0; i < SCREEN_WIDTH / 2; i++) {

            float rayAlpha = player.getAlpha() - PI / 6.f + rayRadian * (float)i;
            if (rayAlpha < 0.f)
                rayAlpha += PI2;
            else if (rayAlpha > PI2)
                rayAlpha -= PI2;

            int dist = 0;
            enum collision_ray_type colliType = collisioRayCheck (P, rayAlpha, map, &C, &dist);
 
            if (colliType != COLLISION_RAY_NONE)
                SDL_RenderDrawLine( renderer,
                    player.getX() / map_scale,
                    player.getY() / map_scale,
                    C.x / map_scale,
                    C.y / map_scale);
            else
                SDL_RenderDrawLine( renderer,
                                player.getX() / map_scale,
                                player.getY() / map_scale,
                                (player.getX() + cosf(rayAlpha)*1000.f) / map_scale,
                                (player.getY() - sinf(rayAlpha)*1000.f)  / map_scale );


            renderRay(renderer, colliType, dist, i);

        }

		//update screen
		SDL_RenderPresent(renderer);

		SDL_Delay(1000 / 30);
    }

    return 0;
}

