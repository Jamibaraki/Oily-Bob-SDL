#pragma once
#include "level-maker.h"

LevelData makeLevel(int level){
    //return 1;
    //switch case here after testing level 1

    LevelData newLevel = LevelData();
    Platform p = Platform();
    p.xPos = 100;
    p.yPos = 250;
    p.width = 5;
    newLevel.platforms.push_back(p);
    return newLevel;
}
