#include "../include/WindowManager.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
  WindowManager windowManager("MACHI - OPENGL TEST", 800, 600);

  // Render loop
  while (!windowManager.shouldClose()) {
    std::cout << windowManager.getWindowSize()[0] << std::endl;
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.60f, 1.0f, 0.9f, 1.0f);

    windowManager.swapBuffers();
    windowManager.pollEvents();
  }

  glfwTerminate();
  return 0;
}
