#!/bin/bash
g++ main.cpp control.cpp player.cpp collision.cpp render.cpp map.cpp -I/usr/include/SDL2/ -lSDL2 -lSDL2_gfx -lSDL2_image -g -gdwarf
#g++ main.cpp control.cpp player.cpp collision.cpp render.cpp map.cpp -I/usr/include/SDL2/ -lSDL2 -lSDL2_gfx -lSDL2_image -g -gdwarf -Wall -fsanitize=address -O1 -fno-omit-frame-pointer -ggdb -gdwarf-2