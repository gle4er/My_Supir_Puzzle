#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <string>

class Menu
{
    private:
        SDL_Renderer *mRenderer;
        SDL_Window *gWindow;
        std::string *pictPath;
        int *pieces;

        void makeTitle(std::string title, int posX, int posY);
        void makePict(std::string path, int posX, int posY);
        void menu();

    public:
        Menu(SDL_Window *, SDL_Renderer *,
                std::string *_pictPath, int *_pieces);
        ~Menu();
};

#endif
