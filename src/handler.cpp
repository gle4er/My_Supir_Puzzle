#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

#include "../header/handler.h"

action Handler::getPuzzleHandle()
{
    static int movedTile = -1;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return EXIT;
        } 

        if (tiles.size() == 0) {
            std::cout << "Error: tiles not found" << std::endl;
            return UNDEFINED;
        }

        if (e.button.button == SDL_BUTTON(SDL_BUTTON_LEFT) &&
                e.type == SDL_MOUSEMOTION) {
            if (movedTile == -1)
                break;
            int x = e.motion.xrel, 
                y = e.motion.yrel;
            tiles[movedTile]->rect.x += x;
            tiles[movedTile]->rect.y += y;
            return PUZZLEMOVE;
        }

        else if (e.button.button == SDL_BUTTON(SDL_BUTTON_LEFT)) {
            int x, y; 
            SDL_GetMouseState(&x, &y);
            for (int i = 0; i < (int) tiles.size(); i++) {
                SDL_Rect area = tiles[i]->rect;
                int xLow = area.x,
                    xHigh = area.x + area.w,
                    yLow = area.y,
                    yHigh = area.y + area.h;

                if (xLow < x && x < xHigh) {
                    if (yLow < y && y < yHigh) {
                        movedTile = i;
                        break;
                    }
                }
            }

            return PUZZLECHOOSE;
        }

        if (e.type == SDL_MOUSEBUTTONUP) {
            movedTile = -1; 
            return PUZZLEDROP;
        }

    }
    return UNDEFINED;
}

action Handler::getMenuHandle()
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return EXIT;
        } 
    }
    return UNDEFINED;
}

action Handler::getEvent() 
{
    if (tiles.size())
        return getPuzzleHandle();
    else
        return getMenuHandle();
}

Handler::Handler() = default;

Handler::Handler(std::vector<Tile *> &_tiles) :
    tiles(_tiles) {}
