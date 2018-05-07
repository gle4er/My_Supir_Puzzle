#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

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
                IMAGE_WIDTH / this->clipPerRow, 
                IMAGE_HEIGHT / this->clipPerColumn 
            };

            SDL_SetRenderTarget(gRenderer, clip);
            SDL_RenderCopy(gRenderer, texture, &rect, NULL);

            this->tiles.push_back(new Tile(clip, rect.x + j, rect.y + i));

        }
    }

    std::cout << "Tiling complete" << std::endl;
}

void Game::puzzle()
{
    SDL_SetRenderTarget(gRenderer, NULL);
    bool quit = false;
    while(!quit) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) if(e.type == SDL_QUIT) quit = 1;
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00,0x00,0x00);
        SDL_RenderClear(gRenderer);
        for(int i = 0; i < this->clipPerRow; i++) {
            for(int j = 0; j < this->clipPerColumn; j++) {

                Tile *tmp_tile = this->tiles[i * clipPerColumn + j];

                SDL_Rect rect = {
                    tmp_tile->x + 100,
                    tmp_tile->y + 100,
                    IMAGE_WIDTH / this->clipPerRow, 
                    IMAGE_HEIGHT / this->clipPerColumn
                };

                SDL_RenderCopy(gRenderer, tmp_tile->clip, NULL, &rect);
            }
        }
        SDL_RenderPresent(gRenderer);
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
