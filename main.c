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

int keypress[6] = {0,0,0,0,0,0};

//rectangle coordinates
int xa = 240; int xb = 400;
int ya = 450; int yb = 356;

double FNCross(double x1, double y1, double x2, double y2) {
    return x1 * y2 - y1 * x2;
}

void Intersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double *x, double *y) {
    double a = FNCross(x1, y1, x2, y2);
    double b = FNCross(x3, y3, x4, y4);
    double det = FNCross(x1-x2,y1-y2,x3-x4,y3-y4);
    *x = FNCross(a,x1-x2,b,x3-x4) / det;
    *y = FNCross(a,y1-y2,b,y3-y4) / det;
}

void TransformLineSegmentTP(int x1, int y1, int x2, int y2) {
    int vx1 = x1;
    int vy1 = y1;
    int vx2 = x2;
    int vy2 = y2;

    double tx1 = vx1 - player.x; double ty1 = vy1 - player.y;
    double tx2 = vx2 - player.x; double ty2 = vy2 - player.y;

    double tz1 = tx1 * SDL_cos(player.angle) + ty1 * SDL_sin(player.angle);
    double tz2 = tx2 * SDL_cos(player.angle) + ty2 * SDL_sin(player.angle);
    tx1 = tx1 * SDL_sin(player.angle) - ty1 * SDL_cos(player.angle);
    tx2 = tx2 * SDL_sin(player.angle) - ty2 * SDL_cos(player.angle);

    SDL_RenderLine(renderer, MID_X - tx1, MID_Y - tz1, MID_X - tx2, MID_Y - tz2);
}

void TransformLineSegmentFP(int xa, int ya, int xb, int yb) {
    double tx1 = xa - player.x; double ty1 = ya - player.y;
    double tx2 = xb - player.x; double ty2 = yb - player.y;

    double tz1 = tx1 * SDL_cos(player.angle) + ty1 * SDL_sin(player.angle);
    double tz2 = tx2 * SDL_cos(player.angle) + ty2 * SDL_sin(player.angle);
    tx1 = tx1 * SDL_sin(player.angle) - ty1 * SDL_cos(player.angle);
    tx2 = tx2 * SDL_sin(player.angle) - ty2 * SDL_cos(player.angle);

    double ix1, iz1, ix2, iz2;
    if (tz1 > 0 || tz2 > 0) {
        Intersect(tx1,tz1, tx2,tz2, -0.0001,0.0001, -150, 7.0, &ix1, &iz1);
        Intersect(tx1, tz1, tx2, tz2,  0.0001f, 0.0001f,  150.0f, 7.0f, &ix2, &iz2);
        if (tz1 <= 0) {if (iz1 > 0) {tx1=ix1; tz1=iz1;}
            else {tx1=ix2; tz1=iz2;}
        }
        if (tz2 <= 0) {if (iz1 > 0) {tx2=ix1; tz2=iz1;}
            else {tx2=ix2; tz2=iz2;}
        }
        double x1 = -tx1 * 16 / tz1; double y1a = -320 / tz1; double y1b = 240 / tz1;
        double x2 = -tx2 * 16 / tz2; double y2a = -320 / tz2; double y2b = 240 / tz2;

        SDL_RenderLine(renderer, MID_X + x1, MID_Y + y1a, MID_X + x2, MID_Y + y2a);
        SDL_RenderLine(renderer, MID_X + x1, MID_Y + y1b, MID_X + x2, MID_Y + y2b);
        SDL_RenderLine(renderer, MID_X + x1, MID_Y + y1a, MID_X + x1, MID_Y + y1b);
    }
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    SDL_SetAppMetadata("Example Renderer Lines", "1.0", "com.example.renderer-lines");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Nick's DOOM Engine", WIDTH, HEIGHT, 0, &window, &renderer)) {
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
            if (sym == SDLK_W){keypress[0] = 1;}
            if (sym == SDLK_A){keypress[1] = 1;}
            if (sym == SDLK_S){keypress[2] = 1;}
            if (sym == SDLK_D){keypress[3] = 1;}
            if (sym == SDLK_LEFT){keypress[4] = 1;}
            if (sym == SDLK_RIGHT){keypress[5] = 1;}
            if (sym == SDLK_M) {
                view_mode++;
                if (view_mode > 3){ view_mode = 1; }
            }
            break;
        case SDL_EVENT_KEY_UP:
            sym = event->key.key;
            if (sym == SDLK_W){keypress[0] = 0;}
            if (sym == SDLK_A){keypress[1] = 0;}
            if (sym == SDLK_S){keypress[2] = 0;}
            if (sym == SDLK_D){keypress[3] = 0;}
            if (sym == SDLK_LEFT){keypress[4] = 0;}
            if (sym == SDLK_RIGHT){keypress[5] = 0;}
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

    if (keypress[0]){ player.x = player.x + SDL_cos(player.angle); player.y = player.y + SDL_sin(player.angle);}
    if (keypress[1]){ player.x = player.x + SDL_sin(player.angle); player.y = player.y - SDL_cos(player.angle);}
    if (keypress[2]){ player.x = player.x - SDL_cos(player.angle); player.y = player.y - SDL_sin(player.angle);}
    if (keypress[3]){ player.x = player.x - SDL_sin(player.angle); player.y = player.y + SDL_cos(player.angle);}
    if (keypress[4]){ player.angle = player.angle - 0.1;}
    if (keypress[5]){ player.angle = player.angle + 0.1;}
    SDL_Delay(20);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black */
    SDL_RenderClear(renderer);

    if (view_mode == 1) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderLine(renderer, player.x,player.y , SDL_cos(player.angle)*20 + player.x,SDL_sin(player.angle)*20 + player.y);

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderLine(renderer, xa, ya, xb, ya);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderLine(renderer, xb, yb, xa, yb);
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderLine(renderer, xa, yb, xa, ya);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderLine(renderer, xb, ya, xb, yb);
    }
    if (view_mode == 2) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderLine(renderer, MID_X,MID_Y , MID_X,MID_Y-20);

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        TransformLineSegmentTP(xa,ya,xb,ya);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        TransformLineSegmentTP(xb,yb,xa,yb);
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
        TransformLineSegmentTP(xa,yb,xa,ya);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        TransformLineSegmentTP(xb,ya,xb,yb);
    }
    if (view_mode == 3) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        TransformLineSegmentFP(xa,ya,xb,ya);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        TransformLineSegmentFP(xb,yb,xa,yb);
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
        TransformLineSegmentFP(xa,yb,xa,ya);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        TransformLineSegmentFP(xb,ya,xb,yb);
    }
    //printf("%f %f %f %f\n", tx1, tz1, tx2, tz2);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}

