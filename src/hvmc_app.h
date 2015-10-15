#ifndef HVMC_APP_H
#define HVMC_APP_H

#include "hvmc_math.h"
#include "hvmc_world.h"

struct SDL_Window;
struct SDL_Renderer;

struct HVMC_App
{
    bool Init();
    void SetupScene();
    void GameLoop();
    void Cleanup();

private:
    void ProcessEvents();
    void Update( f32 dt );
    void Render();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool running = false;

    World world;
};

#endif
