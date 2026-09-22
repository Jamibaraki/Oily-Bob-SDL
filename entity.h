#pragma once

#include <SDL3/SDL.h>


class Entity {
    public:
        int xPos;
        int yPos;
        SDL_Texture *texture;
        int width;
        int height;
        int speed;
        int direction;
        int ySpeed;
};
