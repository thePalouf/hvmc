#ifndef HVMC_UTILS_H
#define HVMC_UTILS_H

struct SDL_Texture;
struct SDL_Renderer;

extern SDL_Texture* CreateTextureFromImage
( SDL_Renderer* renderer, const char* filename );

#endif 
