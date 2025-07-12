#pragma once
#include <string>
#include "WindowManager.hpp"

class Engine {
 private:
  std::string title;
  int width;
  int height;
  bool isInitialized;
  bool isRunning;

  WindowManager windowManager();
  bool initSubsystems();
  void clean();

 public:
  Engine(const std::string& title, int width, int height);
  ~Engine();

  // Main engine lifecycle methods
  bool init();
  void run();
  void render();
  void update(float deltaTime);
  void shutdown();
};
