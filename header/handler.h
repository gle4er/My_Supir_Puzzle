#ifndef HANDLE_H
#define HANDLE_H

#include <vector>
#include "game.h"

enum action { 
    EXIT, PUZZLEMOVE, PUZZLECHOOSE, PUZZLEDROP, UNDEFINED,
    PICT_CH_BACK, PICT_CH_FRONT, PIE_CH_BACK, PIE_CH_FRONT, START
}; 

class Handler {
    private:
        action getPuzzleHandle();
        action getMenuHandle();
        std::vector<Tile *> tiles;

    public:
        Handler();
        Handler(std::vector<Tile *> &tiles);
        action getEvent();
};

#endif
