

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>


#include "collision.hpp"
#include "control.hpp"
#include "main.hpp"
#include "player.hpp"

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

    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = BLOCK_SIZE;
    rect.h = BLOCK_SIZE;

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
                    rect.x = BLOCK_SIZE * j;
                    rect.y = BLOCK_SIZE * i;
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }

        // draw coloe => Gray
		SDL_SetRenderDrawColor(renderer, 0x90, 0x90, 0x90, 0xFF);

        int V1x = 0;
        int V1y = 0;
        int V2x = 0;
        int V2y = MAP_HEIGHT * BLOCK_SIZE;
        for(int i = 0; i < MAP_HEIGHT; i++) {
            V1x = BLOCK_SIZE * i;
            V2x = BLOCK_SIZE * i;
            SDL_RenderDrawLine(renderer, V1x, V1y, V2x, V2y);
        }

        int H1x = 0;
        int H1y = 0;
        int H2x = MAP_WIDTH * BLOCK_SIZE;
        int H2y = 0;
        for(int i = 0; i < MAP_WIDTH; i++) {
            H1y = BLOCK_SIZE * i;
            H2y = BLOCK_SIZE * i;
            SDL_RenderDrawLine(renderer, H1x, H1y, H2x, H2y);
        }
        
        // DRAW MAP (END) ////////////////////////////////////////////////////////////////




        // DRAW PLAYER ////////////////////////////////////////////////////////////////

        if( playerPrev.getX() != player.getX() ||
            playerPrev.getY() != player.getY() ||
            playerPrev.getAlpha() != player.getAlpha())
            printf("[%s:%d] player info : (%f, %f, %f)\n",
                    __func__, __LINE__, player.getX(), player.getY(), player.getAlpha());
        circleRGBA (renderer, player.getX(), player.getY(), 10, 0xff, 0, 0, 0xff);

		SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);

        SDL_RenderDrawLine( renderer,
                            player.getX(),
                            player.getY(),
                            player.getX() + cosf(player.getAlpha())*10.f,
                            player.getY() - sinf(player.getAlpha())*10.f  );

        // DRAW PLAYER (END) ////////////////////////////////////////////////////////////////



        // Horizontal collision ////////////////////////////////////////////////////////////////

		SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0xff, 0xff);

        float rayAlpha = player.getAlpha();
        pointInt A, Aprev;
        pointFloat P = {    .x = player.getX(),
                            .y = player.getY()};

        pointInt C;
        enum collision_ray_type colliType = collisioRayCheck (P, player.getAlpha(), map, &C);

        if (colliType != COLLISION_RAY_NONE)
            SDL_RenderDrawLine( renderer,
                player.getX(),
                player.getY(),
                C.x,
                C.y);
        else
            SDL_RenderDrawLine( renderer,
                            player.getX(),
                            player.getY(),
                            player.getX() + cosf(player.getAlpha())*1000.f,
                            player.getY() - sinf(player.getAlpha())*1000.f  );


		//update screen
		SDL_RenderPresent(renderer);

		SDL_Delay(1000 / 30);
    }

    return 0;
}

