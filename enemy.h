#pragma once

class Enemy {
    public:
        int xPos;
        int yPos;
        int type; //1 for alien 2 for bee?
        int platform; //associated platform
        int direction;
};
