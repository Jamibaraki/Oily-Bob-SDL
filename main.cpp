#include <iostream>

using namespace std;

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#define Ground 329
#define PLATFORM_BLOCK_WIDTH 60

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "entity.h"
#include "platform.h"
#include "level-data.h"
#include "level-maker.h"
#include "main.h"

/**
Oily Bob SDL Version
Conversion of my Allegro platform game to SDL

Todo:
platforms displaying
get the game working
collisions
scoring
levels
high score table
horizontal movement acceleration
prevent chain jumping trick?

get a sprite displaying - DONE
get keyboard also controlling bob - DONE
get joystick controlling bob - DONE
fullscreen mode - DONE
scrolling - DONE
get some text displaying - DONE
get a sound playing - DONE
get sound playing correctly - DONE
get jumping in - DONE
**/



//SDL's more complex examples combine these into appstate structure.. may be worth doing
/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

Uint64 last_step; // for getting animations timed right.

//audio
static SDL_AudioStream *stream = NULL;
static Uint8 *wav_data = NULL;
static Uint32 wav_data_len = 0;


//various game entity types
Entity bee;
Entity background;
Entity bob;
Entity cheese;
Entity alien;
Entity platform;

//don't necessarily need this.. could just pull them dynamically from level maker
LevelData levels[10];
LevelData currentLevel;


//horizontal scroll tracking
int scrollOffsetX = 0;

//control flags
bool pressJump = false;

bool jumpPeak = false;
bool bobYCollision;

Uint16 lives = 0;
Uint16 score = 0;



/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_AudioSpec spec;
    char *wav_path = NULL;

    SDL_SetAppMetadata("Oily Bob", "1.0", "com.jamibaraki.oilybob");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD | SDL_INIT_AUDIO )) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    currentLevel = makeLevel(0);

    //Joypad setup - opens a gamepad if connected. currently just the first one
    int count = 0;
    SDL_JoystickID *ids = SDL_GetGamepads(&count);
    if (count > 0) {
        SDL_Gamepad *gp = SDL_OpenGamepad(ids[0]);
        if (gp) {
            SDL_Log("Opened gamepad");
        }
    }
    SDL_free(ids);


    //fullscreen or windowed
    //if (!SDL_CreateWindowAndRenderer("jamibaraki/oilybob", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    if (!SDL_CreateWindowAndRenderer("jamibaraki/oilybob", 640, 480, SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);


    /* Load the .wav  */
    SDL_asprintf(&wav_path, "%ssound/jump.wav", SDL_GetBasePath());
    if (!SDL_LoadWAV(wav_path, &spec, &wav_data, &wav_data_len)) {
        SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_free(wav_path);  /* done with this string. */

    /* Create our audio stream in the same format as the .wav file. It'll convert to what the audio hardware wants. */
    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
    if (!stream) {
        SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* SDL_OpenAudioDeviceStream starts the device paused. You have to tell it to start! */
    SDL_ResumeAudioStreamDevice(stream);


    //create game entities and load the graphics
    createEntity(&bee,"bee.bmp");
    createEntity(&background,"background.bmp");
    createEntity(&bob,"bob.bmp");
    createEntity(&cheese,"cheese.bmp");
    createEntity(&alien,"enemy.bmp");
    createEntity(&platform,"platform.bmp");
    bee.xPos = 100;
    bee.yPos = 50;
    bee.speed = 2;
    bee.direction = 1;
    bee.lBound = 50;
    bee.rBound = 220;

    bob.xPos=250;
    bob.yPos = 350;
    bob.direction = 0;
    bob.speed = 3;

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

int createEntity( Entity *entity, const std::string& graphic_name ){
    SDL_Surface *surface = NULL;
    char *bmp_path = NULL;
    // load the graphics
    SDL_asprintf(&bmp_path, "%sgraphics/%s", SDL_GetBasePath(),graphic_name.c_str());  /* allocate a string of the full file path */
    surface = SDL_LoadBMP(bmp_path);
    if (!surface) {
        SDL_Log("Couldn't load png: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_free(bmp_path);


    entity->width = surface->w;
    entity->height = surface->h;

    //do the transparency
    const SDL_PixelFormatDetails *fmt = SDL_GetPixelFormatDetails(surface->format);
    const SDL_Palette *palette = SDL_GetSurfacePalette(surface);   // can be NULL
    Uint32 key = SDL_MapRGB(fmt, palette, 255, 0, 255);
    if (!SDL_SetSurfaceColorKey(surface, true, key)) {
        SDL_Log("SDL_SetSurfaceColorKey failed: %s", SDL_GetError());
    }

    entity->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!entity->texture) {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_DestroySurface(surface);

}

//update the game state
int updateGame(){

    if (pressJump){
        bobJump();
    }

    //do all the jump tracking. remember in other version -= yspeed to sprite. trying to flip!!
    if(bob.ySpeed < -11)
         jumpPeak = true;

    if(bob.yPos < Ground && bobYCollision == false)
                 bob.ySpeed ++;

    if ( bob.yPos > Ground) {
        bob.ySpeed = 0;
        bob.yPos = Ground;
        jumpPeak = false;
    }


    //animate the bee. Eventually generalize to all sprites
    bee.xPos += (bee.speed * bee.direction);
    if(bee.xPos>bee.rBound){
        bee.direction = -1;
        bee.xPos-=bee.width/2;
    }
    if (bee.xPos<bee.lBound){
        bee.direction = 1;
        bee.xPos+=bee.width/2;
    }

    bob.xPos += bob.speed * bob.direction;
    bob.yPos += bob.ySpeed;
    int bobVector = bob.speed * bob.direction;
    //check if we need to scroll
    if(bob.xPos - scrollOffsetX > 480 && bobVector > 0)
        scrollOffsetX += bobVector;

    if(bob.xPos - scrollOffsetX < 80 && bobVector < 0)
        scrollOffsetX += bobVector; //which will be negative

    if (scrollOffsetX < 0)
        scrollOffsetX = 0;
    if(scrollOffsetX > 360)
        scrollOffsetX = 360;




}

void bobJump(){

    if (!jumpPeak){


        //set this to zero so it just ads the sample if there's nothing already playing
        if (SDL_GetAudioStreamQueued(stream) == 0) {
            // feed more data to the stream. It will queue at the end, and trickle out as the hardware needs more data.
            SDL_PutAudioStreamData(stream, wav_data, wav_data_len);

        }

        bob.ySpeed-=2;
    }




}

//handle keyboard input
static SDL_AppResult handle_key_event_(SDL_Keycode key_code, int isDown)
{
    switch (key_code) {
#ifndef SDL_PLATFORM_EMSCRIPTEN
    /* Quit. */
    case SDLK_ESCAPE:
    case SDLK_Q:
        return SDL_APP_SUCCESS;
#endif
    /* Restart the game as if the program was launched. */
    case SDLK_SPACE:
        //jump
        pressJump = isDown;

        //audio test

        break;

    case SDLK_RIGHT:
        //move bob right
        bob.direction = isDown;
        break;

    case SDLK_LEFT:
        //move bob left
        bob.direction = isDown*-1;
        break;

    default:
        break;
    }
    return SDL_APP_CONTINUE;
}



//handle joypad input
static SDL_AppResult handle_joypad_event_(int button, int isDown){

    //SDL_Log( SDL_GetGamepadStringForButton((SDL_GamepadButton)button) );
    string input_name = SDL_GetGamepadStringForButton((SDL_GamepadButton)button);
    if( input_name == "dpright" ){
        bob.direction = isDown;
    } else if (input_name == "dpleft" ){

        bob.direction = isDown*-1;
    }

    return SDL_APP_CONTINUE;
}


/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }


    if(event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_KEY_UP ){
        return handle_key_event_(event->key.key,event->key.down);
    }

    if(event->type == SDL_EVENT_GAMEPAD_BUTTON_DOWN || event->type == SDL_EVENT_GAMEPAD_BUTTON_UP){
        return handle_joypad_event_(event->gbutton.button,event->gbutton.down);
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* Runs every frame. Our heartbeat */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_FRect dst_rect;

    const double now = ((double)SDL_GetTicks());
    SDL_SetRenderDrawColorFloat(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */

    //think this looks glitchy, should use floats to smooth out the blips
    const int ticks_per_frame = 16; // roughly 60 fps

    //calculate the frames and lets put the actual game updates in a separate function for clarity
    while ( now > last_step + ticks_per_frame ){
        updateGame();
        last_step += ticks_per_frame;
    }



    // the rendering doesn't need to be repeated. That would be silly.

    //rendering does however need to be functionalized, as it is already quite silly

    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);

    drawSprite(&background,&dst_rect);


    drawSprite(&bee,&dst_rect,true);

    drawSprite(&bob, &dst_rect);


    //do debug text
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDebugTextFormat(renderer, 250, 15, "Score: %i",score);
    SDL_RenderDebugTextFormat(renderer, 10, 15, "Lives: %i",lives);


    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

void drawSprite(Entity *entity, SDL_FRect *rect,bool flipped ){
    rect->x = entity->xPos-scrollOffsetX;
    rect->y = entity->yPos;
    rect->w = (float) entity->width;
    rect->h = (float) entity->height;
    if (flipped ){
        rect->w *= -entity->direction;
    }

    SDL_RenderTexture(renderer, entity->texture, NULL, rect);

}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(bee.texture); //not sure if both are needed!
    SDL_DestroyTexture(background.texture);
    SDL_DestroyTexture(bob.texture);
    SDL_free(wav_data);
    /* SDL will clean up the window/renderer for us. */
}
