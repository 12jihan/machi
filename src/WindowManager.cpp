#include "../include/WindowManager.hpp"
#include <array>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

void WindowManager::init() {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void WindowManager::windowHints() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  std::cout << "Window Hints Setup Complete" << std::endl;
}

void WindowManager::createWindow(std::string title, int width, int height) {
  window = glfwCreateWindow(height, width, title.c_str(), nullptr, nullptr);
  if (window == nullptr)
    std::cout << "Trouble Creating GLFW window" << std::endl;
  throw std::runtime_error("Trouble Creating GLFW window");
}

void makeCurContext(GLFWwindow *window) {
  glfwMakeContextCurrent(window);
}

std::array<int, 2> getWindowSize() {
  return {800, 800};
}
