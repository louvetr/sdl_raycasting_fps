#!/bin/bash
g++ main.cpp control.cpp player.cpp collision.cpp render.cpp -I/usr/include/SDL2/ -lSDL2 -lSDL2_gfx -lSDL2_image -g -gdwarf
