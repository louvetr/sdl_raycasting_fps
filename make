#!/bin/bash
g++ main.cpp control.cpp player.cpp -I/usr/include/SDL2/ -lSDL2 -lSDL2_gfx -g -gdwarf
