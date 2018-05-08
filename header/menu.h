#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <string>

class Menu
{
    private:
        SDL_Renderer *mRenderer;
        SDL_Window *gWindow;

        SDL_Texture *makeTitle(std::string title, int posX, int posY);
        void menu();

    public:
        Menu(SDL_Window *);
        ~Menu();
};

#endif
