#include "../include/WindowManager.hpp"

#include <glad/glad.h>
#include <array>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

WindowManager::WindowManager(std::string& title, int width, int height)
    : window(nullptr),
      title("MACHI - OpenGL Game Engine"),
      width(640),
      height(480),
      isInitialized(false) {}

WindowManager::~WindowManager() {
  if (window) {
    glfwDestroyWindow(window);
  }
  glfwTerminate();
}

void WindowManager::init() {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    std::exit(EXIT_FAILURE);
  }
  windowHints();
  window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Trouble Creating GLFW window" << std::endl;
    throw std::runtime_error("Trouble Creating GLFW window");
  }
  glfwMakeContextCurrent(window);
  // TODO: add this in when you get to user input...
  //  glfwSetFramebufferSizeCallback(window, frameBufferCB);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    glfwTerminate();
    throw std::runtime_error("Trouble Setting Up Glad");
  }

  glfwMakeContextCurrent(window);
}

void WindowManager::windowHints() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  std::cout << "Window Hints Setup Complete" << std::endl;
  // For macOS compatibility
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

  // Optional: Make window resizable
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
}

void WindowManager::makeContextCur(GLFWwindow* window) {
  glfwMakeContextCurrent(window);
}

void WindowManager::pollEvents() {
  glfwPollEvents();
}

std::array<int, 2> getWindowSize() {
  return {800, 800};
}

const GLFWwindow* WindowManager::getWindow() {
  return window;
}

bool shouldClose() const {
  return window ? glfwWindowShouldClose(window) : true;
}
