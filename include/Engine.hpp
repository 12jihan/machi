#pragma once
#include <string>
#include "WindowManager.hpp"

class Engine {
 private:
  WindowManager windowManager;

  std::string title;
  int width;
  int height;
  bool isInitialized;
  bool isRunning;

  bool initSubsystems();
  void update(float deltaTime);
  void render();
  void clean();

 public:
  Engine(const std::string& title, int width, int height);
  ~Engine();

  // Main engine lifecycle methods
  bool init();
  void run();
  void shutdown();
};
