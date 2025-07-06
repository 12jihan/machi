#include "../include/WindowManager.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
  WindowManager windowManager("MACHI - OPENGL TEST", 800, 600);
  windowManager.init();

  // Render loop
  while (windowManager.shouldClose()) {
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers();
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
