#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <emscripten.h>

#define REC_SQRT2 0.7071067811865475

int x = 0;
enum input_state {
    NOTHING_PRESSED = 0,
    UP_PRESSED = 1,
    DOWN_PRESSED = 1<<1,
    LEFT_PRESSED = 1<<2,
    RIGHT_PRESSED = 1<<3
};

struct context {
    SDL_Renderer *renderer;
    SDL_Rect window;
    SDL_Rect sprite;
    SDL_Texture *owl_tex;
    int active_state;
    int owl_vx;
    int owl_vy;
};

/**
 * Loads the owl texture into the context
 */
int get_owl_texture(struct context * ctx) {
  SDL_Surface *image;

  if (ctx->owl_vx < 0) {
    image = IMG_Load("assets/actorB.png");
  }

  if(ctx->owl_vx > 0){
    image = IMG_Load("assets/actor.png");
  }

  if (!image) {
     printf("IMG_Load: %s\n", IMG_GetError());
     return 0;
  }
  ctx->owl_tex = SDL_CreateTextureFromSurface(ctx->renderer, image);
  ctx->window.w = (image->w)/8;
  ctx->window.h = image->h;
  ctx->sprite.w = (image->w)/8;
  ctx->sprite.h = image->h;

  SDL_FreeSurface (image);
  return 1;
}

/**
 * Processes the input events and sets the velocity
 * of the owl accordingly
 */

void process_input(struct context *ctx){
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                if (event.key.type == SDL_KEYDOWN)
                    ctx->active_state |= UP_PRESSED;
                else if (event.key.type == SDL_KEYUP)
                    ctx->active_state ^= UP_PRESSED;
                break;
            case SDLK_DOWN:
                if (event.key.type == SDL_KEYDOWN)
                    ctx->active_state |= DOWN_PRESSED;
                else if (event.key.type == SDL_KEYUP)
                    ctx->active_state ^= DOWN_PRESSED;
                break;
            case SDLK_LEFT:
                if (event.key.type == SDL_KEYDOWN)
                    ctx->active_state |= LEFT_PRESSED;
                else if (event.key.type == SDL_KEYUP)
                    ctx->active_state ^= LEFT_PRESSED;
                break;
            case SDLK_RIGHT:
                if (event.key.type == SDL_KEYDOWN)
                    ctx->active_state |= RIGHT_PRESSED;
                else if (event.key.type == SDL_KEYUP)
                    ctx->active_state ^= RIGHT_PRESSED;
                break;
            default:
                break;
        }
    }

    ctx->owl_vy = 0;
    ctx->owl_vx = 0;
    if (ctx->active_state & UP_PRESSED)
        ctx->owl_vy = -5;
    if (ctx->active_state & DOWN_PRESSED)
        ctx->owl_vy = 5;
    if (ctx->active_state & LEFT_PRESSED)
        ctx->owl_vx = -5;
    if (ctx->active_state & RIGHT_PRESSED)
        ctx->owl_vx = 5;
    if (!ctx->owl_vx && !ctx->owl_vy) {
      x = 0;
    } else {
      x++;
    }

    if (ctx->owl_vx != 0 && ctx->owl_vy != 0) {
        ctx->owl_vx *= REC_SQRT2;
        ctx->owl_vy *= REC_SQRT2;
    }
}

void loop_handler(void *arg) {
    struct context *ctx = (struct context*)arg;

    int vx = 0;
    int vy = 0;
    process_input(ctx);
    get_owl_texture(ctx);

    ctx->sprite.x = (x % 8) * ctx->sprite.w;
    ctx->window.x += ctx->owl_vx;
    ctx->window.y += ctx->owl_vy;
    printf("%d %d\n", ctx->window.x, ctx->window.y);
    SDL_RenderClear(ctx->renderer);
    SDL_RenderCopy(ctx->renderer, ctx->owl_tex, &ctx->sprite, &ctx->window);
    SDL_RenderPresent(ctx->renderer);
}

int main() {
    SDL_Window *window;
    struct context ctx;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(1500, 400, 0, &window, &ctx.renderer);
    SDL_SetRenderDrawColor(ctx.renderer, 255, 255, 255, 255);

    get_owl_texture(&ctx);
    ctx.active_state = NOTHING_PRESSED;
    ctx.owl_vx = 0;
    ctx.owl_vy = 0;

    /**
     * Schedule the main loop handler to get
     * called on each animation frame
     */
    emscripten_set_main_loop_arg(loop_handler, &ctx, -1, 1);

    return 0;
}
