#include <iostream>

using namespace std;

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "entity.cpp"
#include "main.h"

/**
Oily Bob SDL Version
Conversion of my Allegro platform game to SDL

Todo:
get some text displaying
get a sprite displaying - DONE
get a sound playing
get joystick controlling bob
get keyboard also controlling bob
get the game working


**/


/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;


Entity bee;
Entity background;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{



    SDL_SetAppMetadata("Oily Bob", "1.0", "com.jamibaraki.oilybob");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("jamibaraki/oilybob", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);


    createEntity(&bee,"bee.bmp");
    createEntity(&background,"background.bmp");
    bee.xPos = 0;
    bee.yPos = 0;

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


/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_FRect dst_rect;
    const double now = ((double)SDL_GetTicks()) / 1000.0;  /* convert from milliseconds to seconds. */
    SDL_SetRenderDrawColorFloat(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */

    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);


    dst_rect.x = background.xPos;
    dst_rect.y = background.yPos;

    dst_rect.w = (float) background.width;
    dst_rect.h = (float) background.height;

    SDL_RenderTexture(renderer, background.texture, NULL, &dst_rect);

    dst_rect.x = bee.xPos;
    dst_rect.y = bee.yPos;

    dst_rect.w = (float) bee.width;
    dst_rect.h = (float) bee.height;


    SDL_RenderTexture(renderer, bee.texture, NULL, &dst_rect);






    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}



/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(bee.texture); //not sure if both are needed!
    SDL_DestroyTexture(background.texture);
    /* SDL will clean up the window/renderer for us. */
}
