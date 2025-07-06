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

  void createWindow();
  void makeContextCur(GLFWwindow *window);
  void windowHints();

 public:
  WindowManager(std::string &title, int width, int height);
  ~WindowManager();

  // Delete copy constructor and assignment operator
  // WindowManager(const WindowManager &) = delete;
  // WindowManager &operator=(const WindowManager &) = delete;

  void init();
  void pollEvents();
  const GLFWwindow *getWindow();
  std::string getTitle();
  std::array<int, 2> getWindowSize();
  bool shouldClose() const;
};
