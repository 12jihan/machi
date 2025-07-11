#pragma once
#include <string>
#include "WindowManager.hpp"

class Engine {
 private:
  std::string title;
  int width;
  int height;
  WindowManager windowManager();

 public:
  Engine(const std::string& title, int width, int height);
  ~Engine();

  // Main engine lifecycle methods (from diagram)
  bool init();
  void run();
  void update(float deltaTime);
  void render();
  void shutdown();
};
