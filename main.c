#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constants.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

unsigned int last_frame_time = 0;
typedef struct {
  float x;
  float y;
  float width;
  float height;
} ball;


int render_window(void){
  renderer = SDL_CreateRenderer(window, -1, 0);

  if (!renderer) {
    fprintf(stderr, "Error creating SDL Renderer.\n");
    return FALSE;
  }
  return TRUE;
}

int initialize_window(void){
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL!\n");
    return FALSE;
  }
  window = SDL_CreateWindow(
    NULL, //Title of the Window 
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    SDL_WINDOW_BORDERLESS
  );
  if (!window) {
    fprintf(stderr, "Error creating SDL Window.\n");
  }
  render_window();
  return TRUE;
}

void process_input(int *game_is_running){
  SDL_Event event;
  SDL_PollEvent(&event);
  switch (event.type){
    case SDL_QUIT:
      *game_is_running = FALSE;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
        *game_is_running = FALSE;
      break;
    default:
      break;
  }
}
void setup(ball *ball){
  ball->x = 20;
  ball->y = 20;
  ball->width = 15;
  ball->height = 15;
}
void update(ball *ball){
  // Logic to keep a fixed timestep
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
  if (time_to_wait > 0 && time_to_wait >= FRAME_TARGET_TIME){
    SDL_Delay(time_to_wait);
  }
  float delta_time = (SDL_GetTicks() - last_frame_time)/1000.f;

  last_frame_time = SDL_GetTicks();
  ball->x += 70 * delta_time;
  ball->y += 50 * delta_time;

}
void render(ball ball){
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_Rect ball_rect = {
    (int)ball.x,
    (int)ball.y,
    (int)ball.width,
    (int)ball.height
  };
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &ball_rect);
  SDL_RenderPresent(renderer);
}
void destroy_window() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
int main() {
  
  int game_is_running = initialize_window();
  ball ball;
  setup(&ball);
  while(game_is_running) {
    process_input(&game_is_running);
    update(&ball);
    render(ball);
  }
  destroy_window();
  return TRUE;
}
