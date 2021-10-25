
#include "control.hpp"
#include "player.hpp"

#define STEP_SIZE 3.f


static int quit = 0;

int ctrl_getQuit() {return quit;}

int ctrl_main(Player *player)
{
    SDL_Event e;

	while (SDL_PollEvent(&e) != 0) {
		//user ask to quit
		if (e.type == SDL_QUIT ||
		    (e.type == SDL_KEYDOWN &&
		     e.key.keysym.sym == SDLK_ESCAPE)) {
			quit = 1;
            printf("EXIT game\n");
		}


        if (e.type == SDL_KEYDOWN /*&& e.key.repeat == 0*/) {
    		switch (e.key.keysym.sym) {
	    	case SDLK_UP:
	    	    player->setX(player->getX() + cosf(player->getAlpha()) * STEP_SIZE);
	    	    player->setY(player->getY() - sinf(player->getAlpha()) * STEP_SIZE);
                /*printf("[%s:%d] player info : (%u, %u, %u)\n",
                   __func__, __LINE__, player->getX(), player->getY(), player->getAlpha());*/
	    		break;
	    	case SDLK_DOWN:
	    		//player->setY(player->getY() + 1);
	    	    player->setX(player->getX() - cosf(player->getAlpha()) * STEP_SIZE);
	    	    player->setY(player->getY() + sinf(player->getAlpha()) * STEP_SIZE);
               /*printf("[%s:%d] player info : (%u, %u, %u)\n",
                   __func__, __LINE__, player->getX(), player->getY(), player->getAlpha());*/
	    		break;
	    	case SDLK_LEFT:
	    	    player->setAlpha(player->getAlpha() + 0.05f);
	    		break;
	    	case SDLK_RIGHT:
	    	    player->setAlpha(player->getAlpha() - 0.05f);
	    		break;
	    	}
	    } 


		//mo_handle_event(&mo, e);
	}
}