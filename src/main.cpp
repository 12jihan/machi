#include "../include/Engine.hpp"

#include <exception>
#include <iostream>

int main() {
  try {
    Engine engine("MACHI-NGEN - OPENGL TEST", 800, 600);

    if (!engine.initialize()) {
      std::cerr << "Failed to initialize engine!" << std::endl;
      return -1;
    }
    engine.run();

    return 0;
  } catch (std::exception& e) {
    std::cerr << "Engine error: " << e.what() << std::endl;
    return -1;
  }
}
