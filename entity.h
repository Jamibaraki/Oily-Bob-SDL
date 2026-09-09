#pragma once

class Entity {
    public:
        int xPos;
        int yPos;
        SDL_Texture *texture;
        int width;
        int height;
        int lBound;
        int rBound;
        int speed;
        int direction;
        int ySpeed;
};
