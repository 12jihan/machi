#include "../include/WindowManager.hpp"

#include <glad/glad.h>
#include <array>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

WindowManager::WindowManager(const std::string& title, int width, int height)
    : window(nullptr),
      title("MACHI - OpenGL Game Engine"),
      width(640),
      height(480),
      isInitialized(false) {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  windowHints();
  window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

  // check if window was created
  if (window == nullptr) {
    std::cout << "Trouble Creating GLFW window" << std::endl;
    throw std::runtime_error("Trouble Creating GLFW window");
  }

  // change the window context
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, frameBufferCB);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    glfwTerminate();
    throw std::runtime_error("Trouble Setting Up Glad");
  }
}

WindowManager::~WindowManager() {
  if (window) {
    glfwDestroyWindow(window);
  }
  glfwTerminate();
}

void WindowManager::frameBufferCB(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  std::cout << "testing" << "w: " << width << "h: " << height << std::endl;
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

bool WindowManager::shouldClose() const {
  return window ? glfwWindowShouldClose(window) : true;
}

void WindowManager::pollEvents() const {
  glfwPollEvents();
}

void WindowManager::swapBuffers() const {
  if (window) {
    glfwSwapBuffers(window);
  }
}

const GLFWwindow* WindowManager::getWindow() const {
  return window;
}

std::array<int, 2> WindowManager::getWindowSize() {
  if (window) {
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    return {w, h};  // Fixed: return actual window size
  }
  return {0, 0};
}
