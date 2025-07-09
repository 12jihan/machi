#pragma once
#include <GLFW/glfw3.h>
#include <array>
#include <string>

// TODO: I need to make sure later that some of the memebers here are properly guarderd later
class WindowManager {
 private:
  GLFWwindow *window;
  std::string title;
  int width;
  int height;
  bool isFullscreen;
  bool isInitialized;

  void windowHints();
  void createWindow();
  void makeContextCur(GLFWwindow *window);
  void frameBufferCB(GLFWwindow *window, int width, int height);

 public:
  WindowManager(const std::string &title, int width, int height);
  ~WindowManager();

  // Delete copy constructor and assignment operator
  // WindowManager(const WindowManager &) = delete;
  // WindowManager &operator=(const WindowManager &) = delete;

  // void init();
  std::string getTitle();
  const GLFWwindow *getWindow();
  std::array<int, 2> getWindowSize();
  bool shouldClose() const;
  void pollEvents() const;
  void swapBuffers() const;
};
