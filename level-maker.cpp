#pragma once
#include "level-maker.h"

LevelData makeLevel(int level){
    //return 1;
    //switch case here after testing level 1

    LevelData newLevel = LevelData();

    newLevel.platforms.push_back( createPlatform( 50,300,3 ) );
    newLevel.platforms.push_back( createPlatform( 250,400,5 ) );
    newLevel.platforms.push_back( createPlatform( 450,300,3 ) );

    return newLevel;
}


Platform createPlatform( int xPos, int yPos, int width){
    Platform p = Platform();
    p.xPos = xPos;
    p.yPos = yPos;
    p.width = width;
    return p;
}
