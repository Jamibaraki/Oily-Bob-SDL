#pragma once
#include "level-data.h"

LevelData makeLevel(int level);


Platform createPlatform( int xPos, int yPos, int width);
Cheese createCheese( int xPos, int yPos );
Enemy createEnemy( int xPos, int yPos, int type, int platform,int direction = 1 );
