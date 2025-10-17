#include "../include/Engine.hpp"
#include "../include/Logger.hpp"
#include "../include/Shader.hpp"
#include "../include/Utils.hpp"

#include <GLFW/glfw3.h>
#include <chrono>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>

Engine::Engine(const EngineConfig& config) :
 m_config(config),
 m_isInitialized(false),
 m_isRunning(false),
 m_isPaused(false),
 m_deltaTime(0.0f),
 m_totalTime(0.0f),
 m_frameCount(0.0f),
 m_fps(0.0f),
 m_fpsUpdateTimer(0.0f),
 m_windowManager(nullptr)
// m_currentScene(nullptr),
// m_nextScene(nullptr)
{
  if (m_config.enableLogging) {
    Logger::getInstance().enableFileOutput(true, m_config.logFile);
    Logger::getInstance().setLogLevel(LogLevel::INFO);
  }

  LOG_INFO_F("[Engine] Engine created with title: '{}', size: {}x{}",
             m_config.windowTitle,
             m_config.windowWidth,
             m_config.windowHeight);
}

Engine::Engine(const std::string& title, int width, int height) : Engine(EngineConfig{title, width, height}) {
  // This delegates to the main constructor with a default config.
  LOG_INFO_F("[Engine] Engine created with title: '{}', size: {}x{}",
             m_config.windowTitle,
             m_config.windowWidth,
             m_config.windowHeight);
}

Engine::~Engine() {
  LOG_INFO("[Engine] Engine shuttingdown");
  shutdown();
}

bool Engine::initialize() {
  if (m_isInitialized) {
    LOG_INFO("[Engine] Engine already intialized!");
    return true;
  }

  LOG_INFO("[Engine] Beginning engine initialization...");
  m_engineStartTime = std::chrono::high_resolution_clock::now();

  try {
    // Initialize all engine subsystems in the correct order
    // Window system must come first as other systems depend on OpengGL context
    if (!initializeWindowSystem()) {
      LOG_ERROR("[Engine] Failed to initialize window system!");
      return false;
    }
    // @TODO: Create Render class
    // if (!initializeRenderingSystem()) {
    //   LOG_ERROR("[Engine] Failed to initialize rendering system!");
    //   return false;
    // }
    if (!initializeInputSystem()) {
      LOG_ERROR("[Engine] Failed to initialize input system!");
      return false;
    }
    // Audio system is optional for now
    // initializeAudioSystem();

    // Set up intial timing
    m_lastFrameTime = std::chrono::high_resolution_clock::now();

    m_isInitialized = true;
    LOG_INFO("[Engine] Engine intialization completed sucessfully!");

    // Print useful information about the system
    printSystemInfo();

    return true;

  } catch (const std::exception& e) {
    LOG_ERROR_F("[Engine] Initization failed with exception: {}", e.what());
    shutdownSystems();
    return false;
  }
}

bool Engine::initializeWindowSystem() {
  LOG_INFO("[Engine] Initializing window system");
  // Create window configuration from our engine config
  WindowConfig windowConfig;
  windowConfig.title = m_config.windowTitle;
  windowConfig.width = m_config.windowWidth;
  windowConfig.height = m_config.windowHeight;
  windowConfig.fullscreen = m_config.fullscreen;
  windowConfig.vsync = m_config.vsync;
  windowConfig.samples = m_config.msaaSamples;

  m_windowManager = std::make_unique<WindowManager>(windowConfig);

  if (!m_windowManager->initialize()) {
    LOG_ERROR("[Engine] Failed to initialize WindowManager");
    return false;
  }

  // Setup all the window event callbacks to bridge into our event system
  m_windowManager->setResizeCallback([this](int width, int height) -> void { onWindowResize(width, height); });

  m_windowManager->setKeyCallback(
    [this](int key, int scancode, int actions, int mods) -> void { onKeyEvent(key, scancode, actions, mods); });

  // m_windowManager->setMouseButtonCallback(
  // [this](int button, int action, int mods) -> void { onMouseButton(button, action, mods); });

  m_windowManager->setMouseMoveCallback([this](double x, double y) -> void { onMouseMove(x, y); });

  m_windowManager->setScrollCallback([this](double xOffset, double yOffset) { onScroll(xOffset, yOffset); });

  LOG_INFO("[Engine] Window system intialized sucessfully");
  return true;
};

// TODO: extend the renderer class
bool Engine::initializeRenderingSystem() {
  LOG_INFO("[Engine] Initializing rendering system...");
  // Set up basic OpenGL state based on our configuration
  setClearColor(m_config.clearR, m_config.clearG, m_config.clearB, m_config.clearA);
  enableDepthTest(m_config.enableDepthTest);
  enableBlending(m_config.enableBlending);

  // Set initial viewport to match window size
  auto [width, height] = m_windowManager->getSize();
  glViewport(0, 0, width, height);

  LOG_INFO("[Engine] Rendering system initialized successfully");
  return true;
}

bool Engine::initializeInputSystem() {
  LOG_INFO("[Engine] Initializing input system...");
  addEventListener([this](const EngineEvent& event) {
    if (event.type == EngineEventType::KeyPress) {
      // Handle common engine shortcuts
      switch (event.data.keyboard.key) {
        case GLFW_KEY_ESCAPE:
          LOG_INFO("[Engine] Escape pressed - requesting shutdown");
          shutdown();
          break;
        case GLFW_KEY_F2:
          LOG_INFO("[Engine] F2 pressed - toggling fullscreen");
          m_windowManager->toggleFullscreen();
          break;
        case GLFW_KEY_F1:
          // F1 for debug info
          printFrameStats();
          break;
      }
    }

    if (event.type == EngineEventType::WindowClose) {
      LOG_INFO("[Engine] Window close requested");
      requestShutdown();
    }
  });

  LOG_INFO("[Engine] Input system intialized successfully");
  return true;
}

// bool Engine::intializeAudioSystem() {
//   LOG_INFO("[Engine] Audio system intialization skipped (not implemented yet)");
//   return true;
// }

void Engine::run() {
  if (!m_isInitialized) {
    LOG_INFO("[Engine] Cannot run - engine not intialized!");
    return;
  }

  // INFO: --> Shader test starts here
  Shader shader("../shaders/main.vert.glsl", "../shaders/main.frag.glsl");
  // std::string vShaderSource = Utils::loadFile("../shaders/main.vert.glsl");
  // const char* vShaderSrc = vShaderSource.c_str();
  //
  // std::string fShaderSource = Utils::loadFile("../shaders/main.frag.glsl");
  // const char* fShaderSrc = fShaderSource.c_str();

  // int success;
  // char infoLog[512];

  // // vshader
  // unsigned vShader;
  // vShader = glCreateShader(GL_VERTEX_SHADER);
  // glShaderSource(vShader, 1, &vShaderSrc, nullptr);
  // glCompileShader(vShader);
  // glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
  // if (!success) {
  //   glGetShaderInfoLog(vShader, 512, nullptr, infoLog);
  //   LOG_ERROR_F("[Engine]::[Shader] there was an error with the vShader: {}", infoLog);
  // };
  //
  // // fshader
  // unsigned fShader;
  // fShader = glCreateShader(GL_FRAGMENT_SHADER);
  // glShaderSource(fShader, 1, &fShaderSrc, nullptr);
  // glCompileShader(fShader);
  // glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
  // if (!success) {
  //   glGetShaderInfoLog(fShader, 512, nullptr, infoLog);
  //   LOG_ERROR_F("[Engine]::[Shader] there was an error with the fShader: {}", infoLog);
  // };
  //
  // // Shader program
  // unsigned int shaderProgram;
  // shaderProgram = glCreateProgram();
  // glAttachShader(shaderProgram, vShader);
  // glAttachShader(shaderProgram, fShader);
  // glLinkProgram(shaderProgram);
  // glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  // if (!success) {
  //   glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
  //   LOG_ERROR_F("[Engine]::[Shader] there was an error with the shader program: {}", infoLog);
  // }
  //
  // // Delete the shaders
  // glDeleteShader(vShader);
  // glDeleteShader(fShader);

  // VAOs, VBOs, EBOs
  // clang-format off
  // float vertices[] = {
  //   -0.5f, -0.5f, 0.0f,
  //   0.5f, -0.5f, 0.0f,
  //   0.0f, 0.5f, 0.0f
  // };
   
    float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
    };

  // clang-format on
  unsigned int vao, vbo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // INFO: --> Shader test ends here

  LOG_INFO("[ENGINE] starting main engine loop...");
  m_isRunning = true;
  m_lastFrameTime = std::chrono::high_resolution_clock::now();

  // Main engine, loop - this is the heart of your game engine
  LOG_INFO_F("[Engine]::[Shader] m_isPaused {}", m_isPaused);
  if (false) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  // Check how many attributes are available
  int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  LOG_INFO_F("[Engine]::[Shader] Max vertex attributes supported: {}", nrAttributes);

  while (m_isRunning && !m_windowManager->shouldClose()) {
    // Caculate time since last frame for smooth, frame-rate independent updates
    auto currentTime = std::chrono::high_resolution_clock::now();
    m_deltaTime = std::chrono::duration<float>(currentTime - m_lastFrameTime).count();
    m_lastFrameTime = currentTime;

    // Update total time since engine started
    m_totalTime = std::chrono::duration<float>(currentTime - m_engineStartTime).count();

    // Process all pending window events (keyboard, mouse, window operations)
    processEvents();

    // Only update and render if we're not paused
    // if (!m_isPaused) {
    // glUseProgram(shaderProgram);
    shader.use();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    // Update all game systems with the calculated delta time
    // updateSystems(m_deltaTime);

    // Render the current frame
    // renderFrame();

    m_windowManager->swapBuffers();

    // Update frame statistics for performance monitoring
    calculateFrameStats();
    // }
    // TODO: Create Scene class
    // Handle scene transitions if needed
    // performSceneTransition();
  }

  LOG_INFO("[Engine] Main engine loop ended");
}

void Engine::processEvents() {
  // Poll for new window events from GLFW
  m_windowManager->pollEvents();

  // Process our internal event queue
  processEventQueue();
}

void Engine::updateSystems(float deltaTime) {
  // @TODO: implement Scene class
  // Update the current scene if we have one
  // if (m_currentScene) {
  //   m_currentScene->update(deltaTime);
  // }

  // Here you would update other engine system like:
  // - Physics system
  // - Audio system
  // - Animation system
  // - Particle systems
  // - UI system
};

void Engine::renderFrame() {
  LOG_INFO("[Engine] running renderer");
  // Clear the screen with our configured background color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TODO: Implement Scene class
  // Render the current scene if we have one
  // if (m_currentScene) {
  //   m_currentScene->render();
  // }

  // Preset the rendered frame to the screen
  m_windowManager->swapBuffers();
}

void Engine::calculateFrameStats() {
  m_frameCount++;
  m_fpsUpdateTimer += m_deltaTime;

  // Update FPS display every half second
  if (m_fpsUpdateTimer >= 0.5f) {
    m_fps = m_frameCount / m_fpsUpdateTimer;
    m_frameCount = 0;
    m_fpsUpdateTimer = 0.0f;

    // Update window title with FPS if requested
    if (m_config.showFPSInTitle) {
      updateWindowTitle();
    }
  }
}

void Engine::updateWindowTitle() {
  std::ostringstream title;
  title << m_config.windowTitle << " - FPS: " << std::fixed << std::setprecision(1) << m_fps;
  m_windowManager->setTitle(title.str());
}

// Window event handlers - these translate WindowManager callbacks into our event system
void Engine::onWindowResize(int width, int height) {
  // Update our configuration
  m_config.windowWidth = width;
  m_config.windowHeight = height;

  // Update OpenGL viewport
  glViewport(0, 0, width, height);

  // Create and dispatch resize event
  EngineEvent event;
  event.type = EngineEventType::WindowResize;
  event.data.resize = {width, height};
  event.timestamp = m_totalTime;

  dispatchEvent(event);

  LOG_INFO_F("[Event] Window resized to {}x{}", width, height);
}

void Engine::onKeyEvent(int key, int scancode, int action, int mods) {
  EngineEvent event;
  event.type =
    (action == GLFW_PRESS || action == GLFW_REPEAT) ? EngineEventType::KeyPress : EngineEventType::KeyRelease;
  event.data.keyboard = {key, scancode, mods};
  event.timestamp = m_totalTime;

  dispatchEvent(event);
}

void Engine::onMouseMove(double x, double y) {
  EngineEvent event;
  event.type = EngineEventType::MouseMove;
  event.data.mousePos = {x, y};
  event.timestamp = m_totalTime;

  dispatchEvent(event);
}

void Engine::onScroll(double xOffset, double yOffset) {
  // Could be extended to log scrolls
  // EngineEvent event;
  // event.type = EngineEventType::Scroll;
  // event.data.mousePos = {x, y};
  // event.timestamp = m_totalTime;

  // dispatchEvent(event);

  LOG_DEBUG_F("[Engine] Mouse scroll: ({}, {})", xOffset, yOffset);
}

void Engine::dispatchEvent(const EngineEvent& event) {
  // Add event to our queue for processing
  m_eventQueue.push_back(event);
}

void Engine::processEventQueue() {
  // Process all event in the queue
  for (const auto& event : m_eventQueue) {
    // Send event to all registered handlers
    for (const auto& handler : m_eventHandlers) {
      handler(event);
    }
  }
}

// TODO: Implement Scene class
// void Engine::performSceneTransition() {
//   if (m_nextScene) {
//     // Clean up currentScene
//     if (m_currentScene) {
//       m_currentScene.reset();
//     }
//
//     // Switch to new scene
//     mov_currentScene = std::move(m_nextScene);
//     m_nextScene = nullptr;
//
//     LOG_INFO("[Engine] Scene transition completed");
//   }
// }

// TODO: Implement Scene class
// Public interface methods
// Engine::setScene(std::unique_ptr<Scene> scene) {
//   m_currentScene = std::move(scene);
//   Log_INFO("[Engine] Scene set directly");
// }

// TODO: Implement Scene class
// void Engine::transitionToScene(std::unique_ptr<Scene> scene) {
//   m_nextScene = std::move(scene);
//   LOG_INFO("[Engine] Scene transition queue");
// }

void Engine::addEventListener(const EventHandler& handler) {
  m_eventHandlers.push_back(handler);
}

std::array<int, 2> Engine::getWindowSize() const {
  return m_windowManager ? m_windowManager->getSize() : std::array<int, 2>{0, 0};
}

void Engine::setWindowSize(int width, int height) {
  if (m_windowManager) {
    m_windowManager->setSize(width, height);
    m_config.windowWidth = width;
    m_config.windowHeight = height;
  }
}

void Engine::setWindowTitle(const std::string& title) {
  m_config.windowTitle = title;
  if (m_windowManager) {
    m_windowManager->setTitle(title);
  }
}

// TODO: Fix fullscreen
void Engine::toggleFullscreen() {
  if (m_windowManager) {
    m_windowManager->toggleFullscreen();
    m_config.fullscreen = m_windowManager->isFullscreen();
  }
}

bool Engine::isFullscreen() const {
  return m_windowManager ? m_windowManager->isFullscreen() : false;
}

void Engine::setClearColor(float r, float g, float b, float a) {
  m_config.clearR = r;
  m_config.clearG = g;
  m_config.clearB = b;
  m_config.clearA = a;
  glClearColor(r, g, b, a);
}

void Engine::enableDepthTest(bool enable) {
  m_config.enableDepthTest = enable;
  if (enable) {
    glEnable(GL_DEPTH_TEST);
  } else {
    glDisable(GL_DEPTH_TEST);
  };
}

void Engine::enableBlending(bool enable) {
  m_config.enableDepthTest = enable;
  if (enable) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  } else {
    glDisable(GL_BLEND);
  };
}

void Engine::enableVSync(bool enable) {
  m_config.vsync = enable;
  if (m_windowManager) {
    m_windowManager->setVSync(enable);
  };
}

void Engine::pause() {
  m_isPaused = true;
  LOG_INFO("[Engine] Engine paused");
}

void Engine::resume() {
  m_isPaused = false;
  // Reset frame timeing to avoid large delta time jump
  m_lastFrameTime = std::chrono::high_resolution_clock::now();
  LOG_INFO("[Engine] Engine resumed");
}

void Engine::shutdown() {
  if (!m_isInitialized && !m_isRunning) {
    return;
  }

  LOG_INFO("[Engine] Shutting down engine...");
  m_isRunning = false;

  // Dispatch shutdown event
  EngineEvent shutdownEvent;
  shutdownEvent.type = EngineEventType::EngineShutdown;
  shutdownEvent.timestamp = m_totalTime;
  dispatchEvent(shutdownEvent);
  processEventQueue();

  shutdownSystems();

  m_isInitialized = false;
  LOG_INFO("[Engine] Engine shutdown completed");
}

void Engine::shutdownSystems() {
  // @TODO: Create Scene class
  // Clean up scenes
  // m_currentScene.reset();
  // m_nextScene.reset();

  // Clear event handlers
  m_eventHandlers.clear();
  m_eventQueue.clear();

  // WindowManager will clean up automatically through its destructor
  m_windowManager.reset();
}

void Engine::printSystemInfo() const {
  LOG_INFO("=== ENGINE SYSTEM INFORMATION ===");
  LOG_INFO_F("Engine Version: {}", getEngineVersion());
  LOG_INFO_F("Window Size: {}x{}", m_config.windowWidth, m_config.windowHeight);
  LOG_INFO_F("Fullscreen: {}", m_config.fullscreen ? "Yes" : "No");
  LOG_INFO_F("VSync: {}", m_config.vsync ? "Enabled" : "Disabled");
  LOG_INFO_F("MSAA Samples: {}", m_config.msaaSamples);

  if (m_windowManager) {
    LOG_INFO_F("OpenGL Version: {}", (const char*)glGetString(GL_VERSION));
    LOG_INFO_F("Graphics Card: {}", (const char*)glGetString(GL_RENDERER));
    LOG_INFO_F("OpenGL Vendor: {}", (const char*)glGetString(GL_VENDOR));
  }

  LOG_INFO("================================");
}

void Engine::printFrameStats() const {
  LOG_INFO("=== FRAME STATISTICS ===");
  LOG_INFO_F("Current FPS: {:.1f}", m_fps);
  LOG_INFO_F("Frame Time: {:.3f}ms", m_deltaTime * 1000.0f);
  LOG_INFO_F("Total Runtime: {:.2f}s", m_totalTime);
  LOG_INFO("========================");
}

std::string Engine::getBuildInfo() {
  std::ostringstream info;
  info << "Built on " << __DATE__ << " at " << __TIME__;
  info << " | OpenGL " << (const char*)glGetString(GL_VERSION);
  return info.str();
}
