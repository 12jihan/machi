#include "../include/Engine.hpp"
#include <string>
#include <iostream>

Engine::Engine(const std::string &title, int width, int height)
    : title("MACHI - OPENGL TEST"), width(1024), height(768) {
  std::cout << "Created Engine" << std::endl;
};

Engine::~Engine() {
  std::cout << "Created Engine" << std::endl;
}

bool Engine::init() {
  std::cout << "Initializing Engine" << std::endl;
  return 0;
}
