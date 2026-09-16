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
    case 3:
        newLevel.platforms.push_back( createPlatform( 50,200,2 ) );
        newLevel.platforms.push_back( createPlatform( 250,400,5 ) );
        newLevel.platforms.push_back( createPlatform( 350,300,4 ) );
        newLevel.platforms.push_back( createPlatform( 550,200,3 ) );
        newLevel.platforms.push_back( createPlatform( 350,150,4 ) );
        newLevel.platforms.push_back( createPlatform( 100,100,2 ) );

        newLevel.cheeses.push_back( createCheese(275,340));
        newLevel.cheeses.push_back( createCheese(65,140));
        newLevel.cheeseCount = 2;

        newLevel.enemies.push_back( createEnemy( 380,225,1,2) );
        newLevel.enemies.push_back( createEnemy( 280,170,2,50) );
        break;

    case 4:
        newLevel.platforms.push_back( createPlatform( 120,400,15 ) );
        newLevel.platforms.push_back( createPlatform( 10,300,4 ) );
        newLevel.platforms.push_back( createPlatform( 700,300,4 ) );
        newLevel.platforms.push_back( createPlatform( 360,300,4 ) );
        newLevel.platforms.push_back( createPlatform( 0,210,2 ) );
        newLevel.platforms.push_back( createPlatform( 930,210,2 ) );
        newLevel.platforms.push_back( createPlatform( 160,120,3 ) );
        newLevel.platforms.push_back( createPlatform( 680,120,3 ) );
        newLevel.platforms.push_back( createPlatform( 400,160,2 ) );

        newLevel.cheeses.push_back( createCheese(275,340));
        newLevel.cheeses.push_back( createCheese(600,340));
        newLevel.cheeses.push_back( createCheese(870,340));
        newLevel.cheeses.push_back( createCheese(420,100));
        newLevel.cheeses.push_back( createCheese(10,150));
        newLevel.cheeses.push_back( createCheese(940,150));
        newLevel.cheeseCount = 6;

        newLevel.enemies.push_back( createEnemy( 380,325,1,0) );
        newLevel.enemies.push_back( createEnemy( 130,325,1,0) );
        newLevel.enemies.push_back( createEnemy( 170,45,1,6,-1) );
        newLevel.enemies.push_back( createEnemy( 700,45,1,7) );


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
