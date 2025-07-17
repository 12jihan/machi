#include "../include/Engine.hpp"
#include <exception>
#include <iostream>

int main() {
  try {
    Engine engine("MACHI - OPENGL TEST", 1024, 768);

    if (!engine.initialize()) {
      std::cerr << "Failed to initialize engine!" << std::endl;
      return -1;
    }
    // engine.run();
    engine.shaderTest();
    // engine.rbTest();
  } catch (std::exception& e) {
    std::cerr << "Engine error: " << e.what() << std::endl;
  }
  // WindowManager windowManager("MACHI - OPENGL TEST", 800, 600); // windowManager.init();
  //
  // // Render loop
  // while (!windowManager.shouldClose()) {
  //   glClear(GL_COLOR_BUFFER_BIT);
  //
  //   windowManager.swapBuffers();
  //   windowManager.pollEvents();
  // }
  //
  // glfwTerminate();
  // return 0;
}
