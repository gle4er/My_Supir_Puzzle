#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "../header/menu.h"
#include "../header/handler.h"

void Menu::makeTitle(std::string title, int posX, int posY)
{
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/TTF/Ubuntu-R.ttf", 20);
    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface *msgSurf = TTF_RenderText_Blended(font, title.c_str(), white);
    SDL_Texture *pictMsg = SDL_CreateTextureFromSurface(mRenderer, msgSurf);
    SDL_Rect msgRect = {
        posX, posY, 
        msgSurf->w, msgSurf->h
    };
    SDL_RenderCopy(mRenderer, pictMsg, NULL, &msgRect);

    SDL_FreeSurface(msgSurf);
    SDL_DestroyTexture(pictMsg);
	TTF_CloseFont(font);
}

void Menu::makePict(std::string path, int posX, int posY)
{
    SDL_Surface *surf = IMG_Load(path.c_str());
    SDL_Texture *picture = SDL_CreateTextureFromSurface(mRenderer, surf);
    SDL_Rect pictRect = { 
        posX,
        posY,
        170,
        170
    };
    SDL_RenderCopy(mRenderer, picture, NULL, &pictRect);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(picture);
}

void Menu::menu()
{
    SDL_SetRenderDrawColor(mRenderer, 0x26, 0x24, 0x5D, 0x00);
	SDL_RenderClear(mRenderer);
    SDL_RenderPresent(mRenderer);

    std::string path = "./pictures/";
    std::vector<std::string> imgPaths;

    for (int i = 1; i <= 5; i++)
        imgPaths.push_back(path + std::to_string(i) + ".jpg");
    std::vector<std::string> piecesNum = { "35", "54", "120" };
    int imgIndex = 0;
    int pieceIndex = 0;
    bool exit = false;
    Handler menuHandle;
    while (!exit) {
        action rc = menuHandle.getEvent();
        makeTitle("Choose picture", 100, 100);
        makeTitle("Choose pieces", 550, 100);
        makeTitle("<", 30, 250);
        makeTitle(">", 300, 250);
        makeTitle("<", 580, 250);
        makeTitle(">", 640, 250);
        makeTitle("START", 380, 450);

        makePict(imgPaths[imgIndex], 87, 180);
        makeTitle(piecesNum[pieceIndex], 605, 250);

        SDL_RenderPresent(mRenderer);
	    SDL_RenderClear(mRenderer);

        if (rc == PICT_CH_BACK) {
            imgIndex--;
            if (imgIndex < 0)
                imgIndex = imgPaths.size() - 1;
        }

        else if (rc == PICT_CH_FRONT) {
            imgIndex++;
            if (imgIndex > (int) imgPaths.size() - 1)
                imgIndex = 0;
        }

        else if (rc == PIE_CH_BACK) {
            pieceIndex--;
            if (pieceIndex < 0)
                pieceIndex = piecesNum.size() - 1;
        }
        
        else if (rc == PIE_CH_FRONT) {
            pieceIndex++;
            if (pieceIndex > (int) piecesNum.size() - 1)
                pieceIndex = 0;
        }

        else if (rc == START) {
            *pictPath = imgPaths[imgIndex];
            *pieces = pieceIndex;
            exit = true;
        }

		else if (rc == EXIT) {
			*pieces = -1;
			exit = true;
		}

        SDL_Delay(32);
    }

}

Menu::~Menu()
{
	TTF_Quit();
}

Menu::Menu(SDL_Window *_gWindow, SDL_Renderer *_gRendrer, 
        std::string *_pictPath, int *_pieces) : 
    gWindow(_gWindow), mRenderer(_gRendrer), 
    pictPath(_pictPath), pieces(_pieces)
{
    TTF_Init();
    menu();
}
