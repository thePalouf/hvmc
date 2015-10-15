#include <SDL2/SDL.h>
#include <stdio.h>

#include "hvmc_app.h"

int main( int argc, char** argv )
{
    HVMC_App app;
    
    if ( app.Init() )
    {
        app.SetupScene();
        app.GameLoop();
    }

    app.Cleanup();
    
    return 0;
}

