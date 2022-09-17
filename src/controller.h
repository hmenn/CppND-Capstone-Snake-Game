#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <memory>
#include <mutex>

#include "renderer.h"
#include "snake.h"

class Controller {
 public:
  Controller(std::shared_ptr<std::mutex> mtx,
             std::shared_ptr<Renderer> renderer);
  void HandleInput(bool &running, Snake &snake);

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;

  std::shared_ptr<std::mutex> mtx;
  std::unique_lock<std::mutex> lck;
  bool gameBlocked = false;
  std::shared_ptr<Renderer> renderer;
};

#endif