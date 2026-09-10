#pragma once
int createEntity( Entity *entity, const std::string& graphic_name );

int updateGame();

// draws sprites. Flipped sprites are mirrored when moving right
void drawSprite(Entity *entity, SDL_FRect *dst_rect,bool flipped = false);



void bobJump();
