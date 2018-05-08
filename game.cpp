#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <random>

#include "game.h"

void Game::setTiles(std::string image, int row, int col)
{
    SDL_Surface* surf = IMG_Load(image.c_str());

    if (surf == nullptr)
        std::cout << IMG_GetError() << std::endl;
    else
        std::cout << "Tiling image" << std::endl;

    this->clipPerRow = row;
    this->clipPerColumn = col;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surf);
    SDL_FreeSurface(surf);

    int textureWidth = IMAGE_WIDTH / this->clipPerRow;
    int textureHeight = IMAGE_HEIGHT / this->clipPerColumn;

    std::default_random_engine gen;
    gen.seed(std::random_device()());
    std::uniform_int_distribution<int> 
        randX(textureWidth, SCREEN_WIDTH - textureWidth);
    std::uniform_int_distribution<int> 
        randY(textureHeight, SCREEN_HEIGHT - textureHeight);

    for (int i = 0; i < clipPerRow; i++) {
        for (int j = 0; j < clipPerColumn; j++) {
            SDL_Texture *clip = 
                SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, 
                        SDL_TEXTUREACCESS_TARGET, 
                        IMAGE_WIDTH / this->clipPerRow, 
                        IMAGE_HEIGHT / this->clipPerColumn);

            SDL_SetTextureBlendMode(clip, SDL_BLENDMODE_BLEND);
            SDL_Rect rect = { 
                i * IMAGE_WIDTH / this->clipPerRow, 
                j * IMAGE_HEIGHT / this->clipPerColumn, 
                textureWidth,
                textureHeight
            };

            SDL_SetRenderTarget(gRenderer, clip);
            SDL_RenderCopy(gRenderer, texture, &rect, NULL);

            int x = randX(gen),
                y = randY(gen);
            rect.x = x;
            rect.y = y;

            this->tiles.push_back(new Tile(clip, rect));

        }
    }

    std::cout << "Tiling complete" << std::endl;
}

int Game::eventHandle()
{
    static int movedTile = -1;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return 1;
        } 

        if (e.button.button == SDL_BUTTON(SDL_BUTTON_LEFT) &&
                e.type == SDL_MOUSEMOTION) {
            if (movedTile == -1)
                break;
            int x = e.motion.xrel, 
                y = e.motion.yrel;
            tiles[movedTile]->rect.x += x;
            tiles[movedTile]->rect.y += y;
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
        }

        if (e.type == SDL_MOUSEBUTTONUP)
            movedTile = -1; 

    }
    return 0;
}

void Game::draw()
{
    int ticksPresent = SDL_GetTicks();
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00,0x00,0x00);
    SDL_RenderClear(gRenderer);
    for (int i = 0; i < this->clipPerRow; i++) {
        for (int j = 0; j < this->clipPerColumn; j++) {

            Tile *tmp_tile = this->tiles[i * clipPerColumn + j];

            SDL_Rect rect = {
                tmp_tile->rect.x,
                tmp_tile->rect.y,
                IMAGE_WIDTH / this->clipPerRow, 
                IMAGE_HEIGHT / this->clipPerColumn
            };

            SDL_RenderCopy(gRenderer, tmp_tile->clip, NULL, &rect);
        }
    }

    SDL_RenderPresent(gRenderer);

    ticksPresent = SDL_GetTicks() - ticksPresent;
    if (ticksPresent < TICK_LIMIT)
        SDL_Delay(TICK_LIMIT - ticksPresent);
}

void Game::puzzle()
{
    SDL_SetRenderTarget(gRenderer, NULL);
    bool exit = false;
    while (!exit) {
        if (eventHandle() == 1)
            exit = true;
        draw();
    }
}

Game::Game()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    gWindow = SDL_CreateWindow("MDA", 
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    setTiles("./1.jpg", 8, 7);

    puzzle();
}
