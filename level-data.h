#pragma once
#include <vector>
#include "entity.h"
#include "platform.h"
#include "cheese.h"

class LevelData {

    public:
        //int cheeses;
        std::vector<Entity> entities;
        std::vector<Platform> platforms;
        std::vector<Cheese> cheeses;
};

