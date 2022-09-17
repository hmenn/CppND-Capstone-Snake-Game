#include <boost/program_options.hpp>
#include <iostream>

#include "controller.h"
#include "game.h"
#include "renderer.h"

namespace po = boost::program_options;

int main(int argc, char *argv[]) {
  std::size_t framesPerSecond;

  std::size_t screenWidth;
  std::size_t screenHeight;
  float snakeSpeed;

  po::options_description options("Options");
  options.add_options()("help", "produce this help message");
  options.add_options()(
      "fps,f", po::value<std::size_t>(&framesPerSecond)->default_value(60),
      ("Frames per second"));
  options.add_options()(
      "screen-width,w",
      po::value<std::size_t>(&screenWidth)->default_value(640),
      ("Screen Witdh"));
  options.add_options()(
      "screen-height,h",
      po::value<std::size_t>(&screenHeight)->default_value(640),
      ("Screen Witdh"));
  options.add_options()(
      "speed,s", po::value<float>(&snakeSpeed)->default_value(0.1),
      ("Snake Initial Speed [0.1 - 1]  !! 0.3 will be set for other cases"));

  po::variables_map vm;

  try {
    po::store(po::command_line_parser(argc, argv).options(options).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << options << std::endl;
      return EXIT_FAILURE;
    }
  } catch (std::exception &ex) {
    std::cout << "Error parsing command line arguments: " << ex.what()
              << std::endl;
    std::cout << options << std::endl;
    return EXIT_FAILURE;
  }

  std::size_t kMsPerFrame{1000 / framesPerSecond};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  if (snakeSpeed < 0.1 || snakeSpeed > 1.0) {
    snakeSpeed = 0.3;
  }

  auto gameStopperMutex = std::make_shared<std::mutex>();
  auto renderer = std::make_shared<Renderer>(screenWidth, screenHeight,
                                             kGridWidth, kGridHeight);
  Controller controller(gameStopperMutex, renderer);
  Game game(kGridWidth, kGridHeight, snakeSpeed);
  game.Run(controller, renderer, kMsPerFrame, gameStopperMutex);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}