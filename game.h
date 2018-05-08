#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int IMAGE_WIDTH = 440;
const int IMAGE_HEIGHT = 440;
const int TICK_LIMIT = 1000 / 60;

class Game
{
    private:
        SDL_Window* gWindow = NULL;
        SDL_Renderer* gRenderer = NULL;

        struct Tile {
            SDL_Texture *clip;
            SDL_Rect rect;

            Tile(SDL_Texture *_clip, SDL_Rect _rect) :
                clip(_clip), rect(_rect) {}
        };

        std::vector<Tile *> tiles;

        int clipPerRow,
            clipPerColumn;

        void setTiles(std::string image, int rows, int cols);
        int eventHandle();
        void drawPuzzle();
        void menu();
        void puzzle();

    public:
        Game();
};

#endif
