#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Game
{
    private:
        SDL_Window* gWindow = NULL;
        SDL_Renderer* gRenderer = NULL;
        std::vector<SDL_Texture *> tiles;

        void setTiles(std::string image, int rows, int cols);

    public:
        Game();
};

#endif
