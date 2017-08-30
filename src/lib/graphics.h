// Based on lazyfoo tutorial

#pragma once

#include <SDL2/SDL.h>

const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH = 640;

class Graphics {
    public:
        Graphics();
        Graphics(int width, int height);
        ~Graphics();

        bool init();

        void free();
        bool close();

        int height;
        int width;

    private:
        SDL_Window* gWindow;
        SDL_Renderer* gRenderer;
};

