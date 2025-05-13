#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#define WIDTH 640
#define HEIGHT 480
#define MID_X WIDTH / 2
#define MID_Y HEIGHT / 2

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
int view_mode = 1;

int xa = 240; int xb = 400;
int ya = 450; int yb = 356;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
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
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_KEY_DOWN:
            SDL_Keycode sym = event->key.key;
            if (sym == SDLK_W){ player.x = player.x + SDL_cos(player.angle); player.y = player.y + SDL_sin(player.angle);}
            if (sym == SDLK_A){ player.x = player.x + SDL_sin(player.angle); player.y = player.y - SDL_cos(player.angle);}
            if (sym == SDLK_S){ player.x = player.x - SDL_cos(player.angle); player.y = player.y - SDL_sin(player.angle);}
            if (sym == SDLK_D){ player.x = player.x - SDL_sin(player.angle); player.y = player.y + SDL_cos(player.angle);}
            if (sym == SDLK_LEFT){ player.angle = player.angle + 0.1;}
            if (sym == SDLK_RIGHT){ player.angle = player.angle - 0.1;}
            if (sym == SDLK_M) {
                view_mode++;
                if (view_mode > 3){ view_mode = 1; }
            }
            break;
        case SDL_EVENT_KEY_UP:

            break;
        default:
            break;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    // static const SDL_FPoint line_points1[] = {
    //     { 310,240 }, { 330, 240 },
    //     { 325,235 }, {330,240 }, {325,245 }
    // };
    static const SDL_FPoint line_points[] = {
        { 240, 450 }, { 400, 450 },{ 400, 356 },
        { 240, 356 }, { 240, 450 }
    };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black */
    SDL_RenderClear(renderer);
    int vx1 = xa;
    int vy1 = ya;
    int vx2 = xb;
    int vy2 = yb;

    double tx1 = vx1 - player.x; double ty1 = vy1 - player.y;
    double tx2 = vx2 - player.x; double ty2 = vy2 - player.y;

    double tz1 = tx1 * SDL_cos(player.angle) + ty1 * SDL_sin(player.angle);
    double tz2 = tx2 * SDL_cos(player.angle) + ty2 * SDL_sin(player.angle);
    tx1 = tx1 * SDL_sin(player.angle) - ty1 * SDL_cos(player.angle);
    tx2 = tx2 * SDL_sin(player.angle) - ty2 * SDL_cos(player.angle);

    if (view_mode == 1) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderLine(renderer, player.x,player.y , SDL_cos(player.angle)*20 + player.x,SDL_sin(player.angle)*20 + player.y);

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderLine(renderer, xa, ya, xb, yb);
        SDL_RenderLine(renderer, xa, ya, xb, ya);
        SDL_RenderLine(renderer, xa, yb, xb, yb);
        SDL_RenderLine(renderer, xa, yb, xa, ya);
        SDL_RenderLine(renderer, xb, yb, xb, ya);
    }
    if (view_mode == 2) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderLine(renderer, MID_X,MID_Y , MID_X,MID_Y-20);
//320 - tx1, 240 - tz1, 320 - tx2, 240 - tz1
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderLine(renderer, 320 - tx1, 240 - tz1, 320 - tx2, 240 - tz2);

        vx1 = xa;
        vy1 = ya;
        vx2 = xb;
        vy2 = ya;

        tx1 = vx1 - player.x; ty1 = vy1 - player.y;
        tx2 = vx2 - player.x; ty2 = vy2 - player.y;

        tz1 = tx1 * SDL_cos(player.angle) + ty1 * SDL_sin(player.angle);
        tz2 = tx2 * SDL_cos(player.angle) + ty2 * SDL_sin(player.angle);
        tx1 = tx1 * SDL_sin(player.angle) - ty1 * SDL_cos(player.angle);
        tx2 = tx2 * SDL_sin(player.angle) - ty2 * SDL_cos(player.angle);
        SDL_RenderLine(renderer, 320 - tx1, 240 - tz1, 320 - tx2, 240 - tz2);

        vx1 = xa;
        vy1 = yb;
        vx2 = xb;
        vy2 = yb;

        tx1 = vx1 - player.x; ty1 = vy1 - player.y;
        tx2 = vx2 - player.x; ty2 = vy2 - player.y;

        tz1 = tx1 * SDL_cos(player.angle) + ty1 * SDL_sin(player.angle);
        tz2 = tx2 * SDL_cos(player.angle) + ty2 * SDL_sin(player.angle);
        tx1 = tx1 * SDL_sin(player.angle) - ty1 * SDL_cos(player.angle);
        tx2 = tx2 * SDL_sin(player.angle) - ty2 * SDL_cos(player.angle);
        SDL_RenderLine(renderer, 320 - tx1, 240 - tz1, 320 - tx2, 240 - tz2);

        vx1 = xa;
        vy1 = yb;
        vx2 = xa;
        vy2 = ya;

        tx1 = vx1 - player.x; ty1 = vy1 - player.y;
        tx2 = vx2 - player.x; ty2 = vy2 - player.y;

        tz1 = tx1 * SDL_cos(player.angle) + ty1 * SDL_sin(player.angle);
        tz2 = tx2 * SDL_cos(player.angle) + ty2 * SDL_sin(player.angle);
        tx1 = tx1 * SDL_sin(player.angle) - ty1 * SDL_cos(player.angle);
        tx2 = tx2 * SDL_sin(player.angle) - ty2 * SDL_cos(player.angle);
        SDL_RenderLine(renderer, 320 - tx1, 240 - tz1, 320 - tx2, 240 - tz2);

        vx1 = xb;
        vy1 = yb;
        vx2 = xb;
        vy2 = ya;

        tx1 = vx1 - player.x; ty1 = vy1 - player.y;
        tx2 = vx2 - player.x; ty2 = vy2 - player.y;

        tz1 = tx1 * SDL_cos(player.angle) + ty1 * SDL_sin(player.angle);
        tz2 = tx2 * SDL_cos(player.angle) + ty2 * SDL_sin(player.angle);
        tx1 = tx1 * SDL_sin(player.angle) - ty1 * SDL_cos(player.angle);
        tx2 = tx2 * SDL_sin(player.angle) - ty2 * SDL_cos(player.angle);
        SDL_RenderLine(renderer, 320 - tx1, 240 - tz1, 320 - tx2, 240 - tz2);

        // SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        // SDL_RenderLines(renderer, line_points, SDL_arraysize(line_points));
}
    printf("%f %f %f %f\n", tx1, tz1, tx2, tz2);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}

