#include "hvmc_utils.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "hvmc_math.h"

SDL_Texture* CreateTextureFromImage( SDL_Renderer* renderer, const char* filename )
{
    int w, h, n;
    u8* data = stbi_load( filename, &w, &h, &n, 0 );

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    unsigned int rmask = 0xff000000;
    unsigned int gmask = 0x00ff0000;
    unsigned int bmask = 0x0000ff00;
    unsigned int amask = 0x000000ff;
#else
    unsigned int rmask = 0x000000ff;
    unsigned int gmask = 0x0000ff00;
    unsigned int bmask = 0x00ff0000;
    unsigned int amask = 0xff000000;
#endif

    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom( data, w, h, 32, n * w,
                                                     rmask, gmask, bmask, amask );
    
    if ( nullptr == surface )
    {
        fprintf( stderr, "SDL_CreateRGBSurfaceFrom Error : %s\n", SDL_GetError() );
        stbi_image_free( data );
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, surface );
    SDL_FreeSurface( surface );
    stbi_image_free( data );

    if ( nullptr == texture )
    {
        fprintf( stderr, "SDL_CreateTextureFromSurface Error : %s\n", SDL_GetError() );
        return nullptr;
    }

    return texture;
}

