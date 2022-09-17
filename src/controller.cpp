#include "controller.h"

#include <iostream>

#include "SDL.h"
#include "snake.h"

Controller::Controller(std::shared_ptr<std::mutex> mtx,
                       std::shared_ptr<Renderer> renderer)
    : mtx(mtx), renderer(renderer) {
  lck = std::unique_lock<std::mutex>(*this->mtx);
  lck.unlock();
}

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  return;
}

void Controller::HandleInput(bool &running, Snake &snake) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      std::cerr << "Quit event" << std::endl;
      running = false;
      if (gameBlocked) {
        gameBlocked = false;
        lck.unlock();
      }
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(snake, Snake::Direction::kUp,
                          Snake::Direction::kDown);
          break;

        case SDLK_DOWN:
          ChangeDirection(snake, Snake::Direction::kDown,
                          Snake::Direction::kUp);
          break;

        case SDLK_LEFT:
          ChangeDirection(snake, Snake::Direction::kLeft,
                          Snake::Direction::kRight);
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake, Snake::Direction::kRight,
                          Snake::Direction::kLeft);
          break;
        case SDLK_SPACE:
          if (!gameBlocked) {
            lck.lock();
            renderer->UpdateWindowTitle("Game Paused...");
            gameBlocked = true;
          } else {
            lck.unlock();
            gameBlocked = false;
            renderer->UpdateWindowTitle("Resume game ...");
          }
          break;
      }
    }
  }
}