#pragma once
#include <chrono>
#include <memory>
#include <string>
#include "WindowManager.hpp"

// Forward declarations for systems we'll integrate later
class Timer;
class Input;
class Renderer;

// Engine configuration structure - this lets users customize the engine behavior
struct EngineConfig {
  // Window settings
  std::string windowTitle = "Machi Engine";
  int windowWidth = 1024;
  int windowHeight = 768;
  bool fullscreen = false;
  bool vsync = true;

  // Engine settings
  bool enableLogging = true;
  std::string logFile = "engine.log";
  float targetFPS = 60.0f;
  bool showFPSInTitle = true;

  // Rendering settings
  float clearR = 0.2f, clearG = 0.3f, clearB = 0.3f, clearA = 1.0f;  // Background color
  bool enableDepthTest = true;
  bool enableBlending = false;
  int msaaSamples = 4;
};

// Event system - allows different parts of the engine to communicate
enum class EngineEventType {
  WindowResize,
  WindowClose,
  KeyPress,
  KeyRelease,
  MousePress,
  MouseRelease,
  MouseMove,
  EngineShutdown
};

struct EngineEvent {
  EngineEventType type;
  // Union to hold different event data efficiently
  union {
    struct {
      int width, height;
    } resize;
    struct {
      int key, scancode, mods;
    } keyboard;
    struct {
      int button, mods;
    } mouse;
    struct {
      double x, y;
    } mousePos;
  } data;

  double timestamp;  // When the event occurred
};

// Event handler function type - systems can register these to respond to events
using EventHandler = std::function<void(const EngineEvent&)>;

class Engine {
private:
  // Core engine state
  EngineConfig m_config;
  bool m_isInitialized;
  bool m_isRunning;
  bool m_isPaused;

  // Window management - connection to WindowManager
  std::unique_ptr<WindowManager> m_windowManager;

  // Timing sustem for smooth frame rates and delta time calculation
  std::chrono::high_resolution_clock::time_point m_lastFrameTime;
  std::chrono::high_resolution_clock::time_point m_engineStartTime;
  float m_deltaTime;
  float m_totalTime;
  int m_frameCount;
  float m_fps;
  float m_fpsUpdateTimer;

  // Event system for communication between engine components
  std::vector<EventHandler> m_eventHandlers;
  std::vector<EngineEvent> m_eventQueue;

  // TODO: Create SceneManager
  // Scene management - this will hold your game's current state
  // std::unique_ptr<Scene> m_currentScene;
  // std::unique_ptr<Scene> m_nextScene;  // For scene transitions

  // Engine subsystem initialization methods
  bool initializeWindowSystem();
  bool initializeRenderingSystem();
  bool initializeInputSystem();
  bool initializeAudioSystem();  // For future expansion

  void processEvents();
  void updateSystems(float deltaTime);
  void renderFrame();
  void calculateFrameStats();
  void updateWindowTitle();

  // Window event callbacks - these bridge WindowManager events to our event system
  void onWindowResize(int width, int height);
  void onKeyEvent(int key, int scancode, int actions, int mods);
  void onMouseButton(int button, int action, int mods);
  void onMouseMove(double x, double y);
  void onScroll(double xOffset, double yOffset);

  // Event system methods
  void dispatchEvent(const EngineEvent& event);
  void processEventQueue();

  // Scene management helpers
  void performSceneTransition();

  // Cleanup methods
  void shutdownSystems();

public:
  // Constructor with flexible configuration options
  explicit Engine(const EngineConfig& config = {});
  Engine(const std::string& title, int width = 1024, int height = 768);

  // Destructor ensures proper cleanup
  ~Engine();

  // Delete copy operations to prevent accidental copying of the engine
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

  // Move operations are allowed (useful for factory patterns)
  Engine(Engine&&) noexcept = default;
  Engine& operator=(Engine&&) noexcept = default;

  // Core engine lifecycle - the main interface for using the engine
  bool initialize();
  void run();
  void shutdown();
  void pause();
  void resume();

  // Engine state queries - useful for debugging and external systems
  bool isInitialized() const {
    return m_isInitialized;
  }
  bool isRunning() const {
    return m_isRunning;
  }
  bool isPaused() const {
    return m_isPaused;
  }
  float getDeltaTime() const {
    return m_deltaTime;
  }
  float getTotalTime() const {
    return m_totalTime;
  }
  float getFPS() const {
    return m_fps;
  }
  int getFrameCount() const {
    return m_frameCount;
  }

  // Window interface - provides controlled access to window functionality
  WindowManager& getWindowManager() {
    return *m_windowManager;
  }
  const WindowManager& getWindowManager() const {
    return *m_windowManager;
  }

  std::array<int, 2> getWindowSize() const;
  void setWindowSize(int width, int height);
  void setWindowTitle(const std::string&);
  void toggleFullscreen();
  bool isFullscreen() const;

  // TODO: Create SceneManager
  // Scene management - this is how you'll organize your game content
  // void setScene(std::unique_ptr<Scene> scene);
  // void transition(std::unique_ptr<Scene> scene);
  // Scene* getCurrentScene() const { return m)_currentScene.get();};

  // Event system interface - allows other systems to respond to engine events
  void addEventListener(const EventHandler& handler);
  void removeEventListener(const EventHandler& handler);
  void postEvent(const EventHandler& handler);

  // Configuration access - allows runtime modification of engine behavior
  const EngineConfig& getConfig() const {
    return m_config;
  }
  void updateConfig(const EngineConfig& newConfig);

  // Utility methods for common engine operations
  void requestShutdown() {
    m_isRunning = false;
  }
  void setClearColor(float r, float g, float b, float a);
  void enableDepthTest(bool enable);
  void enableBlending(bool enable);
  void enableVSync(bool enable);

  // Debug and profiling helpers
  void printSystemInfo() const;
  void printFrameStats() const;

  // Static utility methods
  static std::string getEngineVersion() {
    return "1.0.0";
  }
  static std::string getBuildInfo();  // Implementation will include compile time, OpengGL version, etc.
};
