#pragma once
#include "level-maker.h"

LevelData makeLevel(int level){

    LevelData newLevel = LevelData();
    //switch case here after testing level 1
    switch (level ){
    case 1:
        newLevel.platforms.push_back( createPlatform( 50,300,3 ) );
        newLevel.platforms.push_back( createPlatform( 250,400,5 ) );
        newLevel.platforms.push_back( createPlatform( 450,300,3 ) );

        newLevel.cheeses.push_back( createCheese(75,240));
        newLevel.cheeses.push_back( createCheese(300,340));
        newLevel.cheeses.push_back( createCheese(470,240));
        newLevel.cheeseCount = 3;

        newLevel.enemies.push_back( createEnemy( 470,230,1,2) );

        //seems in the other version, platform is just used to store a horizontal range for the bee..
        newLevel.enemies.push_back( createEnemy( 150,200,2,50));
        break;
    case 2:
        newLevel.platforms.push_back( createPlatform( 200,400,8 ) );
        newLevel.platforms.push_back( createPlatform( 40,330,3 ) );
        newLevel.platforms.push_back( createPlatform( 180,270,4 ) );
        newLevel.platforms.push_back( createPlatform( 480,270,2 ) );
        newLevel.platforms.push_back( createPlatform( 700,270,2 ) );

        newLevel.cheeses.push_back( createCheese(205,335));
        newLevel.cheeses.push_back( createCheese(485,210));
        newLevel.cheeses.push_back( createCheese(710,210));
        newLevel.cheeseCount = 3;

        newLevel.enemies.push_back( createEnemy( 250,325,1,0) );
        newLevel.enemies.push_back( createEnemy( 340,325,1,0,-1) );


        break;
    }





    return newLevel;
}

Enemy createEnemy( int xPos, int yPos, int type, int platform, int direction ){
    Enemy e = Enemy();
    e.xPos = xPos;
    e.yPos = yPos;
    e.platform = platform;
    e.type = type;
    e.direction = direction;
    return e;
}

Cheese createCheese( int xPos, int yPos ){
    Cheese c = Cheese();
    c.xPos = xPos;
    c.yPos = yPos;
    c.status = true;
    return c;
}

Platform createPlatform( int xPos, int yPos, int width){
    Platform p = Platform();
    p.xPos = xPos;
    p.yPos = yPos;
    p.width = width;
    return p;
}
