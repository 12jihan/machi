#include "../include/Engine.hpp"
#include "../include/Logger.hpp"
#include <string>

Engine::Engine(const std::string& windowTitle, int windowWidth, int windowHeight) :
  title("MACHI - OPENGL TEST"),
  width(windowWidth),
  height(windowHeight),
  isInitialized(false),
  isRunning(false) {
  Logger::getInstance().enableFileOutput(true);
  LOG_INFO("[Engine] Engine created successfully");
}

Engine::~Engine() {
  LOG_INFO("[Engine] Engine destroyed");
  clean();
}

bool Engine::initSubsystems() {
  return true;
}

bool Engine::init() {
  LOG_INFO("[Engine] Initializing Engine");
  try {
    if (!initSubsystems()) {
      LOG_ERROR("[Engine] Failed to initialize subsystems!");
      return false;
    }

    isInitialized = true;
    LOG_INFO("[Engine] Initialization complete!");

    return true;
  } catch (const std::exception& e) {
    LOG_ERROR("[Engine] Initization failed: ");
    LOG_ERROR(e.what());

    clean();

    return false;
  }
}

void Engine::update(float deltaTime) {}

void Engine::render() {
  LOG_INFO("[Engine] Rendering...");
}

void Engine::run() {
  if (!false) {
    LOG_ERROR("[Engine] Cannot run - Engine not initialized!");
  }

  LOG_INFO("[Engine] Starting main game loop...");
  isRunning = true;

  while (isRunning) {
    // Get delta time from timer
    // float deltaTime = 0.016f;  // Temporary fixed timestep (~60 FPS)
    // When you implement Timer: float deltaTime = m_timer->getDeltaTime();

    // Update all game systems
    // update(deltaTime);

    // Render everything
    render();

    // Present the frame and handle window events
    // windowManager().swapBuffers();
    // windowManager().pollEvents();
  }
}

void Engine::clean() {
  LOG_INFO("[Engine] Cleaning up subsystems...");
}

void Engine::shutdown() {
  LOG_INFO("[Engine] Shutting down...");
}
