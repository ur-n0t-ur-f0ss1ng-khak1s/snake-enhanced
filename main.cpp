#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
#include <deque>

int main()
{
  int winWid = 1000;
  int winHei = 1000;
  SDL_Init(SDL_INIT_EVERYTHING);
  auto window = SDL_CreateWindow("Snake3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winWid, winHei, 0);
  auto renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_Event e;

  enum Direction
  {
    DOWN,
    LEFT,
    RIGHT,
    UP
  };
  SDL_Rect head {winWid/2,winHei/2,10,10};
  
  //snake body container
  std::deque<SDL_Rect> rq;
  int size = 1;

  //apple container
  std::vector<SDL_Rect> apples;

  // generate apples randomly
  for(int i = 0; i < 100; i++)
  {
    apples.emplace_back(rand()%100*10, rand()%100*10, 10, 10);
  }

  bool running = true;
  int dir = 0;
  while(running)
  {
    //handle input
    while(SDL_PollEvent(&e))
    {
      if(e.type == SDL_QUIT) { running = false; }
      if(e.type == SDL_KEYDOWN) {
        if(e.key.keysym.sym == SDLK_DOWN) { dir = DOWN; }
        if(e.key.keysym.sym == SDLK_UP) { dir = UP; }
        if(e.key.keysym.sym == SDLK_LEFT) { dir = LEFT; }
        if(e.key.keysym.sym == SDLK_RIGHT) { dir = RIGHT; }
      }
    }
    //move snake head
    switch(dir)
    {
      case DOWN:
        head.y += 10; break;
      case UP:
        head.y -= 10; break;
      case LEFT:
        head.x -= 10; break;
      case RIGHT:
        head.x += 10; break;
    }
    
    //collision detection
    //
    std::for_each(apples.begin(), apples.end(), [&](auto& apple){
      if(head.x == apple.x && head.y == apple.y)
      {
        size+=10;
        apple.x = -10;
        apple.y = -10;
      }
    });

    std::for_each(rq.begin(), rq.end(), [&](auto& snake_segment){
      if(head.x == snake_segment.x && head.y == snake_segment.y)
      {
        size = 1;
      }
    });

    //add new head to snake
    rq.push_front(head);

    while(rq.size() > size)
    {
      rq.pop_back();
    }

    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    
    // draw body
    //
    SDL_SetRenderDrawColor(renderer,0,0,255,255);
    std::for_each(rq.begin(), rq.end(), [&](auto& snake_segment) {
      SDL_RenderFillRect(renderer, &snake_segment);
    });

    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    std::for_each(apples.begin(), apples.end(), [&](auto& apple) {
      SDL_RenderFillRect(renderer, &apple);
    });

    SDL_RenderPresent(renderer);
    SDL_Delay(25);
  }

  return 0;
}
