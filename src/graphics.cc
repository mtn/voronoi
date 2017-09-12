#include "lib/graphics.h"

Graphics::Graphics() {
}

Graphics::Graphics(int width, int height) {
    this->width = width;
    this->height = height;
    this->gWindow = NULL;
    this->gRenderer = NULL;
}

bool Graphics::init() {
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        return false;
    } else {
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
            printf( "Warning: Linear texture filtering not enabled!" );
        }
        this->gWindow = SDL_CreateWindow( "Voronoi", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );


    }
    return true;
}

bool Graphics::close() {
    return false;
}
