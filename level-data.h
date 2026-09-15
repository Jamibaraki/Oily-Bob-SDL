#pragma once
#include <vector>
#include "entity.h"
#include "platform.h"
#include "cheese.h"
#include "enemy.h"

class LevelData {

    public:
        int cheeseCount;
        std::vector<Enemy> enemies;
        std::vector<Platform> platforms;
        std::vector<Cheese> cheeses;

};

