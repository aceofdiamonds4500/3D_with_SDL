/*
 * This example creates an SDL window and renderer, and then draws some lines
 * to it every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

typedef struct {
    double x;
    double y;
    double angle;
} Character;

Character player;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Lines", "1.0", "com.example.renderer-lines");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/lines", 640, 480, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    player.x = 320.0;
    player.y = 240.0;
    player.angle = 0.0;
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_KEY_DOWN:
            SDL_Keycode sym = event->key.key;
            if (sym == SDLK_W){ player.x = player.x + SDL_cos(player.angle); player.y = player.y + SDL_sin(player.angle);}
            if (sym == SDLK_A){ player.angle = player.angle + 0.1;}
            if (sym == SDLK_S){ player.x = player.x - SDL_cos(player.angle); player.y = player.y - SDL_sin(player.angle);}
            if (sym == SDLK_D){ player.angle = player.angle - 0.1;}
            break;
        case SDL_EVENT_KEY_UP:

            break;
        default:
            break;
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    int i;

    /* Lines (line segments, really) are drawn in terms of points: a set of
       X and Y coordinates, one set for each end of the line.
       (0, 0) is the top left of the window, and larger numbers go down
       and to the right. This isn't how geometry works, but this is pretty
       standard in 2D graphics. */
    static const SDL_FPoint line_points[] = {
        { 100, 354 }, { 220, 230 }, { 140, 230 }, { 320, 100 }, { 500, 230 },
        { 420, 230 }, { 540, 354 }, { 400, 354 }, { 100, 354 }
    };
    static const SDL_FPoint line_points1[] = {
        { 310,240 }, { 330, 240 },
        { 325,235 }, {330,240 }, {325,245 }
    };
    //printf("%f %f %f\n", player.x, player.y, player.angle);
    /* as you can see from this, rendering draws over whatever was drawn before it. */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */

    /* You can draw lines, one at a time, like these brown ones... */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderLine(renderer, 240, 450, 400, 450);
    SDL_RenderLine(renderer, 240, 356, 400, 356);
    SDL_RenderLine(renderer, 240, 356, 240, 450);
    SDL_RenderLine(renderer, 400, 356, 400, 450);

    /* You can also draw a series of connected lines in a single batch... */
    //SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    //SDL_RenderLines(renderer, line_points1, SDL_arraysize(line_points1));
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderLine(renderer, player.x,player.y , SDL_cos(player.angle)*20 + player.x,SDL_sin(player.angle)*20 + player.y);


    SDL_RenderPresent(renderer);  /* put it all on the screen! */

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}

