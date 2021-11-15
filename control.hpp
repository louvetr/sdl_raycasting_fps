#ifndef CONTROL_H
#define CONTROL_H

#include "main.hpp"
#include "map.hpp"
#include "player.hpp"
#include <SDL2/SDL.h>

int ctrl_getQuit();
int ctrl_main(Player *player, mapObj *map);

#endif