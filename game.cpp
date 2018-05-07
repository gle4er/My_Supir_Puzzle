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

    int width = surf->w;
    int height = surf->h;
    int clipPerRow = row;
    int clipPerColumn = col;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surf);
    SDL_FreeSurface(surf);

    for (int i = 0; i < clipPerRow; i++) {
        for (int j = 0; j < clipPerColumn; j++) {
            SDL_Texture *clip = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, 
                    SDL_TEXTUREACCESS_TARGET, width / clipPerRow, height / clipPerColumn);

            SDL_SetTextureBlendMode(clip, SDL_BLENDMODE_BLEND);
            SDL_Rect rect = { 
                i * width / clipPerRow, 
                j * height / clipPerColumn, 
                width / clipPerRow, 
                height / clipPerColumn 
            };

            SDL_SetRenderTarget(gRenderer, clip);
            SDL_RenderCopy(gRenderer, texture, &rect, NULL);

            tiles.push_back(clip);
        }
    }

    SDL_SetRenderTarget(gRenderer, NULL);
    int x = 100;
    int y = 100;
    bool quit = false;
    while(!quit) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) if(e.type == SDL_QUIT) quit = 1;
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00,0x00,0x00);
        SDL_RenderClear(gRenderer);
        for(int i = 0; i < clipPerRow; i++) {
            for(int j = 0; j < clipPerColumn; j++) {

                SDL_Rect rect = {
                    x + i * width / clipPerRow + j,
                    y + j * height/clipPerColumn + j, 
                    width / clipPerRow, 
                    height / clipPerColumn
                };

                SDL_RenderCopy(gRenderer,tiles[i * clipPerColumn + j], NULL, &rect);
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
}
