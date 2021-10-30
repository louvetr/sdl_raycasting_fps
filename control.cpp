
#include "control.hpp"
#include "main.hpp"
#include "player.hpp"

#define STEP_SIZE 3.f


static int quit = 0;

int ctrl_getQuit() {return quit;}

int ctrl_main(Player *player, char map[MAP_WIDTH][MAP_HEIGHT])
{
    SDL_Event e;
	int newX, newY;

	while (SDL_PollEvent(&e) != 0) {
		//user ask to quit
		if (e.type == SDL_QUIT ||
		    (e.type == SDL_KEYDOWN &&
		     e.key.keysym.sym == SDLK_ESCAPE)) {
			quit = 1;
            printf("EXIT game\n");
		}

		int oldBlockX, oldBlockY;
        if (e.type == SDL_KEYDOWN /*&& e.key.repeat == 0*/) {
    		switch (e.key.keysym.sym) {
	    	case SDLK_UP:

				oldBlockX = player->getX() / BLOCK_SIZE;
				oldBlockY = player->getY() / BLOCK_SIZE;
				newX = player->getX() + cosf(player->getAlpha()) * STEP_SIZE;
				newY = player->getY() - sinf(player->getAlpha()) * STEP_SIZE;
				if(map[newY/BLOCK_SIZE][newX/BLOCK_SIZE] == '.') {
	    	    	player->setX(newX);
	    	    	player->setY(newY);
				} 
				//printf("move from block [%d,%d] to [%d,%d]\n", oldBlockX, oldBlockY, newX/BLOCK_SIZE, newY/BLOCK_SIZE);
                /*printf("[%s:%d] player info : (%u, %u, %u)\n",
                   __func__, __LINE__, player->getX(), player->getY(), player->getAlpha());*/
	    		break;
	    	case SDLK_DOWN:
	    		//player->setY(player->getY() + 1);
	    	    newX = player->getX() - cosf(player->getAlpha()) * STEP_SIZE;
	    	    newY = player->getY() + sinf(player->getAlpha()) * STEP_SIZE;
				if(map[newY/BLOCK_SIZE][newX/BLOCK_SIZE] == '.') {
	    	    	player->setX(newX);
	    	    	player->setY(newY);
				}
               /*printf("[%s:%d] player info : (%u, %u, %u)\n",
                   __func__, __LINE__, player->getX(), player->getY(), player->getAlpha());*/
	    		break;
	    	case SDLK_LEFT:
	    	    player->setAlpha(player->getAlpha() + 0.05f);
				if (player->getAlpha() < 0.f)
					player->setAlpha(player->getAlpha() + PI2);
    	        else if (player->getAlpha() > PI2)
					player->setAlpha(player->getAlpha() - PI2);
	    		break;
	    	case SDLK_RIGHT:
	    	    player->setAlpha(player->getAlpha() - 0.05f);
				if (player->getAlpha() < 0.f)
					player->setAlpha(player->getAlpha() + PI2);
    	        else if (player->getAlpha() > PI2)
					player->setAlpha(player->getAlpha() - PI2);
	    		break;
	    	}
	    } 


		//mo_handle_event(&mo, e);
	}
}