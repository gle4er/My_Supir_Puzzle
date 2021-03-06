#include <iostream>
#include <string>
#include <vector>
#include <random>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../header/game.h"
#include "../header/menu.h"
#include "../header/handler.h"

void Game::setTiles(std::string image, int row, int col)
{
    SDL_Surface* surf = IMG_Load(image.c_str());

    if (surf == nullptr)
        std::cout << IMG_GetError() << std::endl;

    this->clipPerRow = row;
    this->clipPerColumn = col;

    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(gRenderer, surf);
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
            SDL_RenderCopy(gRenderer, imageTexture, &rect, NULL);

            rect.x = randX(gen);
            rect.y = randY(gen);

            this->tiles.push_back(new Tile(clip, rect));

        }
    }

	SDL_DestroyTexture(imageTexture);
}

void Game::drawPuzzle()
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

    Handler puzzleHandle(tiles);

    while (!exit) {
        action rc = puzzleHandle.getEvent();
        if (rc == EXIT)
            exit = true;

        drawPuzzle();
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
    SDL_SetRenderTarget(gRenderer, NULL);

    std::string pictPath;
    int pieceIndex = 0;
    new Menu(gWindow, gRenderer, &pictPath, &pieceIndex);
    int rows = 0, 
        cols = 0;
	if (pieceIndex != -1) {

        if (pieceIndex == 0) {
            rows = 5;
            cols = 7;
        }
        else if (pieceIndex == 1) {
            rows = 7;
            cols = 8;
        }
        else if (pieceIndex == 2) {
            rows = 10;
            cols = 12;
        }

	    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	    SDL_RenderClear(gRenderer);

        setTiles(pictPath, rows, cols);
        puzzle();
    }
}
