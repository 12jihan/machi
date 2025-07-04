#pragma once
#include <GLFW/glfw3.h>
#include <array>
#include <string>

class WindowManager {
 private:
  GLFWwindow *window;
  std::string title;
  int width;
  int height;
  void createWindow(std::string title, int width, int height);
  void makeContextCur(GLFWwindow *window);
  void windowHints();

 public:
  void init();
  void configure();
  void pollEvents();
  GLFWwindow *getWindow();
  std::string getTitle();
  std::array<int, 2> getWindowSize();
};
