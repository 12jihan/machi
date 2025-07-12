#include "../include/Engine.hpp"
#include "../include/Logger.hpp"
#include <string>
// #include <iostream>

Engine::Engine(const std::string &title, int width, int height)
    : title("MACHI - OPENGL TEST"), width(1024), height(768) {
  Logger::getInstance().enableFileOutput(true);
  LOG_INFO("Engine created successfully");
};

Engine::~Engine() {
  LOG_INFO("Engine destroyed");
}

bool Engine::init() {
  LOG_INFO("Initializing Engine");
  return 1;
}

void Engine::render() {
  if (!false) {
    LOG_ERROR("Cannot run - engine not initialized!");
  }

  LOG_INFO("Starting main game loop...");
  isRunning = true;

  while (isRunning && !windowManager().shouldClose()) {
    // Get delta time from timer
    float deltaTime = 0.016f;  // Temporary fixed timestep (~60 FPS)
    // When you implement Timer: float deltaTime = m_timer->getDeltaTime();

    // Update all game systems
    update(deltaTime);

    // Render everything
    render();

    // Present the frame and handle window events
    windowManager().swapBuffers();
    windowManager().pollEvents();
  }
}

void Engine::shutdown() {
  LOG_INFO("Shutting down...");
};

void Engine::clean() {
  LOG_INFO("Cleaning up subsystems...");
};
