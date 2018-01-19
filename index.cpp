#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <emscripten.h>
#include <vector>
using namespace std;

#define REC_SQRT2 0.7071067811865475

int x = 0;

enum input_state {
    NOTHING_PRESSED = 0,
    UP_PRESSED = 1,
    DOWN_PRESSED = 1<<1,
    LEFT_PRESSED = 1<<2,
    RIGHT_PRESSED = 1<<3
};

class Player {
  public:
    SDL_Rect container;
    SDL_Rect sprite;
    SDL_Texture *text;
    int direction; // -1 left 0 not moving 1 right
    int active_state;
    int vx;
    int vy;
    int moving;
};

struct context {
  SDL_Window *window;
  SDL_Renderer *renderer;
  vector <Player*> players;
};

int get_texture(Player *player, SDL_Renderer *renderer) {
  SDL_Surface *image;
  if (player->direction < 0) {
    image = IMG_Load("assets/actorB.png");
  } else {
    image = IMG_Load("assets/actor.png");
  };

  if (!image) {
     printf("IMG_Load: %s\n", IMG_GetError());
     return 0;
  }

  player->text = SDL_CreateTextureFromSurface(renderer, image);
  player->container.w = (image->w)/8;
  player->container.h = image->h;
  player->sprite.w = (image->w)/8;
  player->sprite.h = image->h;

  SDL_FreeSurface (image);
  return 1;
}

void process_input(Player *player){
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                if (event.key.type == SDL_KEYDOWN)
                    player->active_state |= UP_PRESSED;
                else if (event.key.type == SDL_KEYUP)
                    player->active_state ^= UP_PRESSED;
                break;
            case SDLK_DOWN:
                if (event.key.type == SDL_KEYDOWN)
                    player->active_state |= DOWN_PRESSED;
                else if (event.key.type == SDL_KEYUP)
                    player->active_state ^= DOWN_PRESSED;
                break;
            case SDLK_LEFT:
                if (event.key.type == SDL_KEYDOWN){
                    player->direction = -1;
                    player->active_state |= LEFT_PRESSED;
                }else if (event.key.type == SDL_KEYUP)
                    player->active_state ^= LEFT_PRESSED;
                break;
            case SDLK_RIGHT:
                if (event.key.type == SDL_KEYDOWN){
                    player->direction = 1;
                    player->active_state |= RIGHT_PRESSED;
                }else if (event.key.type == SDL_KEYUP)
                    player->active_state ^= RIGHT_PRESSED;
                break;
            default:
                break;
        }
    }

    player->vy = 0;
    player->vx = 0;
    if (player->active_state & UP_PRESSED)
        player->vy = -5;
    if (player->active_state & DOWN_PRESSED)
        player->vy = 5;
    if (player->active_state & LEFT_PRESSED)
        player->vx = -5;
    if (player->active_state & RIGHT_PRESSED)
        player->vx = 5;
    if (player->vx != 0 && player->vy != 0) {
        player->vx *= REC_SQRT2;
        player->vy *= REC_SQRT2;
    }
    if (player->vx < 0 || player->vy < 0) {
      player->moving--;
    } else if(player->vx>0 || player->vy > 0){
      player->moving++;
    } else {
      player->moving = 0;
    }
}

void loop_handler(void *arg) {
    struct context *ctx = (struct context*)arg;
    vector <Player*> pl = ctx->players;
    SDL_RenderClear(ctx->renderer);
    for (int i = 0; i < pl.size(); i++) {
      get_texture(pl[i], ctx->renderer);
      process_input(pl[i]);
      pl[i]->sprite.x = ((pl[i]->moving / 2) % 8) * pl[i]->sprite.w;
      pl[i]->container.x += (pl[i]->vx);
      pl[i]->container.y += pl[i]->vy;
      SDL_RenderCopy(ctx->renderer, pl[i]->text, &(pl[i]->sprite), &(pl[i]->container));
    }
    SDL_RenderPresent(ctx->renderer);
}

int main() {
    struct context ctx;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(800, 600, 0, &ctx.window, &ctx.renderer);
    SDL_SetRenderDrawColor(ctx.renderer, 100, 100, 255, 255);

    Player *p1 = new Player();
    Player *p2 = new Player();
    Player *p3 = new Player();
    Player *p4 = new Player();

    p1->container.x = 10;
    p2->container.x = 50;
    p3->container.x = 80;
    p4->container.x = 110;

    ctx.players.push_back(p1);
    ctx.players.push_back(p2);
    ctx.players.push_back(p3);
    ctx.players.push_back(p4);

    emscripten_set_main_loop_arg(loop_handler, &ctx, -1, 1);
    return 0;
}
