#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int IMAGE_WIDTH = 220;
const int IMAGE_HEIGHT = 220;

class Game
{
    private:
        SDL_Window* gWindow = NULL;
        SDL_Renderer* gRenderer = NULL;

        struct Tile {
            SDL_Texture *clip;
            int x;
            int y;

            Tile(SDL_Texture *_clip, int _x, int _y) :
                clip(_clip), x(_x), y(_y) {};
        };

        std::vector<Tile *> tiles;

        int clipPerRow,
            clipPerColumn;

        void setTiles(std::string image, int rows, int cols);
        void puzzle();

    public:
        Game();
};

#endif
