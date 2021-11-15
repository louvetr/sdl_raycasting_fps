
#include "main.hpp"
#include "gfx.hpp"

static Uint8 *texturesTab[20];

// STATIC functions ////////////////////////////////////////////////////////////////////////////////////////
/*
Copies the pixels from a SDL2 surface.
You should free() the returned pixels when you're done with it.
*/
Uint8* copySurfacePixels(
  SDL_Surface* surface,  // surface to take pixels from
  Uint32 pixelFormat,    // usually SDL_GetWindowPixelFormat(window)
  SDL_Renderer* renderer,// main SDL2 renderer
  int* width,            // stores result width
  int* height,           // stores result height
  int* pitch)            // stores result pitch
{
    Uint8* pixels = 0;
    SDL_Surface* tmpSurface = 0;
    SDL_Texture* texture = 0;
    int sizeInBytes = 0;
    void* tmpPixels = 0;
    int tmpPitch = 0;

    tmpSurface = SDL_ConvertSurfaceFormat(surface, pixelFormat, 0);
    if (tmpSurface) {
        texture = SDL_CreateTexture( renderer, pixelFormat,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    tmpSurface->w, tmpSurface->h );
        if(!texture)
            printf("[%s:%d] SDL_CreateTexture FAILED\n", __func__, __LINE__);
    }
    else
        printf("[%s:%d] SDL_ConvertSurfaceFormat FAILED\n", __func__, __LINE__);


    if (texture) {
        if (width)
            *width = tmpSurface->w;
        
        if (height)
            *height = tmpSurface->h;
    
        if (pitch)
        *pitch = tmpSurface->pitch;
        
        sizeInBytes = tmpSurface->pitch * tmpSurface->h;
        printf("[%s:%d] sizeInBytes = %d\n", __func__, __LINE__, sizeInBytes);
        pixels = (Uint8*)malloc( sizeInBytes );
        SDL_LockTexture( texture, 0, &tmpPixels, &tmpPitch );
        memcpy( pixels, tmpSurface->pixels, sizeInBytes);
        SDL_UnlockTexture( texture );
    }

    // Cleanup
    if (texture) 
        SDL_DestroyTexture(texture);

    if (tmpSurface)
        SDL_FreeSurface(tmpSurface);
  
    return pixels;
}

Uint8* getPixelsFromTextureFile (const char *path)
{
	SDL_Surface *surface = IMG_Load(path);
	if (!surface) {
		SDL_Log("[%s] Unable to load image %s! SDL_image Error: %s\n",
			__func__,
			path,
			IMG_GetError());
		return NULL;
    }

    Uint32 pf = SDL_GetWindowPixelFormat(getWindow());
    printf("pixelFormat = %d\n", pf);
    int w=0, h=0, p=0;
    Uint8* pixels = copySurfacePixels(surface, pf, getRenderer(), &w, &h, &p);
	if (!pixels)
		SDL_Log("[%s] Unable to create texture from image %s! SDL_image Error: %s\n",
			__func__,
			path,
			IMG_GetError());
        
	SDL_Log("[%s] return texture ptr = 0x%p\n", __func__, pixels);
    return pixels;
}

// PUBLIC functions ////////////////////////////////////////////////////////////////////////////////////////
int gfxLoadTexture()
{
    // Load texture
    texturesTab[silver_sandstone_block] = getPixelsFromTextureFile ("./texture/default_silver_sandstone_block.png");
    SDL_Log("[%s:%d] just set texture #%d = 0x%p", __func__, __LINE__,  silver_sandstone_block, texturesTab[silver_sandstone_block]);
    texturesTab[brick] = getPixelsFromTextureFile ("./texture/default_brick.png");
    texturesTab[stone_brick] = getPixelsFromTextureFile ("./texture/default_stone_brick.png");
    texturesTab[wood] = getPixelsFromTextureFile ("./texture/default_wood.png");
    texturesTab[obsidian_block] = getPixelsFromTextureFile ("./texture/default_obsidian_block.png");
    texturesTab[grass] = getPixelsFromTextureFile ("./texture/default_grass.png");
    texturesTab[gravel] = getPixelsFromTextureFile ("./texture/default_gravel.png");
    texturesTab[mossycobble] = getPixelsFromTextureFile ("./texture/default_mossycobble.png");
    texturesTab[water] = getPixelsFromTextureFile ("./texture/default_water.png");
    texturesTab[water] = getPixelsFromTextureFile ("./texture/default_water.png");
    texturesTab[stone] = getPixelsFromTextureFile ("./texture/default_stone.png");
    texturesTab[steel_block] = getPixelsFromTextureFile ("./texture/default_steel_block.png");

    return 0;
}

Uint8* gfxGetTexture(enum textureId id)
{
    if(id == 0)
        SDL_Log("[%s:%d] return texture #%d = 0x%p", __func__, __LINE__,  id, texturesTab[id]);
    return texturesTab[id];
}