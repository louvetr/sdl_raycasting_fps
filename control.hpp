#ifndef CONTROL_H
#define CONTROL_H

#include <SDL2/SDL.h>
#include "main.hpp"
#include "map.hpp"
#include "player.hpp"

int ctrl_getQuit();
int ctrl_main(Player *player, mapObj *map);

#endif