#include <iostream>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "../header/menu.h"
#include "../header/handler.h"

SDL_Texture *Menu::makeTitle(std::string title, int posX, int posY)
{
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/TTF/Ubuntu-R.ttf", 20);
    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface *msgSurf = TTF_RenderText_Blended(font, title.c_str(), white);
    SDL_Texture *pictMsg = SDL_CreateTextureFromSurface(mRenderer, msgSurf);
    SDL_Rect msgRect = {
        posX, posY, 
        msgSurf->w, msgSurf->h
    };
    SDL_FreeSurface(msgSurf);
    SDL_RenderCopy(mRenderer, pictMsg, NULL, &msgRect);

    return pictMsg;
}

void Menu::menu()
{
    SDL_SetRenderDrawColor(mRenderer, 0x26, 0x24, 0x5D, 0x00);
	SDL_RenderClear(mRenderer);
    SDL_RenderPresent(mRenderer);

    SDL_Texture *pictMsg = makeTitle("Choose picture", 100, 100);
    SDL_Texture *pieceMsg = makeTitle("Choose pieces", 550, 100);
    SDL_Texture *leftArrowOne = makeTitle("<", 30, 250);
    SDL_Texture *rightArrowOne = makeTitle(">", 300, 250);
    SDL_Texture *leftArrowTwo = makeTitle("<", 580, 250);
    SDL_Texture *rightArrowTwo = makeTitle(">", 640, 250);

    std::string path = "./pictures/";
    std::vector<std::string> imgPaths;

    for (int i = 1; i <= 3; i++)
        imgPaths.push_back(path + std::to_string(i) + ".jpg");

    bool exit = false;
    int num = 0;

    Handler menuHandle;
    while (!exit) {
        action rc = menuHandle.getEvent();
        if (rc == EXIT)
            exit = true;

        SDL_Surface* surf = IMG_Load(imgPaths[num].c_str());
        SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surf);
        SDL_FreeSurface(surf);

        SDL_Rect rect = { 
            200,
            200,
            150,
            150
        };
        SDL_RenderCopy(mRenderer, texture, NULL, &rect);

        SDL_RenderPresent(mRenderer);
        SDL_Delay(32);
    }

    SDL_DestroyTexture(pictMsg);
    SDL_DestroyTexture(pieceMsg);
    SDL_DestroyTexture(leftArrowOne);
    SDL_DestroyTexture(leftArrowTwo);
    SDL_DestroyTexture(rightArrowOne);
    SDL_DestroyTexture(rightArrowTwo);
}

Menu::~Menu()
{
    SDL_DestroyRenderer(mRenderer);
}

Menu::Menu(SDL_Window *_gWindow) : gWindow(_gWindow)
{
    mRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderTarget(mRenderer, NULL);
    if (TTF_Init() == -1)
        std::cout << "SDL_ttf Error: " << TTF_GetError();
    menu();
}
