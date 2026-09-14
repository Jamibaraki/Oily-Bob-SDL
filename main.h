#pragma once
int createEntity( Entity *entity, const std::string& graphic_name );

int updateGame();

// draws sprites. Flipped sprites are mirrored when moving right
void drawSprite(Entity *entity, SDL_FRect *dst_rect,int flipped = 0);

void bobJump();

void bobDie();

bool checkCollision( SDL_Rect a, SDL_Rect b );
