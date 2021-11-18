#include "map.hpp"
#include "gfx.hpp"


/////////////////////////////////////////////////////////////////////////////////////////
mapBlock::mapBlock()
{
	type = MBTYPE_UNKNOWN;
}

mapBlock::mapBlock(mapBlockType t)
{
	type = t;
}

mapBlock::~mapBlock() {}

void mapBlock::setType(mapBlockType t)
{
	type = t;
}

void mapBlock::setTextureFloor(Uint8 *tFloor, int tFloorWidth, int tFloorHeight, int tFloorTileFactor)
{
	type = MBTYPE_FLOOR;
	textureFloor = tFloor;
	textureFloorWidth = tFloorWidth;
	textureFloorHeight = tFloorHeight;
	textureFloorTileFactor = tFloorTileFactor;
}


void mapBlock::setTextureWall(Uint8 *tWall, int tWallWidth, int tWallHeight, int tWallTileFactor)
{
	type = MBTYPE_WALL;
	textureWall = tWall;
	textureWallWidth = tWallWidth;
	textureWallHeight = tWallHeight;
	textureWallTileFactor = tWallTileFactor;

	textureFloor = tWall;
	textureFloorWidth = tWallWidth;
	textureFloorHeight = tWallHeight;
	textureFloorTileFactor = tWallTileFactor;
}

/////////////////////////////////////////////////////////////////////////////////////////
mapBlockFloor::mapBlockFloor(Uint8 *tFloor, int tFloorWidth, int tFloorHeight, int tFloorTileFactor)
{
	type = MBTYPE_FLOOR;
	textureFloor = tFloor;
	textureFloorWidth = tFloorWidth;
	textureFloorHeight = tFloorHeight;
	textureFloorTileFactor = tFloorTileFactor;
}

mapBlockFloor::~mapBlockFloor() {}

/////////////////////////////////////////////////////////////////////////////////////////
mapBlockWall::mapBlockWall(Uint8 *tWall, int tWallWidth, int tWallHeight, int tWallTileFactor)
{
	type = MBTYPE_WALL;
	textureWall = tWall;
	textureWallWidth = tWallWidth;
	textureWallHeight = tWallHeight;
	textureWallTileFactor = tWallTileFactor;
}

mapBlockWall::~mapBlockWall()
{
	SDL_Log("[DEBUG][%s:%d] free \n", __func__, __LINE__);
	/* TODO: free */
}


/////////////////////////////////////////////////////////////////////////////////////////
mapFloor::mapFloor(int w, int h)
{
	blocks = (mapBlock **)calloc(w * h * sizeof(mapBlock *), 1);
	if (!blocks) {
		SDL_Log("[ERROR][%s:%d] map calloc failed\n", __func__, __LINE__);
	}

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			blocks[i * h + j] = (mapBlock *)calloc(sizeof(mapBlock), 1);
		}
	}
}

mapFloor::~mapFloor()
{
	SDL_Log("[DEBUG][%s:%d] free \n", __func__, __LINE__);
	free(blocks);
}

/////////////////////////////////////////////////////////////////////////////////////////
mapObj::mapObj(int w, int h, int z)
{
	mapWidth = w;
	mapHeight = h;
	nbFloors = z;
	floors = (mapFloor *)calloc(z * sizeof(mapFloor), 1);
	if (!floors) {
		SDL_Log("[ERROR][%s:%d] map calloc failed\n", __func__, __LINE__);
	}
};

mapObj::~mapObj()
{
	free(floors);
}

void mapObj::build()
{
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			int idx = i * MAP_HEIGHT + j;

			switch (rawMap[i][j]) {
			// Floors
			case 0x00:
				floors->blocks[idx]->setTextureFloor(
					gfxGetTexture(grass), TEXTURE_SIZE, TEXTURE_SIZE, 2);
				break;
			case 0x01:
				floors->blocks[idx]->setTextureFloor(
					gfxGetTexture(water), TEXTURE_SIZE, TEXTURE_SIZE, 2);
				break;
			case 0x02:
				floors->blocks[idx]->setTextureFloor(
					gfxGetTexture(gravel), TEXTURE_SIZE, TEXTURE_SIZE, 1);
				break;
			case 0x03:
				floors->blocks[idx]->setTextureFloor(
					gfxGetTexture(wood), TEXTURE_SIZE, TEXTURE_SIZE, 1);
				break;
			case 0x04:
				floors->blocks[idx]->setTextureFloor(
					gfxGetTexture(mossycobble), TEXTURE_SIZE, TEXTURE_SIZE, 2);
				break;

			// Walls
			case 0x10:
				floors->blocks[idx]->setTextureWall(
					gfxGetTexture(silver_sandstone_block), TEXTURE_SIZE, TEXTURE_SIZE, 1);
				break;
			case 0x20:
				floors->blocks[idx]->setTextureWall(
					gfxGetTexture(brick), TEXTURE_SIZE, TEXTURE_SIZE, 1);
				break;
			case 0x30:
				floors->blocks[idx]->setTextureWall(
					gfxGetTexture(stone_brick), TEXTURE_SIZE, TEXTURE_SIZE, 1);
				break;
			case 0x40:
				floors->blocks[idx]->setTextureWall(gfxGetTexture(wood), TEXTURE_SIZE, TEXTURE_SIZE, 1);
				break;
			case 0x50:
				floors->blocks[idx]->setTextureWall(
					gfxGetTexture(obsidian_block), TEXTURE_SIZE, TEXTURE_SIZE, 1);
				break;
			case 0x60:
				floors->blocks[idx]->setTextureWall(
					gfxGetTexture(stone), TEXTURE_SIZE, TEXTURE_SIZE, 1);
				break;
			case 0x70:
				floors->blocks[idx]->setTextureWall(
					gfxGetTexture(steel_block), TEXTURE_SIZE, TEXTURE_SIZE, 1);
				break;

			case 0x21:	// diagonal /
				floors->blocks[idx]->setTextureWall(
					gfxGetTexture(wood), TEXTURE_SIZE, TEXTURE_SIZE, 1);
				floors->blocks[idx]->setTextureFloor(
					gfxGetTexture(grass), TEXTURE_SIZE, TEXTURE_SIZE, 2);
				floors->blocks[idx]->setType(MBTYPE_WALL_DIAG_BL2TR);
				break;

			default:
				SDL_Log("ERROR when setting map block type");
			}
		}
	}
}