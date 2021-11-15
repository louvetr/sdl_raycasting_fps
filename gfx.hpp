#ifndef GFX_H
#define GFX_H

#include "main.hpp"

enum textureId {
	silver_sandstone_block = 0,
	brick,
	stone_brick,
	wood,
	obsidian_block,
	grass,
	gravel,
	mossycobble,
	water,
	stone,
	steel_block
};

int gfxLoadTexture();
Uint8 *gfxGetTexture(enum textureId id);

#endif